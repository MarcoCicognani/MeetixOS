/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <float.h>
#include <limits.h>
#include <stdint.h>

#if __cplusplus >= 201103L
#    define NOEXCEPT noexcept
#else
#    define NOEXCEPT
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MATH_ERRNO       1
#define MATH_ERREXCEPT   2
#define math_errhandling MATH_ERREXCEPT

#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VAL  __builtin_huge_val()
#define HUGE_VALL __builtin_huge_vall()
#define INFINITY  __builtin_huge_valf()
#define NAN       __builtin_nan("")
#define MAXFLOAT  FLT_MAX

#define M_E        2.7182818284590452354
#define M_LOG2E    1.4426950408889634074
#define M_LOG10E   0.43429448190325182765
#define M_LN2      0.69314718055994530942
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.78539816339744830962
#define M_1_PI     0.31830988618379067154
#define M_2_PI     0.63661977236758134308
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.70710678118654752440

#define M_Ef32        2.7182818284590452354f
#define M_LOG2Ef32    1.4426950408889634074f
#define M_LOG10Ef32   0.43429448190325182765f
#define M_LN2f32      0.69314718055994530942f
#define M_LN10f32     2.30258509299404568402f
#define M_PIf32       3.14159265358979323846f
#define M_PI_2f32     1.57079632679489661923f
#define M_PI_4f32     0.78539816339744830962f
#define M_1_PIf32     0.31830988618379067154f
#define M_2_PIf32     0.63661977236758134308f
#define M_2_SQRTPIf32 1.12837916709551257390f
#define M_SQRT2f32    1.41421356237309504880f
#define M_SQRT1_2f32  0.70710678118654752440f

#define M_El        2.718281828459045235360287471352662498L
#define M_LOG2El    1.442695040888963407359924681001892137L
#define M_LOG10El   0.434294481903251827651128918916605082L
#define M_LN2l      0.693147180559945309417232121458176568L
#define M_LN10l     2.302585092994045684017991454684364208L
#define M_PIl       3.141592653589793238462643383279502884L
#define M_PI_2l     1.570796326794896619231321691639751442L
#define M_PI_4l     0.785398163397448309615660845819875721L
#define M_1_PIl     0.318309886183790671537767526745028724L
#define M_2_PIl     0.636619772367581343075535053490057448L
#define M_2_SQRTPIl 1.128379167095512573896158903121545172L
#define M_SQRT2l    1.414213562373095048801688724209698079L
#define M_SQRT1_2l  0.707106781186547524400844362104849039L

#define FP_NAN        0
#define FP_INFINITE   1
#define FP_ZERO       2
#define FP_SUBNORMAL  3
#define FP_NORMAL     4
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

#define DOUBLE_MAX ((double)0b0111111111101111111111111111111111111111111111111111111111111111)
#define DOUBLE_MIN ((double)0b0000000000010000000000000000000000000000000000000000000000000000)

#define FP_ILOGB0  INT_MIN
#define FP_ILOGNAN INT_MAX

#define FLT_EVAL_METHOD __FLT_EVAL_METHOD__

#if FLT_EVAL_METHOD == 0
typedef float  float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 2
typedef long double float_t;
typedef long double double_t;
#else
typedef float  float_t;
typedef double double_t;
#endif

extern i32 signgam;

/* Basic floating point operations */
double      fabs(double) NOEXCEPT;
float       fabsf(float) NOEXCEPT;
long double fabsl(long double) NOEXCEPT;

double      fmod(double, double) NOEXCEPT;
float       fmodf(float, float) NOEXCEPT;
long double fmodl(long double, long double) NOEXCEPT;

double      fmax(double, double) NOEXCEPT;
float       fmaxf(float, float) NOEXCEPT;
long double fmaxl(long double, long double) NOEXCEPT;

double      fmin(double, double) NOEXCEPT;
float       fminf(float, float) NOEXCEPT;
long double fminl(long double, long double) NOEXCEPT;

double      remainder(double, double) NOEXCEPT;
float       remainderf(float, float) NOEXCEPT;
long double remainderl(long double, long double) NOEXCEPT;

