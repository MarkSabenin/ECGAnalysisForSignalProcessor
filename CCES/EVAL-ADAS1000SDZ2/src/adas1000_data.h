/*******************************************************************************
 * adas1000_data.h
 * Data structures for ADAS1000 driver
 ******************************************************************************/

#ifndef ADAS1000_DATA_H
#define ADAS1000_DATA_H

#include <stdint.h>
#include <stdbool.h>

/* ==================== CONSTANTS ==================== */
#define ADAS_NUM_CHANNELS       5
#define ADAS_NUM_SLAVE_CHANNELS 5
#define ADAS_MAX_FRAME_SIZE     64

/* Frame rates (from documentation, Table 37) */
#define ADAS_FRAME_RATE_31_25HZ    3125
#define ADAS_FRAME_RATE_2KHZ       2000
#define ADAS_FRAME_RATE_16KHZ      16000
#define ADAS_FRAME_RATE_128KHZ     128000

/* ADAS1000 register addresses (from documentation, pages 61-78) */
#define ADAS1000_ECGCTL     0x01
#define ADAS1000_LOFFCTL    0x02
#define ADAS1000_RESPCTL    0x03
#define ADAS1000_PACECTL    0x04
#define ADAS1000_CMREFCTL   0x05
#define ADAS1000_GPIOCTL    0x06
#define ADAS1000_PACEAMPTH  0x07
#define ADAS1000_TESTTONE   0x08
#define ADAS1000_CALDAC     0x09
#define ADAS1000_FRMCTL     0x0A
#define ADAS1000_FILTCTL    0x0B
#define ADAS1000_LOFFUTH    0x0C
#define ADAS1000_LOFFLTH    0x0D
#define ADAS1000_PACEEDGETH 0x0E
#define ADAS1000_PACELVLTH  0x0F
#define ADAS1000_LADATA     0x11
#define ADAS1000_LLDATA     0x12
#define ADAS1000_RADATA     0x13
#define ADAS1000_V1DATA     0x14
#define ADAS1000_V2DATA     0x15
#define ADAS1000_PACEDATA   0x1A
#define ADAS1000_RESPMAG    0x1B
#define ADAS1000_RESPPH     0x1C
#define ADAS1000_LOFF       0x1D
#define ADAS1000_DCLEADSOFF 0x1E
#define ADAS1000_OPSTAT     0x1F
#define ADAS1000_EXTENDSW   0x20
#define ADAS1000_CALLA      0x21
#define ADAS1000_CALLL      0x22
#define ADAS1000_CALRA      0x23
#define ADAS1000_CALV1      0x24
#define ADAS1000_CALV2      0x25
#define ADAS1000_LOAMLA     0x31
#define ADAS1000_LOAMLL     0x32
#define ADAS1000_LOAMRA     0x33
#define ADAS1000_LOAMV1     0x34
#define ADAS1000_LOAMV2     0x35
#define ADAS1000_PACE1_DATA 0x3A
#define ADAS1000_PACE2_DATA 0x3B
#define ADAS1000_PACE3_DATA 0x3C
#define ADAS1000_FRAMES     0x40
#define ADAS1000_CRC        0x41

/* ==================== ECGCTL BITS (Table 28) ==================== */
#define ADAS_ECGCTL_LAEN        (1ul << 23)
#define ADAS_ECGCTL_LLEN        (1ul << 22)
#define ADAS_ECGCTL_RAEN        (1ul << 21)
#define ADAS_ECGCTL_V1EN        (1ul << 20)
#define ADAS_ECGCTL_V2EN        (1ul << 19)
#define ADAS_ECGCTL_CHCONFIG    (1ul << 10)
#define ADAS_ECGCTL_GAIN        (1ul << 8)
#define ADAS_ECGCTL_VREFBUF     (1ul << 7)
#define ADAS_ECGCTL_CLKEXT      (1ul << 6)
#define ADAS_ECGCTL_MASTER      (1ul << 5)
#define ADAS_ECGCTL_GANG        (1ul << 4)
#define ADAS_ECGCTL_HP          (1ul << 3)
#define ADAS_ECGCTL_CNVEN       (1ul << 2)
#define ADAS_ECGCTL_PWREN       (1ul << 1)
#define ADAS_ECGCTL_SWRST       (1ul << 0)

