/*******************************************************************************
 * ecg_processing.h
 * ECG signal processing (filtering, R-peak detection)
 ******************************************************************************/

#ifndef ECG_PROCESSING_H
#define ECG_PROCESSING_H

#include "adas1000_data.h"

#define ECG_FILTER_BUFFER_SIZE  256
#define ECG_SAMPLE_RATE         2000

typedef struct {
    float buffer[ECG_FILTER_BUFFER_SIZE];
    uint32_t index;
    float prev_hp_output;
    float prev_lp_output;
} filter_state_t;

typedef struct {
    uint32_t r_peak_position;
    float    r_peak_amplitude;
    uint32_t rr_interval_ms;
    float    heart_rate_bpm;
    uint32_t timestamp_ms;
} qrs_detection_t;

typedef struct {
    float mean;
    float variance;
    float std_dev;
    float min;
    float max;
    float peak_to_peak;
    float rms;
    uint32_t samples;
} signal_stats_t;

typedef struct {
    filter_state_t filter_state[ADAS_NUM_CHANNELS];
    qrs_detection_t last_qrs;
    signal_stats_t stats[ADAS_NUM_CHANNELS];
    float gain_calibration[ADAS_NUM_CHANNELS];   /* Added for calibration */
    float offset_calibration[ADAS_NUM_CHANNELS]; /* Added for calibration */
    float last_sample[ADAS_NUM_CHANNELS];
    uint8_t last_quality[ADAS_NUM_CHANNELS];
    uint32_t heart_rate_counter;                 /* Added */
    uint32_t last_r_peak_frame;                  /* Added */
    uint32_t last_r_peak_time_ms;                /* Added */
    uint32_t total_frames_processed;
    float qrs_threshold;
    uint32_t refractory_period_frames;
} ecg_processor_t;

/* Function prototypes */
void ecg_processor_init(ecg_processor_t *proc);
void ecg_process_frame(adas_frame_t *frame, ecg_processor_t *proc);
void ecg_filter_channel(adas_sample_t *sample, filter_state_t *state);

#endif /* ECG_PROCESSING_H */
