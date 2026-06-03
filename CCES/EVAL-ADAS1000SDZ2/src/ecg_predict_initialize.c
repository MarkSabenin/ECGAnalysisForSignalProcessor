/*
 * File: ecg_predict_initialize.c
 *
 * MATLAB Coder version            : 25.2
 * C/C++ source code generated on  : 19-May-2026 20:46:02
 */

/* Include Files */
#include "ecg_predict_initialize.h"
#include "ecg_predict.h"
#include "ecg_predict_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void ecg_predict_initialize(void)
{
  ecg_predict_init();
  isInitialized_ecg_predict = true;
}

/*
 * File trailer for ecg_predict_initialize.c
 *
 * [EOF]
 */