/* Gain settings */
#define ADAS_GAIN_1_4   0x00
#define ADAS_GAIN_2_1   0x01
#define ADAS_GAIN_2_8   0x02
#define ADAS_GAIN_4_2   0x03

/* ==================== FRMCTL BITS (Table 37) ==================== */
#define ADAS_FRMCTL_LEAD_I_DIS   (1ul << 23)
#define ADAS_FRMCTL_LEAD_II_DIS  (1ul << 22)
#define ADAS_FRMCTL_LEAD_III_DIS (1ul << 21)
#define ADAS_FRMCTL_V1_DIS       (1ul << 20)
#define ADAS_FRMCTL_V2_DIS       (1ul << 19)
#define ADAS_FRMCTL_PACE_DIS     (1ul << 14)
#define ADAS_FRMCTL_RESP_M_DIS   (1ul << 13)
#define ADAS_FRMCTL_RESP_P_DIS   (1ul << 12)
#define ADAS_FRMCTL_LOFF_DIS     (1ul << 11)
#define ADAS_FRMCTL_GPIO_DIS     (1ul << 10)
#define ADAS_FRMCTL_CRC_DIS      (1ul << 9)
#define ADAS_FRMCTL_SIGNED_EN    (1ul << 8)
#define ADAS_FRMCTL_AUTO_DIS     (1ul << 7)
#define ADAS_FRMCTL_RDY_RPT      (1ul << 6)
#define ADAS_FRMCTL_DATA_FMT     (1ul << 4)
#define ADAS_FRMCTL_SKIP_MASK    (0x03 << 2)
#define ADAS_FRMCTL_FRAME_RATE_MASK (0x03 << 0)

#define ADAS_FRAME_2KHZ      0x00
#define ADAS_FRAME_16KHZ     0x01
#define ADAS_FRAME_128KHZ    0x02
#define ADAS_FRAME_31_25HZ   0x03

/* ==================== FRAMES HEADER BITS (Table 54) ==================== */
#define ADAS_FRAMES_MARKER       (1ul << 31)
#define ADAS_FRAMES_READY_BIT    (1ul << 30)
#define ADAS_FRAMES_OVERFLOW_MASK (0x03 << 28)
#define ADAS_FRAMES_FAULT_BIT    (1ul << 27)
#define ADAS_FRAMES_PACE3_BIT    (1ul << 26)
#define ADAS_FRAMES_PACE2_BIT    (1ul << 25)
#define ADAS_FRAMES_PACE1_BIT    (1ul << 24)
#define ADAS_FRAMES_RESP_BIT     (1ul << 23)
#define ADAS_FRAMES_LOFF_BIT     (1ul << 22)
#define ADAS_FRAMES_DC_LOFF_BIT  (1ul << 21)
#define ADAS_FRAMES_ADC_OR_BIT   (1ul << 20)

/* ==================== ENUMS ==================== */
typedef enum {
    ADAS_CH_LA = 0,
    ADAS_CH_LL = 1,
    ADAS_CH_RA = 2,
    ADAS_CH_V1 = 3,
    ADAS_CH_V2 = 4
} adas_channel_t;

typedef enum {
    ADAS_CH_V3 = 0,
    ADAS_CH_V4 = 1,
    ADAS_CH_V5 = 2,
    ADAS_CH_V6 = 3,
    ADAS_CH_SPARE = 4
} adas_slave_channel_t;

typedef enum {
    ADAS_MODE_NORMAL = 0,
    ADAS_MODE_TEST_TONE,
    ADAS_MODE_CALIBRATION
} adas_mode_t;

typedef enum {
    ADAS_LEAD_ON = 0,
    ADAS_LEAD_OFF = 1
} adas_lead_state_t;

