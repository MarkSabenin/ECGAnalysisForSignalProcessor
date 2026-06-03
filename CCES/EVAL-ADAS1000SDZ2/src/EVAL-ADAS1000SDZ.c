/*******************************************************************************
 * EVAL-ADAS1000SDZ.c - Главный модуль системы сбора и классификации ЭКГ
 * Платформа: Blackfin BF527 + ADAS1000
 * Модель: Мультимодальная свёрточная нейронная сеть (DWT + STFT)
 ******************************************************************************/

#include <sys/adi_core.h>
#include <adi_initialize.h>
#include <sys/platform.h>
#include <cdefBF527.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "adas1000.h"
#include "project_config.h"
#include "ecg_processing.h"
#include "adas1000_data.h"
#include "adas1000_read_frame.h"
#include "ecg_leads.h"
#include "ecg_vector.h"
#include "dwt_bf527.h"
#include "stft_bf527.h"

/* MATLAB Coder сгенерированная модель */
#include "ecg_predict.h"
#include "ecg_predict_initialize.h"
#include "ecg_predict_terminate.h"

/* ============================================================================
 * КОНФИГУРАЦИОННЫЕ КОНСТАНТЫ
 * ============================================================================
 */
/* Частота дискретизации */
#define ADC_SAMPLE_RATE_HZ          2000    /* Реальная частота АЦП ADAS1000 (Гц) */
#define TARGET_SAMPLE_RATE_HZ       500     /* Целевая частота для модели (Гц) */
#define DECIMATION_FACTOR           (ADC_SAMPLE_RATE_HZ / TARGET_SAMPLE_RATE_HZ)  /* = 4 */

#define NUM_LEADS                   8
#define WINDOW_SIZE_MS              5000
#define WINDOW_SIZE_SAMPLES         (TARGET_SAMPLE_RATE_HZ * WINDOW_SIZE_MS / 1000)  /* 2500 */
#define STRIDE_MS                   1000
#define STRIDE_SAMPLES              (TARGET_SAMPLE_RATE_HZ * STRIDE_MS / 1000)       /* 500 */
#define AGGREGATION_WINDOWS         10

#define DWT_INPUT_SIZE              120000 /* 8x125x120 */
#define STFT_INPUT_SIZE             21672  /* 8x129x21 */

/* Размер кольцевого буфера: 10 секунд при целевой частоте 500 Гц */
#define RING_BUFFER_SIZE            (TARGET_SAMPLE_RATE_HZ * 10)  /* 5000 */

#define ADAS_NUM_CHANNELS           5

/* ============================================================================
 * КОНСТАНТЫ ДЛЯ ПРИНЯТИЯ РЕШЕНИЙ
 * ============================================================================
 */
/* Минимальные пороги вероятностей для 12 классов (в порядке возрастания классов 0-11) */
static const float MIN_PROBABILITY_THRESHOLDS[12] = {
    0.59f,  /* Класс 0 */
    0.12f,  /* Класс 1 */
    0.72f,  /* Класс 2 */
    0.54f,  /* Класс 3 */
    0.36f,  /* Класс 4 */
    0.45f,  /* Класс 5 */
    0.17f,  /* Класс 6 */
    0.43f,  /* Класс 7 */
    0.38f,  /* Класс 8 */
    0.77f,  /* Класс 9 */
    0.33f,  /* Класс 10 */
    0.33f   /* Класс 11 */
};

/* Дополнительная конфигурация принятия решений */
#define CONFIDENCE_MARGIN            0.10f  /* Минимальный отрыв от второго класса */
#define REJECTION_THRESHOLD          0.30f  /* Ниже этого значения - отклонять (нет диагноза) */
#define TEMPORAL_CONSISTENCY_WINDOWS 3      /* Количество окон для постановки диагноза */

/* Специальное значение для "нет диагноза" */
#define NO_DIAGNOSIS                 255

/* ============================================================================
 * СТРУКТУРЫ ДАННЫХ
 * ============================================================================
 */
typedef struct {
    float buffer[NUM_LEADS][RING_BUFFER_SIZE];
    uint32_t write_pos;
    uint32_t sample_count;
} ring_buffer_t;

typedef struct {
    float probs[AGGREGATION_WINDOWS][12];
    uint32_t window_idx;
    uint32_t ready_count;
    uint8_t last_class_id;
    float last_probs[12];
} inference_state_t;

/* Структура для хранения истории предсказаний */
typedef struct {
    uint8_t predictions[TEMPORAL_CONSISTENCY_WINDOWS];
    uint32_t window_index;
    uint32_t filled_count;
} prediction_history_t;

/* Структура для управления децимацией */
typedef struct {
    uint32_t counter;
    uint32_t factor;
    float buffer[NUM_LEADS];
    bool sample_ready;
} decimator_t;

/* ============================================================================
 * ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
 * ============================================================================
 */
static ring_buffer_t signal_ring;
static inference_state_t infer_state;
static adas_frame_t frame_buffer[512];
static adas_buffer_t ecg_buffer;
static ecg_processor_t ecg_processor;
static volatile uint32_t data_ready_flag = 0;
static volatile uint32_t frame_counter = 0;
static uint32_t inference_frame_counter = 0;

/* Дециматор */
static decimator_t decimator = {
    .counter = 0,
    .factor = DECIMATION_FACTOR,
    .sample_ready = false
};

static adas_config_t adas_config = {
    .frame_rate = ADC_SAMPLE_RATE_HZ,  /* 2000 Гц */
    .data_format = 0,
    .gain_setting = 0,
    .vref = 4.5f,
    .channels_enabled = 0x1F
};

static const float gain_factors[] = {1.4f, 2.1f, 2.8f, 4.2f};

/* Коэффициенты вейвлета db4 */
const float db4_dec_lo[8] = {
    -0.010597401785069, 0.032883011666885, 0.030841381835560,
    -0.187034811719093, -0.027983769416859, 0.630880767929859,
    0.714846570552915, 0.230377813307896
};

const float db4_dec_hi[8] = {
    -0.230377813307896, 0.714846570552915, -0.630880767929859,
    -0.027983769416859, 0.187034811719093, 0.030841381835560,
    -0.032883011666885, -0.010597401785069
};

