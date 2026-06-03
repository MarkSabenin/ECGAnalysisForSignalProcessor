/*
 * File: rtGetInf.c
 *
 * MATLAB Coder version            : 25.2
 * C/C++ source code generated on  : 19-May-2026 20:46:02
 */

/*
 * Abstract:
 *       MATLAB for code generation function to initialize non-finite, Inf and
 * MinusInf
 */
/* Include Files */
#include "rtGetInf.h"
#include "rt_nonfinite.h"

/* Для Blackfin определяем INFINITY если не определён */
#ifndef INFINITY
#define INFINITY (1.0f/0.0f)
#endif

/*
 * Function: rtGetInf
 * ================================================================== Abstract:
 *  Initialize rtInf needed by the generated code.
 */
real_T rtGetInf(void)
{
  return rtInf;
}

/*
 * Function: rtGetInfF
 * ================================================================= Abstract:
 *  Initialize rtInfF needed by the generated code.
 */
real32_T rtGetInfF(void)
{
  return rtInfF;
}

/*
 * Function: rtGetMinusInf
 * ============================================================= Abstract:
 *  Initialize rtMinusInf needed by the generated code.
 */
real_T rtGetMinusInf(void)
{
  return rtMinusInf;
}

/*
 * Function: rtGetMinusInfF
 * ============================================================ Abstract:
 *  Initialize rtMinusInfF needed by the generated code.
 */
real32_T rtGetMinusInfF(void)
{
  return rtMinusInfF;
}

/*
 * File trailer for rtGetInf.c
 *
 * [EOF]
 */
