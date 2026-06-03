// dwt_bf527.h
#ifndef DWT_BF527_H
#define DWT_BF527_H

#include <stdint.h>
#include <math.h>

// Параметры DWT
#define DWT_LEVELS      5
#define DWT_COEFFS      8
#define DWT_OUT_H       125
#define DWT_OUT_W       120
#define DWT_WINDOW_LEN  2500
#define DWT_FS          500

// Коэффициенты вейвлета db4
// Предварительно рассчитаны в MATLAB
extern const float db4_dec_lo[8];
extern const float db4_dec_hi[8];

// Структура для хранения коэффициентов
typedef struct {
    float cA[DWT_LEVELS+1][DWT_WINDOW_LEN / 2];
    float cD[DWT_LEVELS+1][DWT_WINDOW_LEN / 2];
    uint32_t len[DWT_LEVELS+1];
} dwt_coeffs_t;

// Функции
void dwt_decompose(const float *signal, uint32_t len, dwt_coeffs_t *out);
void dwt_to_scalogram(const dwt_coeffs_t *coeffs, float *output, uint32_t h, uint32_t w, uint32_t ch);

#endif