/* Окно Хэмминга */
const float stft_hamming_window[STFT_WINDOW_LEN] = {
    0.080000, 0.080411, 0.081642, 0.083690, 0.086549, 0.090210, 0.094660, 0.099884,
    0.105864, 0.112578, 0.120000, 0.128101, 0.136848, 0.146202, 0.156122, 0.166563,
    0.177476, 0.188808, 0.200503, 0.212500, 0.224736, 0.237145, 0.249660, 0.262211,
    0.274727, 0.287135, 0.299362, 0.311333, 0.322973, 0.334207, 0.344958, 0.355153,
    0.364718, 0.373580, 0.381667, 0.388908, 0.395236, 0.400585, 0.404893, 0.408100,
    0.410152, 0.411000, 0.410600, 0.408914, 0.405910, 0.401562, 0.395852, 0.388769,
    0.380308, 0.370471, 0.359267, 0.346711, 0.332825, 0.317641, 0.301197, 0.283539,
    0.264722, 0.244807, 0.223862, 0.201963, 0.179191, 0.155636, 0.131394, 0.106565,
    0.081256, 0.055578, 0.029646, 0.003577, -0.022508, -0.048484, -0.074222, -0.099592,
    -0.124465, -0.148712, -0.172207, -0.194826, -0.216449, -0.236960, -0.256250, -0.274212,
    -0.290749, -0.305770, -0.319189, -0.330930, -0.340924, -0.349113, -0.355445, -0.359879,
    -0.362382, -0.362929, -0.361505, -0.358104, -0.352727, -0.345387, -0.336103, -0.324903,
    -0.311825, -0.296914, -0.280225, -0.261820, -0.241766, -0.220138, -0.197018, -0.172493,
    -0.146657, -0.119609, -0.091452, -0.062293, -0.032244, -0.001414, 0.030082, 0.061130,
    0.091618, 0.121435, 0.150474, 0.178633, 0.205815, 0.231929, 0.256889, 0.280617,
    0.303044, 0.324107, 0.343751, 0.361931, 0.378608, 0.393753, 0.407347, 0.419378,
    0.429844, 0.438749, 0.446106, 0.451933, 0.456258, 0.459112, 0.460539, 0.460586,
    0.459309, 0.456771, 0.453044, 0.448204, 0.442336, 0.435528, 0.427876, 0.419477,
    0.410433, 0.400850, 0.390834, 0.380495, 0.369945, 0.359296, 0.348659, 0.338143,
    0.327854, 0.317892, 0.308351, 0.299316, 0.290862, 0.283053, 0.275941, 0.269566,
    0.263956, 0.259129, 0.255094, 0.251847, 0.249374, 0.247651, 0.246645, 0.246316,
    0.246615, 0.247490, 0.248880, 0.250719, 0.252933, 0.255447, 0.258179, 0.261044,
    0.263954, 0.266820, 0.269554, 0.272067, 0.274274, 0.276093, 0.277446, 0.278259,
    0.278465, 0.278004, 0.276823, 0.274877, 0.272128, 0.268547, 0.264113, 0.258812,
    0.252638, 0.245588, 0.237669, 0.228893, 0.219279, 0.208853, 0.197647, 0.185698,
    0.173051, 0.159755, 0.145865, 0.131440, 0.116543, 0.101240, 0.085600, 0.069695,
    0.053599, 0.037388, 0.021140, 0.004933, -0.011152, -0.027027, -0.042603, -0.057792,
    -0.072508, -0.086667, -0.100187, -0.112991, -0.125003, -0.136152, -0.146371, -0.155598,
    -0.163775, -0.170848, -0.176770, -0.181498, -0.184996, -0.187234, -0.188188, -0.187844,
    -0.186196, -0.183245, -0.179000, -0.173476, -0.166698, -0.158697, -0.149512, -0.139185,
    -0.127765, -0.115307, -0.101873, -0.087528, -0.072344, -0.056397, -0.039767, -0.022540,
    -0.004804, 0.013354, 0.031846, 0.050578, 0.069456, 0.088383, 0.107264, 0.126000
};

/* ============================================================================
 * ФУНКЦИИ ЗАДЕРЖКИ
 * ============================================================================
 */
static void delay_us(uint32_t us)
{
    volatile uint32_t i;
    for (i = 0; i < us * 40; i++) {
        asm("nop;");
    }
}

static void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++) {
        delay_us(1000);
    }
}

/* ============================================================================
 * UART ФУНКЦИИ
 * ============================================================================
 */
static void uart_send_byte(uint8_t c)
{
    while (!(*pUART0_LSR & 0x0020)) {
        asm("nop;");
    }
    *pUART0_THR = c;
}

static void uart_send_string(const char *str)
{
    while (*str) {
        uart_send_byte(*str++);
    }
}

static void uart_send_int(int32_t val)
{
    char buffer[16];
    sprintf(buffer, "%ld", (long)val);
    uart_send_string(buffer);
}

static void uart_export_frame(const adas_frame_t *frame)
{
    char buffer[128];

    sprintf(buffer, "%lu,%lu,%.3f,%.3f,%.3f,%.3f,%.3f,%u\n",
            (unsigned long)frame->frame_number,
            (unsigned long)frame->system_time_ms,
            frame->channels[0].scaled_value,
            frame->channels[1].scaled_value,
            frame->channels[2].scaled_value,
            frame->channels[3].scaled_value,
            frame->channels[4].scaled_value,
            frame->leads_off_status);

    uart_send_string(buffer);
}

static void uart_export_stats(void)
{
    char buffer[128];

    uart_send_string("\n=== ECG Statistics ===\n");

    for (int i = 0; i < ADAS_NUM_CHANNELS; i++) {
        sprintf(buffer, "Channel %d: last=%.3fmV, quality=%d%%\n",
                i, ecg_processor.last_sample[i], ecg_processor.last_quality[i]);
        uart_send_string(buffer);
    }

    sprintf(buffer, "Total frames (ADC): %lu\n", (unsigned long)frame_counter);
    uart_send_string(buffer);
    sprintf(buffer, "Inference frames: %lu\n", (unsigned long)inference_frame_counter);
    uart_send_string(buffer);

    if (infer_state.ready_count >= AGGREGATION_WINDOWS) {
        if (infer_state.last_class_id == NO_DIAGNOSIS) {
            uart_send_string("Last prediction: NO DIAGNOSIS (rejected)\n");
        } else {
            sprintf(buffer, "Last prediction: class %d\n", infer_state.last_class_id);
            uart_send_string(buffer);
        }
    }

    uart_send_string("======================\n\n");
}

/* ============================================================================
 * ФУНКЦИИ ПРИНЯТИЯ РЕШЕНИЙ
 * ============================================================================
 */

/**
 * Проверяет, превышает ли вероятность предсказания минимальный порог для данного класса
 */
static bool is_probability_above_threshold(uint8_t class_id, float probability)
{
    if (class_id >= 12) {
        return false;
    }
    return (probability >= MIN_PROBABILITY_THRESHOLDS[class_id]);
}

/**
 * Находит класс с максимальной вероятностью и проверяет отрыв от второго
 * Возвращает:
 *   - class_id, если уверенность достаточна
 *   - NO_DIAGNOSIS (255), если предсказание следует отклонить
 */
