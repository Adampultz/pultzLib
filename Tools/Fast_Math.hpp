/*
 hpp file for allowing switching between neon fast math functions and regular C++ functions
Useful for running Bela
*/

#ifndef Fast_Math_hpp
#define Fast_Math_hpp

//#define Bela // Change this if you want to use the regular C++ functions

#ifdef Bela
#include <libraries/math_neon/math_neon.h>
#define fabsf_fast fabsf_neon
#define powf_fast powf_neon
#define sqrtf_fast sqrtf_neon
#define ceilf_fast ceilf_neon
#define expf_fast expf_neon
#define logf_fast logf_neon
#define logf_fast logf_neon
#define log10f_fast log10f_neon
#define floorf_fast floorf_neon
#else
#define fabsf_fast fabsf
#define powf_fast powf
#define sqrtf_fast sqrtf
#define ceilf_fast ceilf
#define expf_fast expf
#define logf_fast logf
#define logf_fast logf
#define log10f_fast log10f
#define floorf_fast floorf
#endif

#endif /* Header_h */