double      nan(const char*) NOEXCEPT;
float       nanf(const char*) NOEXCEPT;
long double nanl(const char*) NOEXCEPT;

/* Exponential functions */
double      exp(double) NOEXCEPT;
float       expf(float) NOEXCEPT;
long double expl(long double) NOEXCEPT;

double      exp2(double) NOEXCEPT;
float       exp2f(float) NOEXCEPT;
long double exp2l(long double) NOEXCEPT;

double      expm1(double) NOEXCEPT;
float       expm1f(float) NOEXCEPT;
long double expm1l(long double) NOEXCEPT;

double      log(double) NOEXCEPT;
float       logf(float) NOEXCEPT;
long double logl(long double) NOEXCEPT;

double      log2(double) NOEXCEPT;
float       log2f(float) NOEXCEPT;
long double log2l(long double) NOEXCEPT;

double      log10(double) NOEXCEPT;
float       log10f(float) NOEXCEPT;
long double log10l(long double) NOEXCEPT;

double      log1p(double) NOEXCEPT;
float       log1pf(float) NOEXCEPT;
long double log1pl(long double) NOEXCEPT;

/* Power functions */
double      pow(double, double) NOEXCEPT;
float       powf(float, float) NOEXCEPT;
long double powl(long double, long double) NOEXCEPT;

double      sqrt(double) NOEXCEPT;
float       sqrtf(float) NOEXCEPT;
long double sqrtl(long double) NOEXCEPT;

double      cbrt(double) NOEXCEPT;
float       cbrtf(float) NOEXCEPT;
long double cbrtl(long double) NOEXCEPT;

double      hypot(double, double) NOEXCEPT;
float       hypotf(float, float) NOEXCEPT;
long double hypotl(long double, long double) NOEXCEPT;

/* Trigonometric functions */
double      sin(double) NOEXCEPT;
float       sinf(float) NOEXCEPT;
long double sinl(long double) NOEXCEPT;

double      cos(double) NOEXCEPT;
float       cosf(float) NOEXCEPT;
long double cosl(long double) NOEXCEPT;

double      tan(double) NOEXCEPT;
float       tanf(float) NOEXCEPT;
long double tanl(long double) NOEXCEPT;

double      asin(double) NOEXCEPT;
float       asinf(float) NOEXCEPT;
long double asinl(long double) NOEXCEPT;

double      acos(double) NOEXCEPT;
float       acosf(float) NOEXCEPT;
long double acosl(long double) NOEXCEPT;

double      atan(double) NOEXCEPT;
float       atanf(float) NOEXCEPT;
long double atanl(long double) NOEXCEPT;

double      atan2(double, double) NOEXCEPT;
float       atan2f(float, float) NOEXCEPT;
long double atan2l(long double, long double) NOEXCEPT;

/* Hyperbolic functions*/
double      sinh(double) NOEXCEPT;
float       sinhf(float) NOEXCEPT;
long double sinhl(long double) NOEXCEPT;

double      cosh(double) NOEXCEPT;
float       coshf(float) NOEXCEPT;
long double coshl(long double) NOEXCEPT;

double      tanh(double) NOEXCEPT;
float       tanhf(float) NOEXCEPT;
long double tanhl(long double) NOEXCEPT;

double      asinh(double) NOEXCEPT;
float       asinhf(float) NOEXCEPT;
long double asinhl(long double) NOEXCEPT;

double      acosh(double) NOEXCEPT;
float       acoshf(float) NOEXCEPT;
long double acoshl(long double) NOEXCEPT;

double      atanh(double) NOEXCEPT;
float       atanhf(float) NOEXCEPT;
long double atanhl(long double) NOEXCEPT;

/* Error and gamma functions */
double      erf(double) NOEXCEPT;
float       erff(float) NOEXCEPT;
long double erfl(long double) NOEXCEPT;

double      erfc(double) NOEXCEPT;
float       erfcf(float) NOEXCEPT;
long double erfcl(long double) NOEXCEPT;

double gamma(double) NOEXCEPT;

double      tgamma(double) NOEXCEPT;
float       tgammaf(float) NOEXCEPT;
long double tgammal(long double) NOEXCEPT;