static uint8_t get_class_with_confidence(float* probs, float* max_prob_out, float* second_max_out)
{
    uint8_t max_class = 0;
    uint8_t second_class = 0;
    float max_prob = probs[0];
    float second_max_prob = 0.0f;

    for (int i = 1; i < 12; i++) {
        if (probs[i] > max_prob) {
            second_max_prob = max_prob;
            second_class = max_class;
            max_prob = probs[i];
            max_class = (uint8_t)i;
        } else if (probs[i] > second_max_prob) {
            second_max_prob = probs[i];
            second_class = (uint8_t)i;
        }
    }

    if (max_prob_out) *max_prob_out = max_prob;
    if (second_max_out) *second_max_out = second_max_prob;

    if (max_prob < REJECTION_THRESHOLD) {
        return NO_DIAGNOSIS;
    }

    float margin = max_prob - second_max_prob;
    if (margin < CONFIDENCE_MARGIN && max_prob < 0.6f) {
        return NO_DIAGNOSIS;
    }

    return max_class;
}

/**
 * Обновляет историю предсказаний и проверяет временную консистенцию
 */
static bool update_prediction_history(prediction_history_t* history, uint8_t new_prediction)
{
    history->predictions[history->window_index] = new_prediction;
    history->window_index = (history->window_index + 1) % TEMPORAL_CONSISTENCY_WINDOWS;

    if (history->filled_count < TEMPORAL_CONSISTENCY_WINDOWS) {
        history->filled_count++;
    }

    if (history->filled_count == TEMPORAL_CONSISTENCY_WINDOWS) {
        uint8_t first = history->predictions[0];
        for (int i = 1; i < TEMPORAL_CONSISTENCY_WINDOWS; i++) {
            if (history->predictions[i] != first) {
                return false;
            }
        }
        return true;
    }

    return false;
}

/**
 * Применяет пороговые значения к вероятностям предсказания
 */
static void apply_thresholds_to_probabilities(float* probs)
{
    for (int i = 0; i < 12; i++) {
        if (probs[i] < MIN_PROBABILITY_THRESHOLDS[i]) {
            probs[i] = 0.0f;
        }
    }

    float sum = 0.0f;
    for (int i = 0; i < 12; i++) {
        sum += probs[i];
    }

    if (sum > 1e-6f) {
        for (int i = 0; i < 12; i++) {
            probs[i] /= sum;
        }
    }
}

/**
 * Выводит информацию о порогах и диагнозах
 */
static void print_thresholds(void)
{
    char buffer[256];
    uart_send_string("\n=== DIAGNOSIS THRESHOLDS ===\n");
    uart_send_string("Class | Threshold | Description\n");
    uart_send_string("------|-----------|-------------\n");

    for (int i = 0; i < 12; i++) {
        sprintf(buffer, "  %2d  |    %.2f    |\n", i, MIN_PROBABILITY_THRESHOLDS[i]);
        uart_send_string(buffer);
    }

    uart_send_string("------|-----------|-------------\n");
    sprintf(buffer, "Confidence margin: %.2f\n", CONFIDENCE_MARGIN);
    uart_send_string(buffer);
    sprintf(buffer, "Rejection threshold: %.2f\n", REJECTION_THRESHOLD);
    uart_send_string(buffer);
    sprintf(buffer, "Temporal consistency windows: %d\n", TEMPORAL_CONSISTENCY_WINDOWS);
    uart_send_string(buffer);
    uart_send_string("================================\n\n");
}

/**
 * Расширенный вывод предсказания с информацией о порогах
 */
static void uart_print_prediction_detailed(uint8_t class_id, const float* probs,
                                            float max_prob, float second_max_prob)
{
    char buffer[256];

    if (class_id == NO_DIAGNOSIS) {
        uart_send_string("\n=== PREDICTION REJECTED ===\n");
        uart_send_string("Insufficient confidence (below thresholds or margin)\n");

        sprintf(buffer, "Max prob: %.3f, Second max: %.3f\n", max_prob, second_max_prob);
        uart_send_string(buffer);

        uart_send_string("Raw probs: [");
        for (int i = 0; i < 12; i++) {
            sprintf(buffer, "%.3f", probs[i]);
            uart_send_string(buffer);
            if (i < 11) uart_send_string(", ");
        }
        uart_send_string("]\n");
    } else {
        uart_send_string("\n=== PREDICTION ===\n");
        sprintf(buffer, "Class: %d (threshold %.2f, prob %.3f)\n",
                class_id, MIN_PROBABILITY_THRESHOLDS[class_id], max_prob);
        uart_send_string(buffer);

        if (max_prob - second_max_prob >= CONFIDENCE_MARGIN) {
            sprintf(buffer, "Confidence margin: %.3f (OK)\n", max_prob - second_max_prob);
        } else {
            sprintf(buffer, "Confidence margin: %.3f (low but accepted)\n", max_prob - second_max_prob);
        }
        uart_send_string(buffer);

        uart_send_string("Probs: [");
        for (int i = 0; i < 12; i++) {
            sprintf(buffer, "%.3f", probs[i]);
            uart_send_string(buffer);
            if (i < 11) uart_send_string(", ");
        }
        uart_send_string("]\n");

        uart_send_string("Thresholds exceeded: ");
        bool first = true;
        for (int i = 0; i < 12; i++) {
            if (probs[i] >= MIN_PROBABILITY_THRESHOLDS[i]) {
                if (!first) uart_send_string(", ");
                sprintf(buffer, "%d(%.3f/%.2f)", i, probs[i], MIN_PROBABILITY_THRESHOLDS[i]);
                uart_send_string(buffer);
                first = false;
            }
        }
        if (first) uart_send_string("none");
        uart_send_string("\n");
    }
    uart_send_string("================\n\n");
}

/* ============================================================================
 * КОЛЬЦЕВОЙ БУФЕР
 * ============================================================================
 */
static void ring_buffer_init(ring_buffer_t *rb)
{
    memset(rb, 0, sizeof(ring_buffer_t));
    rb->write_pos = 0;
    rb->sample_count = 0;
}

static void ring_buffer_add_sample(ring_buffer_t *rb, const float samples[NUM_LEADS])
{
    for (int lead = 0; lead < NUM_LEADS; lead++) {
        rb->buffer[lead][rb->write_pos] = samples[lead];
    }

    rb->write_pos = (rb->write_pos + 1) % RING_BUFFER_SIZE;
    if (rb->sample_count < RING_BUFFER_SIZE) {
        rb->sample_count++;
    }
}

static uint32_t ring_buffer_get_window(ring_buffer_t *rb, float* output, uint32_t start_offset)
{
    if (rb->sample_count < WINDOW_SIZE_SAMPLES + start_offset) {
        return 0;
    }

    uint32_t start_pos = (rb->write_pos - rb->sample_count + start_offset) % RING_BUFFER_SIZE;

    for (int lead = 0; lead < NUM_LEADS; lead++) {
        for (uint32_t i = 0; i < WINDOW_SIZE_SAMPLES; i++) {
            uint32_t idx = (start_pos + i) % RING_BUFFER_SIZE;
            output[lead * WINDOW_SIZE_SAMPLES + i] = rb->buffer[lead][idx];
        }
    }

    return 1;
}

/* ============================================================================
 * ДЕЦИМАЦИЯ
 * ============================================================================
 */

/**
 * Инициализация дециматора
 */
