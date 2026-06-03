/* rt_nonfinite.c - Минимальная версия */
#include "rt_nonfinite.h"

real_T rtNaN = 0.0;
real_T rtInf = 1.0e38;
real_T rtMinusInf = -1.0e38;
real32_T rtNaNF = 0.0F;
real32_T rtInfF = 1.0e38F;
real32_T rtMinusInfF = -1.0e38F;

boolean_T rtIsInf(real_T value) { (void)value; return 0; }
boolean_T rtIsInfF(real32_T value) { (void)value; return 0; }
boolean_T rtIsNaN(real_T value) { (void)value; return 0; }
boolean_T rtIsNaNF(real32_T value) { (void)value; return 0; }