double      lgamma(double) NOEXCEPT;
float       lgammaf(float) NOEXCEPT;
long double lgammal(long double) NOEXCEPT;

double      lgamma_r(double, i32*) NOEXCEPT;
float       lgammaf_r(float, i32*) NOEXCEPT;
long double lgammal_r(long double, i32*) NOEXCEPT;

/* Nearest integer floating point operations */
double      ceil(double) NOEXCEPT;
float       ceilf(float) NOEXCEPT;
long double ceill(long double) NOEXCEPT;

double      floor(double) NOEXCEPT;
float       floorf(float) NOEXCEPT;
long double floorl(long double) NOEXCEPT;

double      trunc(double) NOEXCEPT;
float       truncf(float) NOEXCEPT;
long double truncl(long double) NOEXCEPT;

double      round(double) NOEXCEPT;
float       roundf(float) NOEXCEPT;
long double roundl(long double) NOEXCEPT;

long lround(double) NOEXCEPT;
long lroundf(float) NOEXCEPT;
long lroundl(long double) NOEXCEPT;

long long llround(double) NOEXCEPT;
long long llroundf(float) NOEXCEPT;
long long llroundl(long double) NOEXCEPT;
long long llroundd(long double) NOEXCEPT;

double      nearbyint(double) NOEXCEPT;
float       nearbyintf(float) NOEXCEPT;
long double nearbyintl(long double) NOEXCEPT;

double      rint(double) NOEXCEPT;
float       rintf(float) NOEXCEPT;
long double rintl(long double) NOEXCEPT;

long lrint(double) NOEXCEPT;
long lrintf(float) NOEXCEPT;
long lrintl(long double) NOEXCEPT;

long long llrint(double) NOEXCEPT;
long long llrintf(float) NOEXCEPT;
long long llrintl(long double) NOEXCEPT;

/* Floating point manipulation functions */
double      frexp(double, i32*) NOEXCEPT;
float       frexpf(float, i32*) NOEXCEPT;
long double frexpl(long double, i32*) NOEXCEPT;

double      ldexp(double, i32) NOEXCEPT;
float       ldexpf(float, i32) NOEXCEPT;
long double ldexpl(long double, i32) NOEXCEPT;

double      modf(double, double*) NOEXCEPT;
float       modff(float, float*) NOEXCEPT;
long double modfl(long double, long double*) NOEXCEPT;

double      scalbn(double, i32) NOEXCEPT;
float       scalbnf(float, i32) NOEXCEPT;
long double scalbnl(long double, i32) NOEXCEPT;

double      scalbln(double, long) NOEXCEPT;
float       scalbnlf(float, long) NOEXCEPT;
float       scalblnf(float, long) NOEXCEPT;
long double scalblnl(long double, long) NOEXCEPT;

i32 ilogb(double) NOEXCEPT;
i32 ilogbf(float) NOEXCEPT;
i32 ilogbl(long double) NOEXCEPT;

double      logb(double) NOEXCEPT;
float       logbf(float) NOEXCEPT;
long double logbl(long double) NOEXCEPT;

double      nextafter(double, double) NOEXCEPT;
float       nextafterf(float, float) NOEXCEPT;
long double nextafterl(long double, long double) NOEXCEPT;

double      nexttoward(double, long double) NOEXCEPT;
float       nexttowardf(float, long double) NOEXCEPT;
long double nexttowardl(long double, long double) NOEXCEPT;

double      copysign(double, double) NOEXCEPT;
float       copysignf(float, float) NOEXCEPT;
long double copysignl(long double, long double) NOEXCEPT;

/* positive difference */
double      fdim(double, double) NOEXCEPT;
float       fdimf(float, float) NOEXCEPT;
long double fdiml(long double, long double) NOEXCEPT;

/* floating-point multiply and add */
double      fma(double, double, double) NOEXCEPT;
float       fmaf(float, float, float) NOEXCEPT;
long double fmal(long double x, long double y, long double z) NOEXCEPT;

/* remainder and part of quotient */
double      remquo(double, double, i32*) NOEXCEPT;
float       remquof(float, float, i32*) NOEXCEPT;
long double remquol(long double, long double, i32*) NOEXCEPT;

#ifdef __cplusplus
}
#endif