static void decimator_init(decimator_t *dec)
{
    dec->counter = 0;
    dec->factor = DECIMATION_FACTOR;
    dec->sample_ready = false;
    for (int i = 0; i < NUM_LEADS; i++) {
        dec->buffer[i] = 0.0f;
    }
}

/**
 * Обработка отсчёта с децимацией
 * Возвращает true, когда накоплен достаточный буфер и новый отсчёт готов
 */
static bool decimator_process(decimator_t *dec, const float new_samples[NUM_LEADS], float output_samples[NUM_LEADS])
{
    /* Накопление суммы для усреднения (anti-aliasing) */
    for (int i = 0; i < NUM_LEADS; i++) {
        dec->buffer[i] += new_samples[i];
    }

    dec->counter++;

    if (dec->counter >= dec->factor) {
        /* Вычисляем среднее значение для сглаживания */
        float inv_factor = 1.0f / dec->factor;
        for (int i = 0; i < NUM_LEADS; i++) {
            output_samples[i] = dec->buffer[i] * inv_factor;
            dec->buffer[i] = 0.0f;
        }
        dec->counter = 0;
        return true;
    }

    return false;
}

/* ============================================================================
 * ОБРАБОТКА СИГНАЛА
 * ============================================================================
 */
static void compute_all_leads(const float raw[5], float leads[NUM_LEADS])
{
    float la = raw[0];
    float ll = raw[1];
    float ra = raw[2];
    float v1 = raw[3];
    float v2 = raw[4];

    float lead_I = la - ra;
    float lead_II = ll - ra;
    float lead_III = ll - la;
    float aVR = -(lead_I + lead_II) / 2.0f;
    float aVL = lead_I - lead_II / 2.0f;
    float aVF = lead_II - lead_I / 2.0f;

    leads[0] = lead_I;
    leads[1] = lead_II;
    leads[2] = lead_III;
    leads[3] = aVR;
    leads[4] = aVL;
    leads[5] = aVF;
    leads[6] = v1;
    leads[7] = v2;
}

static void normalize_window(float* window, uint32_t length)
{
    for (int lead = 0; lead < NUM_LEADS; lead++) {
        float mean = 0.0f;
        float std = 0.0f;

        for (uint32_t i = 0; i < length; i++) {
            mean += window[lead * length + i];
        }
        mean /= length;

        for (uint32_t i = 0; i < length; i++) {
            float diff = window[lead * length + i] - mean;
            std += diff * diff;
        }
        std = sqrtf(std / length);

        if (std < 1e-6f) std = 1e-6f;

        for (uint32_t i = 0; i < length; i++) {
            window[lead * length + i] = (window[lead * length + i] - mean) / std;
        }
    }
}

/* ============================================================================
 * DWT И STFT
 * ============================================================================
 */
static void dwt_to_scalogram(const dwt_coeffs_t *coeffs, float *output, uint32_t h, uint32_t w, uint32_t ch)
{
    (void)ch;

    uint32_t max_samples = (h * w < 15000) ? (h * w) : 15000;

    for (uint32_t i = 0; i < max_samples; i++) {
        output[i] = coeffs->cA[0][i];
    }
}

static void compute_dwt_scalogram(const float* window, float* dwt_output)
{
    dwt_coeffs_t coeffs;

    for (int lead = 0; lead < NUM_LEADS; lead++) {
        const float* signal = window + lead * WINDOW_SIZE_SAMPLES;
        float* out = dwt_output + lead * 125 * 120;

        dwt_decompose(signal, WINDOW_SIZE_SAMPLES, &coeffs);
        dwt_to_scalogram(&coeffs, out, 125, 120, lead);
    }
}

static void stft_to_spectrogram(const spectrogram_t *spec, float *output, uint32_t h, uint32_t w)
{
    for (uint32_t i = 0; i < h; i++) {
        for (uint32_t j = 0; j < w; j++) {
            output[i * w + j] = spec->magnitude[i][j];
        }
    }
}

static void compute_stft_spectrogram(const float* window, float* stft_output)
{
    spectrogram_t spec;

    for (int lead = 0; lead < NUM_LEADS; lead++) {
        const float* signal = window + lead * WINDOW_SIZE_SAMPLES;
        float* out = stft_output + lead * 129 * 21;

        stft_compute(signal, WINDOW_SIZE_SAMPLES, &spec);
        stft_to_spectrogram(&spec, out, 129, 21);
    }
}

/* ============================================================================
 * ИНФЕРЕНС МОДЕЛИ (С ВЕСАМИ И ПОРОГАМИ)
 * ============================================================================
 */
static void inference_init(inference_state_t* state)
{
    memset(state, 0, sizeof(inference_state_t));
    state->window_idx = 0;
    state->ready_count = 0;
    state->last_class_id = NO_DIAGNOSIS;

    for (int i = 0; i < 12; i++) {
        state->last_probs[i] = 0.0f;
    }

    for (int i = 0; i < AGGREGATION_WINDOWS; i++) {
        for (int j = 0; j < 12; j++) {
            state->probs[i][j] = 0.0f;
        }
    }
}

static void inference_aggregate(inference_state_t* state, uint8_t* class_id, float* probs)
{
    float agg_probs[12] = {0};
    float temp_probs[12];

    for (int c = 0; c < 12; c++) {
        for (int w = 0; w < AGGREGATION_WINDOWS; w++) {
            if (state->probs[w][c] > agg_probs[c]) {
                agg_probs[c] = state->probs[w][c];
            }
        }
    }

    memcpy(temp_probs, agg_probs, sizeof(agg_probs));
    apply_thresholds_to_probabilities(temp_probs);

    float max_prob, second_max;
    uint8_t raw_class = get_class_with_confidence(temp_probs, &max_prob, &second_max);

    memcpy(probs, agg_probs, sizeof(agg_probs));
    memcpy(state->last_probs, agg_probs, sizeof(agg_probs));

    if (raw_class == NO_DIAGNOSIS) {
        *class_id = NO_DIAGNOSIS;
        state->last_class_id = NO_DIAGNOSIS;
        uart_print_prediction_detailed(NO_DIAGNOSIS, agg_probs, max_prob, second_max);
    } else {
        *class_id = raw_class;
        state->last_class_id = raw_class;
        uart_print_prediction_detailed(raw_class, agg_probs, max_prob, second_max);
    }
}

