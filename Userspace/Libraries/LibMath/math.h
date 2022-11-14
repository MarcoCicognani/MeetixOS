/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-deprecated-headers"
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <float.h>
#include <limits.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

CONST_VALUE(MATH_ERRNO, int, 1);
CONST_VALUE(MATH_ERREXCEPT, int, 2);
CONST_VALUE(math_errhandling, int, MATH_ERREXCEPT);

#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VAL  __builtin_huge_val()
#define HUGE_VALL __builtin_huge_vall()
#define INFINITY  __builtin_huge_valf()
#define NAN       __builtin_nan("")
#define MAXFLOAT  FLT_MAX

CONST_VALUE(M_E, double, 2.7182818284590452354);
CONST_VALUE(M_LOG2E, double, 1.4426950408889634074);
CONST_VALUE(M_LOG10E, double, 0.43429448190325182765);
CONST_VALUE(M_LN2, double, 0.69314718055994530942);
CONST_VALUE(M_LN10, double, 2.30258509299404568402);
CONST_VALUE(M_PI, double, 3.14159265358979323846);
CONST_VALUE(M_PI_2, double, 1.57079632679489661923);
CONST_VALUE(M_PI_4, double, 0.78539816339744830962);
CONST_VALUE(M_1_PI, double, 0.31830988618379067154);
CONST_VALUE(M_2_PI, double, 0.63661977236758134308);
CONST_VALUE(M_2_SQRTPI, double, 1.12837916709551257390);
CONST_VALUE(M_SQRT2, double, 1.41421356237309504880);
CONST_VALUE(M_SQRT1_2, double, 0.70710678118654752440);

CONST_VALUE(M_Ef32, float, 2.7182818284590452354f);
CONST_VALUE(M_LOG2Ef32, float, 1.4426950408889634074f);
CONST_VALUE(M_LOG10Ef32, float, 0.43429448190325182765f);
CONST_VALUE(M_LN2f32, float, 0.69314718055994530942f);
CONST_VALUE(M_LN10f32, float, 2.30258509299404568402f);
CONST_VALUE(M_PIf32, float, 3.14159265358979323846f);
CONST_VALUE(M_PI_2f32, float, 1.57079632679489661923f);
CONST_VALUE(M_PI_4f32, float, 0.78539816339744830962f);
CONST_VALUE(M_1_PIf32, float, 0.31830988618379067154f);
CONST_VALUE(M_2_PIf32, float, 0.63661977236758134308f);
CONST_VALUE(M_2_SQRTPIf32, float, 1.12837916709551257390f);
CONST_VALUE(M_SQRT2f32, float, 1.41421356237309504880f);
CONST_VALUE(M_SQRT1_2f32, float, 0.70710678118654752440f);

CONST_VALUE(M_El, long double, 2.718281828459045235360287471352662498L);
CONST_VALUE(M_LOG2El, long double, 1.442695040888963407359924681001892137L);
CONST_VALUE(M_LOG10El, long double, 0.434294481903251827651128918916605082L);
CONST_VALUE(M_LN2l, long double, 0.693147180559945309417232121458176568L);
CONST_VALUE(M_LN10l, long double, 2.302585092994045684017991454684364208L);
CONST_VALUE(M_PIl, long double, 3.141592653589793238462643383279502884L);
CONST_VALUE(M_PI_2l, long double, 1.570796326794896619231321691639751442L);
CONST_VALUE(M_PI_4l, long double, 0.785398163397448309615660845819875721L);
CONST_VALUE(M_1_PIl, long double, 0.318309886183790671537767526745028724L);
CONST_VALUE(M_2_PIl, long double, 0.636619772367581343075535053490057448L);
CONST_VALUE(M_2_SQRTPIl, long double, 1.128379167095512573896158903121545172L);
CONST_VALUE(M_SQRT2l, long double, 1.414213562373095048801688724209698079L);
CONST_VALUE(M_SQRT1_2l, long double, 0.707106781186547524400844362104849039L);

CONST_VALUE(FP_NAN, int, 0);
CONST_VALUE(FP_INFINITE, int, 1);
CONST_VALUE(FP_ZERO, int, 2);
CONST_VALUE(FP_SUBNORMAL, int, 3);
CONST_VALUE(FP_NORMAL, int, 4);
#define fpclassify(x) __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_ZERO, FP_SUBNORMAL, FP_ZERO, x)

#define signbit(x)           __builtin_signbit(x)
#define isnan(x)             __builtin_isnan(x)
#define isinf(x)             __builtin_isinf_sign(x)
#define isfinite(x)          __builtin_isfinite(x)
#define isnormal(x)          __builtin_isnormal(x)
#define isgreater(x, y)      __builtin_isgreater((x), (y))
#define isgreaterequal(x, y) __builtin_isgreaterequal((x), (y))
#define isless(x, y)         __builtin_isless((x), (y))
#define islessequal(x, y)    __builtin_islessequal((x), (y))
#define islessgreater(x, y)  __builtin_islessgreater((x), (y))
#define isunordered(x, y)    __builtin_isunordered((x), (y))

CONST_VALUE(DOUBLE_MAX, double, 0b0111111111101111111111111111111111111111111111111111111111111111);
CONST_VALUE(DOUBLE_MIN, double, 0b0000000000010000000000000000000000000000000000000000000000000000);

CONST_VALUE(FP_ILOGB0, int, INT_MIN);
CONST_VALUE(FP_ILOGNAN, int, INT_MAX);

#define FLT_EVAL_METHOD __FLT_EVAL_METHOD__

