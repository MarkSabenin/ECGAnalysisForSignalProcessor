/*
 * File: ecg_predict.h
 *
 * MATLAB Coder version            : 25.2
 * C/C++ source code generated on  : 19-May-2026 20:46:02
 */

#ifndef ECG_PREDICT_H
#define ECG_PREDICT_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void ecg_predict(const float dwt_input[120000],
                        const float stft_input[21672], unsigned char *class_id,
                        float probabilities[12]);

void ecg_predict_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ecg_predict.h
 *
 * [EOF]
 */