static void inference_process_window(inference_state_t* state, const float* window, uint32_t timestamp_ms)
{
    float* dwt_input = (float*)malloc(DWT_INPUT_SIZE * sizeof(float));
    float* stft_input = (float*)malloc(STFT_INPUT_SIZE * sizeof(float));
    float window_norm[NUM_LEADS * WINDOW_SIZE_SAMPLES];

    static prediction_history_t history = {0};
    static uint32_t last_stable_frame = 0;
    static uint8_t last_stable_prediction = NO_DIAGNOSIS;
    static uint32_t stable_confirmation_counter = 0;

    if (!dwt_input || !stft_input) {
        uart_send_string("ERROR: Failed to allocate inference buffers!\n");
        free(dwt_input);
        free(stft_input);
        return;
    }

    memcpy(window_norm, window, sizeof(window_norm));
    normalize_window(window_norm, WINDOW_SIZE_SAMPLES);

    compute_dwt_scalogram(window_norm, dwt_input);
    compute_stft_spectrogram(window_norm, stft_input);

    unsigned char class_id;
    float probabilities[12];

    ecg_predict(dwt_input, stft_input, &class_id, probabilities);

    memcpy(state->probs[state->window_idx], probabilities, sizeof(probabilities));
    state->window_idx = (state->window_idx + 1) % AGGREGATION_WINDOWS;

    if (state->ready_count < AGGREGATION_WINDOWS) {
        state->ready_count++;
    }

    if (state->ready_count >= AGGREGATION_WINDOWS) {
        uint8_t agg_class_id;
        float agg_probs[12];

        inference_aggregate(state, &agg_class_id, agg_probs);

        if (agg_class_id != NO_DIAGNOSIS) {
            if (update_prediction_history(&history, agg_class_id)) {
                if (last_stable_prediction != agg_class_id) {
                    char buffer[128];
                    uart_send_string("\n*** STABLE DIAGNOSIS CONFIRMED ***\n");
                    sprintf(buffer, "Class %d confirmed over %d windows\n",
                            agg_class_id, TEMPORAL_CONSISTENCY_WINDOWS);
                    uart_send_string(buffer);
                    last_stable_prediction = agg_class_id;
                    stable_confirmation_counter = 0;
                }
                last_stable_frame = timestamp_ms;
            }
        } else {
            if (history.filled_count >= TEMPORAL_CONSISTENCY_WINDOWS) {
                if (last_stable_prediction != NO_DIAGNOSIS) {
                    stable_confirmation_counter++;
                    if (stable_confirmation_counter >= 5) {
                        uart_send_string("\n*** STABLE DIAGNOSIS EXPIRED ***\n");
                        last_stable_prediction = NO_DIAGNOSIS;
                        stable_confirmation_counter = 0;
                        memset(&history, 0, sizeof(history));
                    }
                } else {
                    if (timestamp_ms - last_stable_frame > 30000) {
                        memset(&history, 0, sizeof(history));
                    }
                }
            }
        }

        inference_frame_counter++;
    }

    free(dwt_input);
    free(stft_input);

    (void)timestamp_ms;
}

/* ============================================================================
 * SPI ФУНКЦИИ
 * ============================================================================
 */
static uint8_t spi_transfer_byte(uint8_t data)
{
    while (!(*pSPI_STAT & 0x0002)) {
        asm("nop;");
    }
    *pSPI_TDBR = data;
    asm("ssync;");

    while (!(*pSPI_STAT & 0x0001)) {
        asm("nop;");
    }
    return (uint8_t)*pSPI_RDBR;
}

static void spi_transfer_block(uint8_t *tx_data, uint8_t *rx_data, uint16_t length)
{
    *pPORTFIO_CLEAR = PIN_CS;
    asm("ssync;");

    for (uint16_t i = 0; i < length; i++) {
        uint8_t tx = tx_data ? tx_data[i] : 0xFF;
        uint8_t rx = spi_transfer_byte(tx);
        if (rx_data) {
            rx_data[i] = rx;
        }
    }

    *pPORTFIO_SET = PIN_CS;
    asm("ssync;");
}

/* ============================================================================
 * ФУНКЦИИ ADAS1000
 * ============================================================================
 */
static void adas1000_write_register(uint8_t reg_addr, uint32_t value)
{
    uint8_t cmd[4];
    cmd[0] = 0x80;
    cmd[1] = reg_addr;
    cmd[2] = (value >> 8) & 0xFF;
    cmd[3] = value & 0xFF;

    *pPORTFIO_CLEAR = PIN_CS;
    asm("ssync;");

    for (int i = 0; i < 4; i++) {
        spi_transfer_byte(cmd[i]);
    }

    *pPORTFIO_SET = PIN_CS;
    asm("ssync;");
}

static uint32_t adas1000_read_register(uint8_t reg_addr)
{
    uint8_t cmd[4] = {0x00, reg_addr, 0x00, 0x00};
    uint8_t resp[4];

    *pPORTFIO_CLEAR = PIN_CS;
    asm("ssync;");

    for (int i = 0; i < 4; i++) {
        resp[i] = spi_transfer_byte(cmd[i]);
    }

    *pPORTFIO_SET = PIN_CS;
    asm("ssync;");

    return (resp[0] << 24) | (resp[1] << 16) | (resp[2] << 8) | resp[3];
}

static void adas1000_hardware_reset(void)
{
    uart_send_string("Hardware reset...\n");
    *pPORTFIO_CLEAR = PIN_RESET;
    delay_ms(10);
    *pPORTFIO_SET = PIN_RESET;
    delay_ms(100);
}

static void adas1000_configure(void)
{
    uint32_t reg_val;

    uart_send_string("Configuring ADAS1000...\n");

    reg_val = 0x00E00006;
    adas1000_write_register(ADAS1000_ECGCTL, reg_val);
    uart_send_string("  ECGCTL = 0x00E00006\n");
    delay_ms(10);

    reg_val = 0x00000000;
    adas1000_write_register(ADAS1000_FRMCTL, reg_val);
    uart_send_string("  FRMCTL = 0x00000000 (2kHz output)\n");
    delay_ms(5);

    reg_val = 0x00000000;
    adas1000_write_register(ADAS1000_FILTCTL, reg_val);
    uart_send_string("  FILTCTL = 0x00000000 (LPF=40Hz)\n");
    delay_ms(5);

    reg_val = 0x00000001;
    adas1000_write_register(ADAS1000_LOFFCTL, reg_val);
    uart_send_string("  LOFFCTL = 0x00000001\n");

    uart_send_string("ADAS1000 configuration complete.\n");
}

static void adas1000_read_frame(adas_frame_t *frame)
{
    uint8_t header_cmd[4] = {0x00, ADAS1000_FRAMES, 0x00, 0x00};
    uint8_t header_resp[4];
    uint32_t header;

    spi_transfer_block(header_cmd, header_resp, 4);
    header = (header_resp[0] << 24) | (header_resp[1] << 16) |
             (header_resp[2] << 8) | header_resp[3];

    frame->ready = (header & ADAS_FRAMES_READY_BIT) ? 0 : 1;
    frame->header = header;
    frame->overflow = (header >> 28) & 0x03;
    frame->fault = (header >> 27) & 0x01;
    frame->leads_off_status = (header >> 22) & 0x01;
    frame->dc_leads_off_status = (header >> 21) & 0x01;

    uint8_t reg_addrs[5] = {ADAS1000_LADATA, ADAS1000_LLDATA, ADAS1000_RADATA,
                            ADAS1000_V1DATA, ADAS1000_V2DATA};

    for (int i = 0; i < ADAS_NUM_CHANNELS; i++) {
        uint8_t cmd[4] = {0x00, reg_addrs[i], 0x00, 0x00};
        uint8_t resp[4];

        spi_transfer_block(cmd, resp, 4);

        uint32_t raw = (resp[0] << 24) | (resp[1] << 16) |
                       (resp[2] << 8) | resp[3];

        frame->channels[i].raw_value = raw & 0x00FFFFFF;

        if (frame->channels[i].raw_value & 0x00800000) {
            frame->channels[i].raw_value |= 0xFF000000;
        }

        frame->channels[i].scaled_value = (float)frame->channels[i].raw_value *
                                          4.5f / (1.4f * 16777216.0f) * 1000.0f;
        frame->channels[i].channel_id = i;
        frame->channels[i].quality = 100;
        frame->channels[i].timestamp_us = frame_counter * 1000;
        frame->channels[i].filtered_value = frame->channels[i].scaled_value;
    }

    frame->frame_number = frame_counter;
    frame->system_time_ms = frame_counter * 1000 / ADC_SAMPLE_RATE_HZ;
    frame_counter++;
}