typedef enum {
    ADAS_QUALITY_GOOD = 100,
    ADAS_QUALITY_FAIR = 70,
    ADAS_QUALITY_POOR = 30,
    ADAS_QUALITY_BAD = 0
} adas_quality_t;

/* ==================== DATA STRUCTURES ==================== */
typedef struct {
    int32_t raw_value;
    float   scaled_value;
    float   filtered_value;
    uint32_t timestamp_us;
    uint8_t  channel_id;
    uint8_t  quality;
    uint8_t  reserved[2];
} adas_sample_t;

typedef struct {
    uint32_t header;
    uint8_t  ready;
    uint8_t  overflow;
    uint8_t  fault;
    uint8_t  reserved1;
    adas_sample_t channels[ADAS_NUM_CHANNELS];
    uint8_t  leads_off_status;
    uint8_t  dc_leads_off_status;
    uint8_t  reserved2[2];
    uint8_t  pace_detected[3];
    uint16_t pace_width[3];
    uint16_t pace_height[3];
    uint32_t resp_magnitude;
    uint32_t resp_phase;
    uint32_t crc;
    uint32_t frame_number;
    uint32_t system_time_ms;
} adas_frame_t;

typedef struct {
    uint32_t header;
    uint8_t  ready;
    uint8_t  overflow;
    uint8_t  fault;
    uint8_t  reserved1;
    adas_sample_t channels[ADAS_NUM_SLAVE_CHANNELS];
    uint8_t  leads_off_status;
    uint32_t crc;
    uint32_t frame_number;
    uint32_t system_time_ms;
} adas_slave_frame_t;

typedef struct {
    uint32_t frame_rate;
    uint8_t  data_format;
    uint8_t  gain_setting;
    float    vref;
    uint8_t  channels_enabled;
    uint8_t  high_performance;
    uint8_t  reserved[2];
} adas_config_t;

typedef struct {
    adas_frame_t *buffer;
    uint32_t size;
    volatile uint32_t head;
    volatile uint32_t tail;
    volatile uint32_t count;
} adas_buffer_t;

/* ==================== BUFFER FUNCTIONS ==================== */
static inline void adas_buffer_init(adas_buffer_t *fb, adas_frame_t *buffer, uint32_t size)
{
    fb->buffer = buffer;
    fb->size = size;
    fb->head = 0;
    fb->tail = 0;
    fb->count = 0;
}

static inline int adas_buffer_put(adas_buffer_t *fb, const adas_frame_t *frame)
{
    if (fb->count >= fb->size) return -1;
    fb->buffer[fb->head] = *frame;
    fb->head = (fb->head + 1) % fb->size;
    fb->count++;
    return 0;
}

static inline int adas_buffer_get(adas_buffer_t *fb, adas_frame_t *frame)
{
    if (fb->count == 0) return -1;
    *frame = fb->buffer[fb->tail];
    fb->tail = (fb->tail + 1) % fb->size;
    fb->count--;
    return 0;
}

static inline int adas_buffer_is_empty(const adas_buffer_t *fb)
{
    return (fb->count == 0);
}

static inline uint32_t adas_buffer_count(const adas_buffer_t *fb)
{
    return fb->count;
}

static inline void adas_buffer_clear(adas_buffer_t *fb)
{
    fb->head = 0;
    fb->tail = 0;
    fb->count = 0;
}

/* ==================== HELPER FUNCTIONS ==================== */
static inline float adas_raw_to_millivolts(int32_t raw_value, uint8_t gain_setting, float vref)
{
    const float gain_factors[] = {1.4f, 2.1f, 2.8f, 4.2f};
    float gain = gain_factors[gain_setting & 0x03];
    float lsb = (2.0f * vref) / (gain * 16777215.0f);
    return (float)raw_value * lsb * 1000.0f;
}

static inline int32_t adas_convert_24bit_to_int32(uint32_t raw_24bit)
{
    int32_t result = raw_24bit & 0x00FFFFFF;
    if (result & 0x00800000) {
        result |= 0xFF000000;
    }
    return result;
}

#endif /* ADAS1000_DATA_H */
