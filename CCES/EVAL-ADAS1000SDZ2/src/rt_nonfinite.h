/*
 * File: rt_nonfinite.h
 *
 * MATLAB Coder version            : 25.2
 * C/C++ source code generated on  : 19-May-2026 20:46:02
 */

#ifndef RT_NONFINITE_H
#define RT_NONFINITE_H

/* Include Files */
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Для Blackfin определяем недостающие макросы */
#ifndef INFINITY
#define INFINITY (1.0f/0.0f)
#endif

#ifndef NAN
#define NAN (0.0f/0.0f)
#endif

extern real_T rtInf;
extern real_T rtMinusInf;
extern real_T rtNaN;
extern real32_T rtInfF;
extern real32_T rtMinusInfF;
extern real32_T rtNaNF;

extern boolean_T rtIsInf(real_T value);
extern boolean_T rtIsInfF(real32_T value);
extern boolean_T rtIsNaN(real_T value);
extern boolean_T rtIsNaNF(real32_T value);

#ifdef __cplusplus
}
#endif
#endif /* RT_NONFINITE_H */