/* ============================================================================
 * ИНИЦИАЛИЗАЦИЯ BF527
 * ============================================================================
 */
static void bf527_system_init(void)
{
    asm("ssync;");
}

static void bf527_uart_init(void)
{
    uint16_t lcr_temp;

    *pUART0_GCTL = 0x0001;
    asm("ssync;");

    lcr_temp = *pUART0_LCR;
    *pUART0_LCR = lcr_temp | 0x0080;
    asm("ssync;");

    *pUART0_DLL = 0x006C;
    *pUART0_DLH = 0x0000;
    asm("ssync;");

    *pUART0_LCR = 0x0003;
    asm("ssync;");

    uart_send_string("UART initialized at 115200 baud\n");
}

static void bf527_gpio_control_init(void)
{
    *pPORTF_FER |= (PIN_CS | PIN_DRDY | PIN_RESET);
    asm("ssync;");

    *pPORTFIO_DIR |= PIN_CS;
    *pPORTFIO_SET = PIN_CS;
    asm("ssync;");

    *pPORTFIO_DIR &= ~PIN_DRDY;
    *pPORTFIO_INEN |= PIN_DRDY;
    asm("ssync;");

    *pPORTFIO_DIR |= PIN_RESET;
    *pPORTFIO_SET = PIN_RESET;
    asm("ssync;");

    uart_send_string("GPIO control pins initialized\n");
}

static void bf527_spi_init(void)
{
    *pPORTF_FER |= 0x0700;
    *pPORTFIO_DIR |= 0x0500;
    *pPORTFIO_DIR &= ~0x0200;
    asm("ssync;");

    *pSPI_BAUD = 49;
    asm("ssync;");

    uint16_t ctl = 0;
    ctl |= 0x0001;
    ctl |= 0x0002;
    ctl |= 0x0010;
    ctl |= 0x0020;
    ctl |= 0x0100;
    *pSPI_CTL = ctl;
    asm("ssync;");

    uart_send_string("SPI initialized: 2MHz, Mode 3, Master\n");
}

static void bf527_interrupt_init(void)
{
    *pPORTFIO_INEN |= PIN_DRDY;
    *pPORTFIO_MASKA |= PIN_DRDY;
    *pPORTFIO_EDGE |= PIN_DRDY;
    *pPORTFIO_BOTH &= ~PIN_DRDY;
    *pPORTFIO_POLAR &= ~PIN_DRDY;
    asm("ssync;");

    uint32_t iar = *pSIC_IAR0;
    iar &= ~(0xFF << 16);
    iar |= (0x0B << 16);
    *pSIC_IAR0 = iar;
    asm("ssync;");

    *pSIC_IMASK0 |= (1 << 11);
    asm("ssync;");

    uart_send_string("Interrupts initialized (DRDY on IVG11)\n");
}

static void bf527_adas1000_init(void)
{
    char hex[16];

    uart_send_string("\n========================================\n");
    uart_send_string("ADAS1000 Initialization\n");
    uart_send_string("========================================\n");

    adas1000_hardware_reset();

    uart_send_string("Checking communication...\n");
    uint32_t ecgctl = adas1000_read_register(ADAS1000_ECGCTL);
    sprintf(hex, "%08lX", (unsigned long)ecgctl);
    uart_send_string("  ECGCTL = 0x");
    uart_send_string(hex);
    uart_send_string("\n");

    if (ecgctl != 0xFFFFFFFF) {
        uart_send_string("  Communication OK\n");
    } else {
        uart_send_string("  WARNING: Communication failed!\n");
    }

    adas1000_configure();

    uart_send_string("========================================\n");
    uart_send_string("ADAS1000 Initialization COMPLETE!\n");
    uart_send_string("========================================\n\n");
}

/* ============================================================================
 * ОБРАБОТКА ЭКГ
 * ============================================================================
 */
static void ecg_processor_init(ecg_processor_t *proc)
{
    memset(proc, 0, sizeof(ecg_processor_t));

    for (int i = 0; i < ADAS_NUM_CHANNELS; i++) {
        proc->last_sample[i] = 0.0f;
        proc->last_quality[i] = 100;
        proc->gain_calibration[i] = 1.0f;
        proc->offset_calibration[i] = 0.0f;
    }

    proc->qrs_threshold = 0.5f;
    proc->refractory_period_frames = 200 * ADC_SAMPLE_RATE_HZ / 1000;
    proc->heart_rate_counter = 0;
    proc->last_r_peak_frame = 0;
    proc->last_r_peak_time_ms = 0;
    proc->total_frames_processed = 0;
}

static void ecg_process_frame(adas_frame_t *frame, ecg_processor_t *proc)
{
    if (!frame->ready) return;

    for (int i = 0; i < ADAS_NUM_CHANNELS; i++) {
        proc->last_sample[i] = frame->channels[i].scaled_value;
        proc->last_quality[i] = frame->channels[i].quality;
    }

    proc->total_frames_processed++;
}

/* ============================================================================
 * ОТЛАДОЧНЫЕ ФУНКЦИИ
 * ============================================================================
 */
void dump_adas1000_registers(void)
{
    uint32_t reg_value;
    char buffer[64];

    uart_send_string("\n=== ADAS1000 REGISTER DUMP ===\n");

    reg_value = adas1000_read_register(ADAS1000_ECGCTL);
    sprintf(buffer, "ECGCTL (0x01): 0x%08lX\n", (unsigned long)reg_value);
    uart_send_string(buffer);

    reg_value = adas1000_read_register(ADAS1000_FRMCTL);
    sprintf(buffer, "FRMCTL (0x0A): 0x%08lX\n", (unsigned long)reg_value);
    uart_send_string(buffer);

    reg_value = adas1000_read_register(ADAS1000_FILTCTL);
    sprintf(buffer, "FILTCTL (0x0B): 0x%08lX\n", (unsigned long)reg_value);
    uart_send_string(buffer);

    reg_value = adas1000_read_register(ADAS1000_LOFFCTL);
    sprintf(buffer, "LOFFCTL (0x02): 0x%08lX\n", (unsigned long)reg_value);
    uart_send_string(buffer);

    uart_send_string("=============================\n\n");
}