#if FLT_EVAL_METHOD == 0
TYPE_ALIAS(float_t, float);
TYPE_ALIAS(double_t, double);
#elif FLT_EVAL_METHOD == 1
TYPE_ALIAS(float_t, double);
TYPE_ALIAS(double_t, double);
#elif FLT_EVAL_METHOD == 2
TYPE_ALIAS(float_t, long double);
TYPE_ALIAS(double_t, long double);
#else
TYPE_ALIAS(float_t, float);
TYPE_ALIAS(double_t, double);
#endif

extern int signgam;

/* Basic floating point operations */
double      fabs(double);
float       fabsf(float);
long double fabsl(long double);

double      fmod(double, double);
float       fmodf(float, float);
long double fmodl(long double, long double);

double      fmax(double, double);
float       fmaxf(float, float);
long double fmaxl(long double, long double);

double      fmin(double, double);
float       fminf(float, float);
long double fminl(long double, long double);

double      remainder(double, double);
float       remainderf(float, float);
long double remainderl(long double, long double);

double      nan(const char*);
float       nanf(const char*);
long double nanl(const char*);

/* Exponential functions */
double      exp(double);
float       expf(float);
long double expl(long double);

double      exp2(double);
float       exp2f(float);
long double exp2l(long double);

double      expm1(double);
float       expm1f(float);
long double expm1l(long double);

double      log(double);
float       logf(float);
long double logl(long double);

double      log2(double);
float       log2f(float);
long double log2l(long double);

double      log10(double);
float       log10f(float);
long double log10l(long double);

double      log1p(double);
float       log1pf(float);
long double log1pl(long double);

/* Power functions */
double      pow(double, double);
float       powf(float, float);
long double powl(long double, long double);

double      sqrt(double);
float       sqrtf(float);
long double sqrtl(long double);

double      cbrt(double);
float       cbrtf(float);
long double cbrtl(long double);

double      hypot(double, double);
float       hypotf(float, float);
long double hypotl(long double, long double);

/* Trigonometric functions */
double      sin(double);
float       sinf(float);
long double sinl(long double);

double      cos(double);
float       cosf(float);
long double cosl(long double);

double      tan(double);
float       tanf(float);
long double tanl(long double);

double      asin(double);
float       asinf(float);
long double asinl(long double);

double      acos(double);
float       acosf(float);
long double acosl(long double);

double      atan(double);
float       atanf(float);
long double atanl(long double);

double      atan2(double, double);
float       atan2f(float, float);
long double atan2l(long double, long double);

/* Hyperbolic functions*/
double      sinh(double);
float       sinhf(float);
long double sinhl(long double);

double      cosh(double);
float       coshf(float);
long double coshl(long double);

double      tanh(double);
float       tanhf(float);
long double tanhl(long double);

double      asinh(double);
float       asinhf(float);
long double asinhl(long double);

double      acosh(double);
float       acoshf(float);
long double acoshl(long double);

double      atanh(double);
float       atanhf(float);
long double atanhl(long double);

/* Error and gamma functions */
double      erf(double);
float       erff(float);
long double erfl(long double);

double      erfc(double);
float       erfcf(float);
long double erfcl(long double);

double gamma(double);

double      tgamma(double);
float       tgammaf(float);
long double tgammal(long double);

double      lgamma(double);
float       lgammaf(float);
long double lgammal(long double);

double      lgamma_r(double, int*);
float       lgammaf_r(float, int*);
long double lgammal_r(long double, int*);

/* Nearest integer floating point operations */
double      ceil(double);
float       ceilf(float);
long double ceill(long double);

double      floor(double);
float       floorf(float);
long double floorl(long double);

double      trunc(double);
float       truncf(float);
long double truncl(long double);

double      round(double);
float       roundf(float);
long double roundl(long double);

long lround(double);
long lroundf(float);
long lroundl(long double);

long long llround(double);
long long llroundf(float);
long long llroundl(long double);
long long llroundd(long double);

double      nearbyint(double);
float       nearbyintf(float);
long double nearbyintl(long double);

double      rint(double);
float       rintf(float);
long double rintl(long double);

long lrint(double);
long lrintf(float);
long lrintl(long double);

long long llrint(double);
long long llrintf(float);
long long llrintl(long double);

/* Floating point manipulation functions */
double      frexp(double, int*);
float       frexpf(float, int*);
long double frexpl(long double, int*);

double      ldexp(double, int);
float       ldexpf(float, int);
long double ldexpl(long double, int);

double      modf(double, double*);
float       modff(float, float*);
long double modfl(long double, long double*);

double      scalbn(double, int);
float       scalbnf(float, int);
long double scalbnl(long double, int);

double      scalbln(double, long);
float       scalbnlf(float, long);
float       scalblnf(float, long);
long double scalblnl(long double, long);

int ilogb(double);
int ilogbf(float);
int ilogbl(long double);

double      logb(double);
float       logbf(float);
long double logbl(long double);

double      nextafter(double, double);
float       nextafterf(float, float);
long double nextafterl(long double, long double);

double      nexttoward(double, long double);
float       nexttowardf(float, long double);
long double nexttowardl(long double, long double);

double      copysign(double, double);
float       copysignf(float, float);
long double copysignl(long double, long double);

/* positive difference */
double      fdim(double, double);
float       fdimf(float, float);
long double fdiml(long double, long double);

/* floating-point multiply and add */
double      fma(double, double, double);
float       fmaf(float, float, float);
long double fmal(long double x, long double y, long double z);

/* remainder and part of quotient */
double      remquo(double, double, int*);
float       remquof(float, float, int*);
long double remquol(long double, long double, int*);

#ifdef __cplusplus
}
#endif

#pragma clang diagnostic pop
