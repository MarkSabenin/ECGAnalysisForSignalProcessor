/******************************************************************************
 * stft_bf527.h - STFT implementation for Blackfin BF527
 ******************************************************************************/

#ifndef STFT_BF527_H
#define STFT_BF527_H

#include <stdint.h>
#include <math.h>

/* STFT Parameters */
#define STFT_NFFT           256
#define STFT_WINDOW_LEN     256
#define STFT_OVERLAP        128
#define STFT_OUT_H          129     /* NFFT/2 + 1 */
#define STFT_OUT_W          21      /* (2500 - 256) / (256-128) + 1 */
#define STFT_SAMPLE_RATE    500
#define STFT_WINDOW_SIZE    2500

/* Hamming window coefficients (256 values) */
extern const float stft_hamming_window[STFT_WINDOW_LEN];

/* Spectrogram structure */
typedef struct {
    float magnitude[STFT_OUT_H][STFT_OUT_W];
    uint32_t num_freq_bins;
    uint32_t num_time_windows;
} spectrogram_t;

/* Function prototypes */
void stft_compute(const float *signal, uint32_t signal_len, spectrogram_t *out);
void stft_to_multichannel(const float *signal_12leads[12], spectrogram_t *out_spectrograms[12]);
void stft_normalize(spectrogram_t *spec);

#endif /* STFT_BF527_H */