void print_lead_status(void)
{
    uint32_t loff_status;
    char buffer[64];

    loff_status = adas1000_read_register(ADAS1000_LOFF);

    uart_send_string("\n=== LEADS STATUS ===\n");

    sprintf(buffer, "LA (Lead I):   %s\n", (loff_status & 0x00400000) ? "OFF" : "ON");
    uart_send_string(buffer);

    sprintf(buffer, "LL (Lead II):  %s\n", (loff_status & 0x00200000) ? "OFF" : "ON");
    uart_send_string(buffer);

    sprintf(buffer, "RA (Lead III): %s\n", (loff_status & 0x00100000) ? "OFF" : "ON");
    uart_send_string(buffer);

    sprintf(buffer, "V1:            %s\n", (loff_status & 0x00080000) ? "OFF" : "ON");
    uart_send_string(buffer);

    sprintf(buffer, "V2:            %s\n", (loff_status & 0x00040000) ? "OFF" : "ON");
    uart_send_string(buffer);

    uart_send_string("==================\n\n");
}

/* ============================================================================
 * ПРЕРЫВАНИЕ DRDY
 * ============================================================================
 */
void drdy_interrupt_handler(void)
{
    adas_frame_t frame;
    float leads[NUM_LEADS];
    float decimated_leads[NUM_LEADS];
    float raw[5];

    /* Очистка флага прерывания GPIO */
    *pPORTFIO_CLEAR = PIN_DRDY;

    /* Чтение кадра данных */
    adas1000_read_frame(&frame);

    /* Сохранение в кольцевой буфер */
    adas_buffer_put(&ecg_buffer, &frame);

    /* Расчёт отведений из сырых данных текущего кадра */
    raw[0] = frame.channels[0].scaled_value;  /* LA */
    raw[1] = frame.channels[1].scaled_value;  /* LL */
    raw[2] = frame.channels[2].scaled_value;  /* RA */
    raw[3] = frame.channels[3].scaled_value;  /* V1 */
    raw[4] = frame.channels[4].scaled_value;  /* V2 */

    compute_all_leads(raw, leads);

    /* ДЕЦИМАЦИЯ: преобразование 2000 Гц → 500 Гц */
    if (decimator_process(&decimator, leads, decimated_leads)) {
        /* Добавление прореженного отсчёта в кольцевой буфер */
        ring_buffer_add_sample(&signal_ring, decimated_leads);
    }

    /* Установка флага для main loop */
    data_ready_flag = 1;

    /* Очистка прерывания в контроллере SIC */
    *pSIC_ISR0 = (1 << 11);
}

/* ============================================================================
 * DWT И STFT (ВСПОМОГАТЕЛЬНЫЕ)
 * ============================================================================
 */
void dwt_decompose(const float *signal, uint32_t len, dwt_coeffs_t *out)
{
    uint32_t i, j, k;
    float *temp = (float*)malloc(len * sizeof(float));

    memcpy(temp, signal, len * sizeof(float));
    out->len[0] = len;

    for (int level = 0; level < DWT_LEVELS; level++) {
        uint32_t curr_len = out->len[level];
        uint32_t new_len = curr_len / 2;

        for (i = 0; i < new_len; i++) {
            float sum_lo = 0, sum_hi = 0;
            for (j = 0; j < 8; j++) {
                k = 2*i + j;
                if (k < curr_len) {
                    sum_lo += temp[k] * db4_dec_lo[j];
                    sum_hi += temp[k] * db4_dec_hi[j];
                }
            }
            out->cA[level][i] = sum_lo;
            out->cD[level][i] = sum_hi;
        }

        memcpy(temp, out->cA[level], new_len * sizeof(float));
        out->len[level+1] = new_len;
    }

    free(temp);
}

