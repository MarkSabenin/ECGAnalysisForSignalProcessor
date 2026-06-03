/*
 * File: minOrMax.c
 *
 * MATLAB Coder version            : 25.2
 * C/C++ source code generated on  : 19-May-2026 20:46:02
 */

/* Include Files */
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const float x[12]
 *                int *idx
 * Return Type  : float
 */
float maximum(const float x[12], int *idx)
{
  float ex;
  int b_idx;
  int b_k;
  if (!rtIsNaNF(x[0])) {
    b_idx = 1;
  } else {
    int k;
    bool exitg1;
    b_idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 13)) {
      if (!rtIsNaNF(x[k - 1])) {
        b_idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (b_idx == 0) {
    ex = x[0];
    *idx = 1;
  } else {
    ex = x[b_idx - 1];
    *idx = b_idx;
    b_idx++;
    for (b_k = b_idx; b_k < 13; b_k++) {
      float f;
      f = x[b_k - 1];
      if (ex < f) {
        ex = f;
        *idx = b_k;
      }
    }
  }
  return ex;
}

/*
 * File trailer for minOrMax.c
 *
 * [EOF]
 */