static void fft_brute_force(float *real, float *imag, uint32_t n)
{
    uint32_t i, j, k, m;
    uint32_t n2 = n / 2;
    float tmp_real, tmp_imag;

    j = 0;
    for (i = 0; i < n - 1; i++) {
        if (i < j) {
            tmp_real = real[i];
            tmp_imag = imag[i];
            real[i] = real[j];
            imag[i] = imag[j];
            real[j] = tmp_real;
            imag[j] = tmp_imag;
        }
        k = n2;
        while (k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
    }

    for (m = 1; m <= n2; m *= 2) {
        float angle = -3.14159265359f / m;
        float w_real = cosf(angle);
        float w_imag = sinf(angle);

        for (i = 0; i < n; i += 2 * m) {
            float wr = 1.0f;
            float wi = 0.0f;
            for (j = 0; j < m; j++) {
                int k_idx = i + j + m;
                float tr = wr * real[k_idx] - wi * imag[k_idx];
                float ti = wr * imag[k_idx] + wi * real[k_idx];
                real[k_idx] = real[i + j] - tr;
                imag[k_idx] = imag[i + j] - ti;
                real[i + j] = real[i + j] + tr;
                imag[i + j] = imag[i + j] + ti;

                float new_wr = wr * w_real - wi * w_imag;
                float new_wi = wr * w_imag + wi * w_real;
                wr = new_wr;
                wi = new_wi;
            }
        }
    }
}

void stft_compute(const float *signal, uint32_t signal_len, spectrogram_t *out)
{
    uint32_t i, j, t;
    uint32_t step = STFT_WINDOW_LEN - STFT_OVERLAP;
    uint32_t num_windows = (signal_len - STFT_WINDOW_LEN) / step + 1;
    float real[STFT_NFFT];
    float imag[STFT_NFFT];

    out->num_freq_bins = STFT_OUT_H;
    out->num_time_windows = num_windows;

    for (t = 0; t < num_windows; t++) {
        uint32_t start = t * step;

        memset(real, 0, sizeof(real));
        memset(imag, 0, sizeof(imag));

        for (j = 0; j < STFT_WINDOW_LEN && (start + j) < signal_len; j++) {
            real[j] = signal[start + j] * stft_hamming_window[j];
        }

        fft_brute_force(real, imag, STFT_NFFT);

        for (i = 0; i < STFT_OUT_H; i++) {
            float mag = sqrtf(real[i] * real[i] + imag[i] * imag[i]);
            out->magnitude[i][t] = mag;
        }
    }
}

void stft_to_multichannel(const float *signal_12leads[12], spectrogram_t *out_spectrograms[12])
{
    for (int lead = 0; lead < 12; lead++) {
        stft_compute(signal_12leads[lead], STFT_WINDOW_SIZE, out_spectrograms[lead]);
    }
}

void stft_normalize(spectrogram_t *spec)
{
    uint32_t i, t;
    float max_val = 0.0f;
    float min_val = 1e6f;

    for (i = 0; i < spec->num_freq_bins; i++) {
        for (t = 0; t < spec->num_time_windows; t++) {
            float val = spec->magnitude[i][t];
            if (val > max_val) max_val = val;
            if (val < min_val) min_val = val;
        }
    }

    float range = max_val - min_val;
    if (range > 1e-6f) {
        for (i = 0; i < spec->num_freq_bins; i++) {
            for (t = 0; t < spec->num_time_windows; t++) {
                spec->magnitude[i][t] = (spec->magnitude[i][t] - min_val) / range;
            }
        }
    }
}

/* ============================================================================
 * ГЛАВНАЯ ФУНКЦИЯ
 * ============================================================================
 */
int main(void)
{
    adas_frame_t current_frame;
    char uart_buffer[64];
    uint8_t uart_index = 0;
    uint8_t uart_char;

    uint8_t continuous_stats = 0;
    uint8_t continuous_inference = 1;
    uint32_t last_stats_time = 0;
    uint32_t window_start_offset = 0;
    float* window_buffer = NULL;

    /* Выделение памяти под буфер окна */
    window_buffer = (float*)malloc(NUM_LEADS * WINDOW_SIZE_SAMPLES * sizeof(float));
    if (!window_buffer) {
        uart_send_string("ERROR: Failed to allocate window buffer!\n");
        while(1);
    }

    /* Отключить прерывания на время инициализации */
    asm("cli r0;");

    /* Инициализация системы */
    bf527_system_init();
    bf527_uart_init();
    bf527_gpio_control_init();
    bf527_spi_init();
    bf527_interrupt_init();

    /* Инициализация буферов и процессоров */
    adas_buffer_init(&ecg_buffer, frame_buffer, 512);
    ecg_processor_init(&ecg_processor);
    ring_buffer_init(&signal_ring);
    inference_init(&infer_state);
    decimator_init(&decimator);

    /* Инициализация модели */
    ecg_predict_initialize();

    /* Инициализация ADAS1000 */
    bf527_adas1000_init();

    /* Включение прерываний */
    asm("sti r0;");

    /* Вывод информации о конфигурации */
    uart_send_string("\n========================================\n");
    uart_send_string("SYSTEM CONFIGURATION\n");
    uart_send_string("========================================\n");
    char config_buf[128];
    sprintf(config_buf, "ADC sample rate: %d Hz\n", ADC_SAMPLE_RATE_HZ);
    uart_send_string(config_buf);
    sprintf(config_buf, "Target sample rate: %d Hz\n", TARGET_SAMPLE_RATE_HZ);
    uart_send_string(config_buf);
    sprintf(config_buf, "Decimation factor: %d\n", DECIMATION_FACTOR);
    uart_send_string(config_buf);
    sprintf(config_buf, "Window size: %d samples (%.1f sec)\n",
            WINDOW_SIZE_SAMPLES, (float)WINDOW_SIZE_SAMPLES / TARGET_SAMPLE_RATE_HZ);
    uart_send_string(config_buf);
    uart_send_string("========================================\n\n");

    /* Вывод меню управления */
    uart_send_string("\n========================================\n");
    uart_send_string("ECG SYSTEM READY\n");
    uart_send_string("========================================\n");
    uart_send_string("Commands:\n");
    uart_send_string("  s or S - Show statistics\n");
    uart_send_string("  c or C - Continuous statistics (toggle)\n");
    uart_send_string("  i or I - Continuous inference (toggle)\n");
    uart_send_string("  d or D - Dump ADAS1000 registers\n");
    uart_send_string("  l or L - Show lead status\n");
    uart_send_string("  r or R - Show real-time ECG\n");
    uart_send_string("  t or T - Show diagnosis thresholds\n");
    uart_send_string("  h or H - Show help\n");
    uart_send_string("========================================\n\n");

    /* Основной цикл */
    while (1) {
        /* === ОБРАБОТКА КОМАНД С UART === */
        if (*pUART0_LSR & 0x0001) {
            uart_char = (uint8_t)*pUART0_RBR;

            if (uart_char == '\r' || uart_char == '\n') {
                if (uart_index > 0) {
                    uart_buffer[uart_index] = '\0';

                    switch (uart_buffer[0]) {
                        case 's':
                        case 'S':
                            uart_export_stats();
                            break;
                        case 'c':
                        case 'C':
                            continuous_stats = !continuous_stats;
                            uart_send_string(continuous_stats ? "Continuous stats: ON\n" : "Continuous stats: OFF\n");
                            break;
                        case 'i':
                        case 'I':
                            continuous_inference = !continuous_inference;
                            uart_send_string(continuous_inference ? "Inference: ON\n" : "Inference: OFF\n");
                            break;
                        case 'r':
                        case 'R':
                            uart_send_string("Real-time ECG: data streaming...\n");
                            break;
                        case 'd':
                        case 'D':
                            dump_adas1000_registers();
                            break;
                        case 'l':
                        case 'L':
                            print_lead_status();
                            break;
                        case 't':
                        case 'T':
                            print_thresholds();
                            break;
                        case 'h':
                        case 'H':
                            uart_send_string("\nCommands:\n");
                            uart_send_string("  s/S - Statistics\n");
                            uart_send_string("  c/C - Continuous statistics (toggle)\n");
                            uart_send_string("  i/I - Continuous inference (toggle)\n");
                            uart_send_string("  r/R - Real-time ECG\n");
                            uart_send_string("  d/D - Register dump\n");
                            uart_send_string("  l/L - Lead status\n");
                            uart_send_string("  t/T - Show diagnosis thresholds\n");
                            uart_send_string("  h/H - Help\n");
                            break;
                        default:
                            uart_send_string("Unknown command. Press 'h' for help.\n");
                            break;
                    }
                    uart_index = 0;
                }
            } else if (uart_index < sizeof(uart_buffer) - 1) {
                uart_buffer[uart_index++] = uart_char;
                uart_send_byte(uart_char);
            }
        }

        /* === ОБРАБОТКА ДАННЫХ ЭКГ === */
        if (data_ready_flag) {
            data_ready_flag = 0;

            while (adas_buffer_get(&ecg_buffer, &current_frame) == 0) {
                ecg_process_frame(&current_frame, &ecg_processor);

                /* Непрерывная статистика */
                if (continuous_stats && (frame_counter - last_stats_time) >= 200) {
                    uart_export_stats();
                    last_stats_time = frame_counter;
                }

                /* Отправка данных на ПК */
                uart_export_frame(&current_frame);
            }

            /* Инференс (использует данные из кольцевого буфера после децимации) */
            if (continuous_inference) {
                uint32_t required_samples = window_start_offset + WINDOW_SIZE_SAMPLES;

                if (signal_ring.sample_count >= required_samples) {
                    if (ring_buffer_get_window(&signal_ring, window_buffer, window_start_offset)) {
                        inference_process_window(&infer_state, window_buffer, frame_counter);
                    }
                    window_start_offset += STRIDE_SAMPLES;
                }
            }
        }

        /* Режим энергосбережения */
        asm("idle;");
    }
}
