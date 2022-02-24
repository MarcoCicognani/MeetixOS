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

/* Implementation taken from https://github.com/SerenityOS/serenity/blob/master/Userland/Libraries/LibM/math.cpp*/

#include <Api/Common.h>
#include <Api/User.h>
#include <fenv.h>
#include <LibTC/Assertions.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Math.hh>
#include <math.h>

namespace Details {

template<usize>
constexpr double e_to_power();

template<>
constexpr double e_to_power<0>() {
    return 1;
}

template<usize exponent>
constexpr double e_to_power() {
    return M_E * e_to_power<exponent - 1>();
}

template<usize>
constexpr usize factorial();

template<>
constexpr usize factorial<0>() {
    return 1;
}

template<usize value>
constexpr usize factorial() {
    return value * factorial<value - 1>();
}

template<usize>
constexpr usize product_even();

template<>
constexpr usize product_even<2>() {
    return 2;
}

template<usize value>
constexpr usize product_even() {
    return value * product_even<value - 2>();
}

template<usize>
constexpr usize product_odd();

template<>
constexpr usize product_odd<1>() {
    return 1;
}

template<usize value>
constexpr usize product_odd() {
    return value * Details::product_odd<value - 2>();
}

enum class RoundingMode {
    ToZero = FE_TOWARDZERO,
    Up     = FE_UPWARD,
    Down   = FE_DOWNWARD,
    ToEven = FE_TONEAREST
};

template<typename T>
union FloatExtractor;

/* Assumes long double is 80 bits, which is true with GCC on Intel platforms */
template<>
union FloatExtractor<long double> {
    static int const                m_mantissa_bits = 64;
    static unsigned long long const m_mantissa_max  = ~0u;
    static int const                m_exponent_bias = 16383;
    static int const                m_exponent_bits = 15;
    static unsigned const           m_exponent_max  = 32767;
    struct {
        unsigned long long m_mantissa;
        unsigned           m_exponent : 15;
        unsigned           m_sign     : 1;
    };
    long double m_value;
};

template<>
union FloatExtractor<double> {
    static int const                m_mantissa_bits = 52;
    static unsigned long long const m_mantissa_max  = (1uLL << 52) - 1;
    static int const                m_exponent_bias = 1023;
    static int const                m_exponent_bits = 11;
    static unsigned const           m_exponent_max  = 2047;
    struct {
        unsigned long long m_mantissa : 52;
        unsigned           m_exponent : 11;
        unsigned           m_sign     : 1;
    };
    double m_value;
};

template<>
union FloatExtractor<float> {
    static int const      m_mantissa_bits = 23;
    static unsigned const m_mantissa_max  = (1 << 23) - 1;
    static int const      m_exponent_bias = 127;
    static int const      m_exponent_bits = 8;
    static unsigned const m_exponent_max  = 255;
    struct {
        unsigned long long m_mantissa : 23;
        unsigned           m_exponent : 8;
        unsigned           m_sign     : 1;
    };
    float m_value;
};

template<typename T>
static T to_integer(T x, Details::RoundingMode rounding_mode) {
    if ( !isfinite(x) )
        return x;

    using Extractor = FloatExtractor<T>;

    Extractor extractor;
    extractor.m_value = x;

    auto unbiased_exponent = extractor.m_exponent - Extractor::m_exponent_bias;

    bool has_half_fraction    = false;
    bool has_nonhalf_fraction = false;
    if ( unbiased_exponent < 0 ) {
        /* it was easier to special case [0..1) as it saves us from handling subnormals, under flows, etc */
        if ( unbiased_exponent == -1 )
            has_half_fraction = true;

        has_nonhalf_fraction = unbiased_exponent < -1 || extractor.m_mantissa != 0;
        extractor.m_mantissa = 0;
        extractor.m_exponent = 0;
    } else {
        if ( unbiased_exponent >= Extractor::m_mantissa_bits )
            return x;

        auto dead_bitcount = Extractor::m_mantissa_bits - unbiased_exponent;
        auto dead_mask     = (1ull << dead_bitcount) - 1;
        auto dead_bits     = extractor.m_mantissa & dead_mask;
        extractor.m_mantissa &= ~dead_mask;

        auto nonhalf_fraction_mask = dead_mask >> 1;
        has_nonhalf_fraction       = (dead_bits & nonhalf_fraction_mask) != 0;
        has_half_fraction          = (dead_bits & ~nonhalf_fraction_mask) != 0;
    }

    bool should_round = false;
    switch ( rounding_mode ) {
        case Details::RoundingMode::ToEven:
            should_round = has_half_fraction;
            break;
        case Details::RoundingMode::Up:
            if ( !extractor.m_sign )
                should_round = has_nonhalf_fraction || has_half_fraction;
            break;
        case Details::RoundingMode::Down:
            if ( extractor.m_sign )
                should_round = has_nonhalf_fraction || has_half_fraction;
            break;
        case Details::RoundingMode::ToZero:
            break;
    }

    if ( should_round ) {
        /* we could do this ourselves, but this saves us from manually handling overflow */
        if ( extractor.m_sign )
            extractor.m_value -= static_cast<T>(1.0);
        else
            extractor.m_value += static_cast<T>(1.0);
    }
    return extractor.m_value;
}

template<typename T>
static T nextafter(T x, bool up) {
    if ( !isfinite(x) )
        return x;

    using Extractor = FloatExtractor<T>;

    Extractor extractor;
    extractor.m_value = x;
    if ( x == 0 ) {
        if ( !extractor.m_sign ) {
            extractor.m_mantissa = 1;
            extractor.m_sign     = !up;
            return extractor.m_value;
        }
        if ( up ) {
            extractor.m_sign     = false;
            extractor.m_mantissa = 1;
            return extractor.m_value;
        }
        extractor.m_mantissa = 1;
        extractor.m_sign     = up != extractor.m_sign;
        return extractor.m_value;
    }
    if ( up != extractor.m_sign ) {
        extractor.m_mantissa++;
        if ( !extractor.m_mantissa ) {
            // no need to normalize the mantissa as we just hit a power
            // of two.
            extractor.m_exponent++;
            if ( extractor.m_exponent == Extractor::m_exponent_max ) {
                extractor.m_exponent = Extractor::m_exponent_max - 1;
                extractor.m_mantissa = Extractor::m_mantissa_max;
            }
        }
        return extractor.m_value;
    }

    if ( !extractor.m_mantissa ) {
        if ( extractor.m_exponent ) {
            extractor.m_exponent--;
            extractor.m_mantissa = Extractor::m_mantissa_max;
        } else
            extractor.m_value = 0;
        return extractor.m_value;
    }

    extractor.m_mantissa--;
    if ( extractor.m_mantissa != Extractor::m_mantissa_max )
        return extractor.m_value;
    if ( extractor.m_exponent ) {
        extractor.m_exponent--;
        /* normalize */
        extractor.m_mantissa <<= 1;
    } else {
        if ( extractor.m_sign ) {
            /* negative infinity */
            extractor.m_mantissa = 0;
            extractor.m_exponent = Extractor::m_exponent_max;
        }
    }
    return extractor.m_value;
}

template<typename T>
static int ilogb(T x) noexcept {
    if ( x == 0 )
        return FP_ILOGB0;
    if ( isnan(x) )
        return FP_ILOGNAN;
    if ( !isfinite(x) )
        return INT_MAX;

    using Extractor = FloatExtractor<T>;

    Extractor extractor;
    extractor.m_value = x;

    return static_cast<int>(extractor.m_exponent) - Extractor::m_exponent_bias;
}

template<typename T>
static T modf(T x, T* intpart) noexcept {
    auto integer_part = Details::to_integer(x, Details::RoundingMode::ToZero);
    *intpart          = integer_part;
    auto fraction     = x - integer_part;
    if ( signbit(fraction) != signbit(x) )
        fraction = -fraction;
    return fraction;
}

template<typename T>
static T scalbn(T x, int exponent) noexcept {
    if ( x == 0 || !isfinite(x) || isnan(x) || exponent == 0 )
        return x;

    using Extractor = FloatExtractor<T>;

    Extractor extractor;
    extractor.m_value = x;
    if ( extractor.m_exponent != 0 ) {
        extractor.m_exponent
            = TC::clamp((int)extractor.m_exponent + exponent, 0, static_cast<int>(Extractor::m_exponent_max));
        return extractor.m_value;
    }

    auto leading_mantissa_zeroes = extractor.m_mantissa == 0 ? 32 : TC::count_leading_zeroes(extractor.m_mantissa);
    auto shift                   = TC::min(static_cast<int>(leading_mantissa_zeroes), exponent);
    exponent                     = TC::max(exponent - shift, 0);

    extractor.m_exponent <<= shift;
    extractor.m_exponent = exponent + 1;

    return extractor.m_value;
}

template<typename T>
static T copysign(T x, T y) noexcept {
    using Extractor = FloatExtractor<T>;

    Extractor ex{};
    ex.m_value = x;

    Extractor ey{};
    ey.m_value = y;

    ex.m_sign = ey.m_sign;
    return ex.m_value;
}

template<typename T>
static T gamma(T x) noexcept {
    if ( isnan(x) )
        return static_cast<T>(NAN);

    if ( x == static_cast<T>(0.0) )
        return signbit(x) ? static_cast<T>(-INFINITY) : static_cast<T>(INFINITY);

    if ( x < static_cast<T>(0) && (rintl(x) == x || isinf(x)) )
        return static_cast<T>(NAN);

    if ( isinf(x) )
        return static_cast<T>(INFINITY);

    using Extractor = FloatExtractor<T>;

    /* these constants were obtained through use of WolframAlpha */
    constexpr long long max_integer_whose_factorial_fits
        = (Extractor::m_mantissa_bits == FloatExtractor<long double>::m_mantissa_bits
               ? 20
               : (Extractor::m_mantissa_bits == FloatExtractor<double>::m_mantissa_bits
                      ? 18
                      : (Extractor::m_mantissa_bits == FloatExtractor<float>::m_mantissa_bits ? 10 : 0)));
    static_assert(max_integer_whose_factorial_fits != 0,
                  "Details::gamma needs to be aware of the integer factorial that fits in this floating point type.");

    if ( static_cast<int>(x) == x && x <= max_integer_whose_factorial_fits + 1 ) {
        long long result = 1;
        for ( long long cursor = 2; cursor < static_cast<long long>(x); cursor++ )
            result *= cursor;

        return static_cast<T>(result);
    }

    /* stirling approximation */
    return sqrtl(2.0 * M_PIl / static_cast<long double>(x))
         * powl(static_cast<long double>(x) / M_El, static_cast<long double>(x));
}

} /* namespace Details */

extern "C" {

double fabs(double x) noexcept {
    asm("fabs" : "+t"(x));
    return x;
}

float fabsf(float x) noexcept {
    asm("fabs" : "+t"(x));
    return x;
}

long double fabsl(long double x) noexcept {
    asm("fabs" : "+t"(x));
    return x;
}

double fmod(double x, double y) noexcept {
    u16 fpu_status;
    do {
        asm("fprem\n"
            "fnstsw %%ax\n"
            : "+t"(x), "=a"(fpu_status)
            : "u"(y));
    } while ( fpu_status & 0x400 );
    return x;
}

float fmodf(float x, float y) noexcept {
    u16 fpu_status;
    do {
        asm("fprem\n"
            "fnstsw %%ax\n"
            : "+t"(x), "=a"(fpu_status)
            : "u"(y));
    } while ( fpu_status & 0x400 );
    return x;
}

long double fmodl(long double x, long double y) noexcept {
    u16 fpu_status;
    do {
        asm("fprem\n"
            "fnstsw %%ax\n"
            : "+t"(x), "=a"(fpu_status)
            : "u"(y));
    } while ( fpu_status & 0x400 );
    return x;
}

double fmax(double x, double y) noexcept {
    if ( isnan(x) )
        return y;
    if ( isnan(y) )
        return x;
    else
        return x > y ? x : y;
}

float fmaxf(float x, float y) noexcept {
    if ( isnan(x) )
        return y;
    if ( isnan(y) )
        return x;
    else
        return x > y ? x : y;
}

long double fmaxl(long double x, long double y) noexcept {
    if ( isnan(x) )
        return y;
    if ( isnan(y) )
        return x;
    else
        return x > y ? x : y;
}

double fmin(double x, double y) noexcept {
    if ( isnan(x) )
        return y;
    if ( isnan(y) )
        return x;
    else
        return x < y ? x : y;
}

float fminf(float x, float y) noexcept {
    if ( isnan(x) )
        return y;
    if ( isnan(y) )
        return x;
    else
        return x < y ? x : y;
}

long double fminl(long double x, long double y) noexcept {
    if ( isnan(x) )
        return y;
    if ( isnan(y) )
        return x;
    else
        return x < y ? x : y;
}

double remainder(double x, double y) noexcept {
    u16 fpu_status;
    do {
        asm("fprem1\n"
            "fnstsw %%ax\n"
            : "+t"(x), "=a"(fpu_status)
            : "u"(y));
    } while ( fpu_status & 0x400 );
    return x;
}

float remainderf(float x, float y) noexcept {
    u16 fpu_status;
    do {
        asm("fprem1\n"
            "fnstsw %%ax\n"
            : "+t"(x), "=a"(fpu_status)
            : "u"(y));
    } while ( fpu_status & 0x400 );
    return x;
}

long double remainderl(long double x, long double y) noexcept {
    u16 fpu_status;
    do {
        asm("fprem1\n"
            "fnstsw %%ax\n"
            : "+t"(x), "=a"(fpu_status)
            : "u"(y));
    } while ( fpu_status & 0x400 );
    return x;
}

double nan(const char* s) noexcept {
    return __builtin_nan(s);
}

float nanf(const char* s) noexcept {
    return __builtin_nanf(s);
}

long double nanl(const char* s) noexcept {
    return __builtin_nanl(s);
}

double exp(double exponent) noexcept {
    double res;
    asm("fldl2e\n"
        "fmulp\n"
        "fld1\n"
        "fld %%st(1)\n"
        "fprem\n"
        "f2xm1\n"
        "faddp\n"
        "fscale\n"
        "fstp %%st(1)"
        : "=t"(res)
        : "0"(exponent));
    return res;
}

float expf(float exponent) noexcept {
    float res;
    asm("fldl2e\n"
        "fmulp\n"
        "fld1\n"
        "fld %%st(1)\n"
        "fprem\n"
        "f2xm1\n"
        "faddp\n"
        "fscale\n"
        "fstp %%st(1)"
        : "=t"(res)
        : "0"(exponent));
    return res;
}

long double expl(long double exponent) noexcept {
    long double res;
    asm("fldl2e\n"
        "fmulp\n"
        "fld1\n"
        "fld %%st(1)\n"
        "fprem\n"
        "f2xm1\n"
        "faddp\n"
        "fscale\n"
        "fstp %%st(1)"
        : "=t"(res)
        : "0"(exponent));
    return res;
}

double exp2(double exponent) noexcept {
    double res;
    asm("fld1\n"
        "fld %%st(1)\n"
        "fprem\n"
        "f2xm1\n"
        "faddp\n"
        "fscale\n"
        "fstp %%st(1)"
        : "=t"(res)
        : "0"(exponent));
    return res;
}

float exp2f(float exponent) noexcept {
    float res;
    asm("fld1\n"
        "fld %%st(1)\n"
        "fprem\n"
        "f2xm1\n"
        "faddp\n"
        "fscale\n"
        "fstp %%st(1)"
        : "=t"(res)
        : "0"(exponent));
    return res;
}

long double exp2l(long double exponent) noexcept {
    long double res;
    asm("fld1\n"
        "fld %%st(1)\n"
        "fprem\n"
        "f2xm1\n"
        "faddp\n"
        "fscale\n"
        "fstp %%st(1)"
        : "=t"(res)
        : "0"(exponent));
    return res;
}

double expm1(double x) noexcept {
    return exp(x) - 1;
}

float expm1f(float x) noexcept {
    return expf(x) - 1;
}

long double expm1l(long double x) noexcept {
    return expl(x) - 1;
}

double log(double x) noexcept {
    double ret;
    asm("fldln2\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

float logf(float x) noexcept {
    float ret;
    asm("fldln2\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

long double logl(long double x) noexcept {
    long double ret;
    asm("fldln2\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

double log2(double x) noexcept {
    double ret;
    asm("fld1\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

float log2f(float x) noexcept {
    float ret;
    asm("fld1\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

long double log2l(long double x) noexcept {
    long double ret;
    asm("fld1\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

double log10(double x) noexcept {
    double ret;
    asm("fldlg2\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

float log10f(float x) noexcept {
    float ret;
    asm("fldlg2\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

long double log10l(long double x) noexcept {
    long double ret;
    asm("fldlg2\n"
        "fxch %%st(1)\n"
        "fyl2x\n"
        : "=t"(ret)
        : "0"(x));
    return ret;
}

double log1p(double x) noexcept {
    return log(1 + x);
}

float log1pf(float x) noexcept {
    return logf(1 + x);
}

long double log1pl(long double x) noexcept {
    return logl(1 + x);
}

double pow(double x, double y) noexcept {
    if ( isnan(y) )
        return y;
    if ( y == 0 )
        return 1;
    if ( x == 0 )
        return 0;
    if ( y == 1 )
        return x;

    auto y_as_int = static_cast<int>(y);
    if ( y == static_cast<double>(y_as_int) ) {
        auto result = x;
        for ( int i = 0; i < fabs(y) - 1; ++i )
            result *= x;

        if ( y < 0 )
            result = 1.0 / result;
        return result;
    }

    return exp2(y * log2(x));
}

float powf(float x, float y) noexcept {
    if ( isnan(y) )
        return y;
    if ( y == 0 )
        return 1;
    if ( x == 0 )
        return 0;
    if ( y == 1 )
        return x;

    auto y_as_int = static_cast<int>(y);
    if ( y == static_cast<float>(y_as_int) ) {
        auto result = x;
        for ( int i = 0; i < static_cast<int>(fabsf(y) - 1); ++i )
            result *= x;

        if ( y < 0 )
            result = 1.0f / result;
        return result;
    }

    return exp2f(y * log2f(x));
}

long double powl(long double x, long double y) noexcept {
    if ( isnan(y) )
        return y;
    if ( y == 0 )
        return 1;
    if ( x == 0 )
        return 0;
    if ( y == 1 )
        return x;

    auto y_as_int = static_cast<int>(y);
    if ( y == static_cast<long double>(y_as_int) ) {
        auto result = x;
        for ( int i = 0; i < static_cast<int>(fabsl(y) - 1); ++i )
            result *= x;

        if ( y < 0 )
            result = 1.0L / result;
        return result;
    }

    return exp2l(y * log2l(x));
}

double sqrt(double x) noexcept {
    double res;
    asm("fsqrt" : "=t"(res) : "0"(x));
    return res;
}

float sqrtf(float x) noexcept {
    float res;
    asm("fsqrt" : "=t"(res) : "0"(x));
    return res;
}

long double sqrtl(long double x) noexcept {
    long double res;
    asm("fsqrt" : "=t"(res) : "0"(x));
    return res;
}

double cbrt(double x) noexcept {
    if ( isinf(x) || x == 0 )
        return x;
    if ( x < 0 )
        return -cbrt(-x);

    double r  = x;
    double ex = 0;

    while ( r < 0.125 ) {
        r *= 8;
        ex--;
    }
    while ( r > 1.0 ) {
        r *= 0.125;
        ex++;
    }

    r = (-0.46946116 * r + 1.072302) * r + 0.3812513;

    while ( ex < 0 ) {
        r *= 0.5;
        ex++;
    }
    while ( ex > 0 ) {
        r *= 2.0;
        ex--;
    }

    r = (2.0 / 3.0) * r + (1.0 / 3.0) * x / (r * r);
    r = (2.0 / 3.0) * r + (1.0 / 3.0) * x / (r * r);
    r = (2.0 / 3.0) * r + (1.0 / 3.0) * x / (r * r);
    r = (2.0 / 3.0) * r + (1.0 / 3.0) * x / (r * r);
    return r;
}

float cbrtf(float x) noexcept {
    if ( isinf(x) || x == 0 )
        return x;
    if ( x < 0 )
        return -cbrtf(-x);

    float r  = x;
    float ex = 0;

    while ( r < 0.125f ) {
        r *= 8;
        ex--;
    }
    while ( r > 1.0f ) {
        r *= 0.125f;
        ex++;
    }

    r = (-0.46946116f * r + 1.072302f) * r + 0.3812513f;

    while ( ex < 0 ) {
        r *= 0.5f;
        ex++;
    }
    while ( ex > 0 ) {
        r *= 2.0f;
        ex--;
    }

    r = (2.0f / 3.0f) * r + (1.0f / 3.0f) * x / (r * r);
    r = (2.0f / 3.0f) * r + (1.0f / 3.0f) * x / (r * r);
    r = (2.0f / 3.0f) * r + (1.0f / 3.0f) * x / (r * r);
    r = (2.0f / 3.0f) * r + (1.0f / 3.0f) * x / (r * r);
    return r;
}

long double cbrtl(long double x) noexcept {
    if ( isinf(x) || x == 0 )
        return x;
    if ( x < 0 )
        return -cbrtl(-x);

    long double r  = x;
    long double ex = 0;

    while ( r < 0.125L ) {
        r *= 8;
        ex--;
    }
    while ( r > 1.0L ) {
        r *= 0.125L;
        ex++;
    }

    r = (-0.46946116L * r + 1.072302L) * r + 0.3812513L;

    while ( ex < 0 ) {
        r *= 0.5L;
        ex++;
    }
    while ( ex > 0 ) {
        r *= 2.0L;
        ex--;
    }

    r = (2.0L / 3.0L) * r + (1.0L / 3.0L) * x / (r * r);
    r = (2.0L / 3.0L) * r + (1.0L / 3.0L) * x / (r * r);
    r = (2.0L / 3.0L) * r + (1.0L / 3.0L) * x / (r * r);
    r = (2.0L / 3.0L) * r + (1.0L / 3.0L) * x / (r * r);
    return r;
}

double hypot(double x, double y) noexcept {
    return sqrt(x * x + y * y);
}

float hypotf(float x, float y) noexcept {
    return sqrtf(x * x + y * y);
}

long double hypotl(long double x, long double y) noexcept {
    return sqrtl(x * x + y * y);
}

double sin(double angle) noexcept {
    double ret;
    asm("fsin" : "=t"(ret) : "0"(angle));
    return ret;
}

float sinf(float angle) noexcept {
    float ret;
    asm("fsin" : "=t"(ret) : "0"(angle));
    return ret;
}

long double sinl(long double angle) noexcept {
    long double ret;
    asm("fsin" : "=t"(ret) : "0"(angle));
    return ret;
}

double cos(double angle) noexcept {
    double ret;
    asm("fcos" : "=t"(ret) : "0"(angle));
    return ret;
}

float cosf(float angle) noexcept {
    float ret;
    asm("fcos" : "=t"(ret) : "0"(angle));
    return ret;
}

long double cosl(long double angle) noexcept {
    long double ret;
    asm("fcos" : "=t"(ret) : "0"(angle));
    return ret;
}

double tan(double angle) noexcept {
    double ret;
    double one;
    asm("fptan" : "=t"(one), "=u"(ret) : "0"(angle));
    return ret;
}

float tanf(float angle) noexcept {
    float ret;
    float one;
    asm("fptan" : "=t"(one), "=u"(ret) : "0"(angle));
    return ret;
}

long double tanl(long double angle) noexcept {
    long double ret;
    long double one;
    asm("fptan" : "=t"(one), "=u"(ret) : "0"(angle));
    return ret;
}

double asin(double x) noexcept {
    if ( x > 1 || x < -1 )
        return nan("");
    if ( x > 0.5 || x < -0.5 )
        return 2 * atan(x / (1 + sqrt(1 - x * x)));

    auto squared = x * x;
    auto value   = x;
    auto i       = x * squared;
    value += i * Details::product_odd<1>() / Details::product_even<2>() / 3;
    i *= squared;
    value += i * Details::product_odd<3>() / Details::product_even<4>() / 5;
    i *= squared;
    value += i * Details::product_odd<5>() / Details::product_even<6>() / 7;
    i *= squared;
    value += i * Details::product_odd<7>() / Details::product_even<8>() / 9;
    i *= squared;
    value += i * Details::product_odd<9>() / Details::product_even<10>() / 11;
    i *= squared;
    value += i * Details::product_odd<11>() / Details::product_even<12>() / 13;
    i *= squared;
    value += i * Details::product_odd<13>() / Details::product_even<14>() / 15;
    i *= squared;
    value += i * Details::product_odd<15>() / Details::product_even<16>() / 17;
    return value;
}

float asinf(float x) noexcept {
    if ( x > 1 || x < -1 )
        return nanf("");
    if ( x > 0.5f || x < -0.5f )
        return 2 * atanf(x / (1 + sqrtf(1 - x * x)));

    auto squared = x * x;
    auto value   = x;
    auto i       = x * squared;
    value += i * Details::product_odd<1>() / Details::product_even<2>() / 3;
    i *= squared;
    value += i * Details::product_odd<3>() / Details::product_even<4>() / 5;
    i *= squared;
    value += i * Details::product_odd<5>() / Details::product_even<6>() / 7;
    i *= squared;
    value += i * Details::product_odd<7>() / Details::product_even<8>() / 9;
    i *= squared;
    value += i * Details::product_odd<9>() / Details::product_even<10>() / 11;
    i *= squared;
    value += i * Details::product_odd<11>() / Details::product_even<12>() / 13;
    i *= squared;
    value += i * Details::product_odd<13>() / Details::product_even<14>() / 15;
    i *= squared;
    value += i * Details::product_odd<15>() / Details::product_even<16>() / 17;
    return value;
}

long double asinl(long double x) noexcept {
    if ( x > 1 || x < -1 )
        return nanl("");
    if ( x > 0.5L || x < -0.5L )
        return 2 * atanl(x / (1 + sqrtl(1 - x * x)));

    auto squared = x * x;
    auto value   = x;
    auto i       = x * squared;
    value += i * Details::product_odd<1>() / Details::product_even<2>() / 3;
    i *= squared;
    value += i * Details::product_odd<3>() / Details::product_even<4>() / 5;
    i *= squared;
    value += i * Details::product_odd<5>() / Details::product_even<6>() / 7;
    i *= squared;
    value += i * Details::product_odd<7>() / Details::product_even<8>() / 9;
    i *= squared;
    value += i * Details::product_odd<9>() / Details::product_even<10>() / 11;
    i *= squared;
    value += i * Details::product_odd<11>() / Details::product_even<12>() / 13;
    i *= squared;
    value += i * Details::product_odd<13>() / Details::product_even<14>() / 15;
    i *= squared;
    value += i * Details::product_odd<15>() / Details::product_even<16>() / 17;
    return value;
}

double acos(double value) noexcept {
    return 0.5 * M_PI - asin(value);
}

float acosf(float value) noexcept {
    return 0.5f * M_PIf32 - asinf(value);
}

long double acosl(long double value) noexcept {
    return 0.5L * M_PIl - asinl(value);
}

double atan(double value) noexcept {
    double ret;
    asm("fld1\n"
        "fpatan\n"
        : "=t"(ret)
        : "0"(value));
    return ret;
}

float atanf(float value) noexcept {
    float ret;
    asm("fld1\n"
        "fpatan\n"
        : "=t"(ret)
        : "0"(value));
    return ret;
}

long double atanl(long double value) noexcept {
    long double ret;
    asm("fld1\n"
        "fpatan\n"
        : "=t"(ret)
        : "0"(value));
    return ret;
}

double atan2(double y, double x) noexcept {
    double ret;
    asm("fpatan" : "=t"(ret) : "0"(x), "u"(y) : "st(1)");
    return ret;
}

float atan2f(float y, float x) noexcept {
    float ret;
    asm("fpatan" : "=t"(ret) : "0"(x), "u"(y) : "st(1)");
    return ret;
}

long double atan2l(long double y, long double x) noexcept {
    long double ret;
    asm("fpatan" : "=t"(ret) : "0"(x), "u"(y) : "st(1)");
    return ret;
}

double sinh(double x) noexcept {
    auto exp_value = exp(x);
    if ( x > 0 )
        return (exp_value * exp_value - 1) / 2 / exp_value;
    else
        return (exp_value - 1 / exp_value) / 2;
}

float sinhf(float x) noexcept {
    auto exp_value = expf(x);
    if ( x > 0 )
        return (exp_value * exp_value - 1) / 2 / exp_value;
    else
        return (exp_value - 1 / exp_value) / 2;
}

long double sinhl(long double x) noexcept {
    auto exp_value = expl(x);
    if ( x > 0 )
        return (exp_value * exp_value - 1) / 2 / exp_value;
    else
        return (exp_value - 1 / exp_value) / 2;
}

double cosh(double x) noexcept {
    auto exp_value = exp(-x);
    if ( x < 0 )
        return (1 + exp_value * exp_value) / 2 / exp_value;
    else
        return (1 / exp_value + exp_value) / 2;
}

float coshf(float x) noexcept {
    auto exp_value = expf(-x);
    if ( x < 0 )
        return (1 + exp_value * exp_value) / 2 / exp_value;
    else
        return (1 / exp_value + exp_value) / 2;
}

long double coshl(long double x) noexcept {
    auto exponentiated = expl(-x);
    if ( x < 0 )
        return (1 + exponentiated * exponentiated) / 2 / exponentiated;
    else
        return (1 / exponentiated + exponentiated) / 2;
}

double tanh(double x) noexcept {
    if ( x > 0 ) {
        auto exp_value = exp(2 * x);
        return (exp_value - 1) / (exp_value + 1);
    } else {
        auto plus_x  = exp(x);
        auto minus_x = 1 / plus_x;
        return (plus_x - minus_x) / (plus_x + minus_x);
    }
}

float tanhf(float x) noexcept {
    if ( x > 0 ) {
        auto exp_value = expf(2 * x);
        return (exp_value - 1) / (exp_value + 1);
    } else {
        auto plus_x  = expf(x);
        auto minus_x = 1 / plus_x;
        return (plus_x - minus_x) / (plus_x + minus_x);
    }
}

long double tanhl(long double x) noexcept {
    if ( x > 0 ) {
        auto exp_value = expl(2 * x);
        return (exp_value - 1) / (exp_value + 1);
    } else {
        auto plus_x  = expl(x);
        auto minus_x = 1 / plus_x;
        return (plus_x - minus_x) / (plus_x + minus_x);
    }
}

double asinh(double x) noexcept {
    return log(x + sqrt(x * x + 1));
}

float asinhf(float x) noexcept {
    return logf(x + sqrtf(x * x + 1));
}

long double asinhl(long double x) noexcept {
    return logl(x + sqrtl(x * x + 1));
}

double acosh(double x) noexcept {
    return log(x + sqrt(x * x - 1));
}

float acoshf(float x) noexcept {
    return logf(x + sqrtf(x * x - 1));
}

long double acoshl(long double x) noexcept {
    return logl(x + sqrtl(x * x - 1));
}

double atanh(double x) noexcept {
    return log((1 + x) / (1 - x)) / 2.0;
}

float atanhf(float x) noexcept {
    return logf((1 + x) / (1 - x)) / 2.0f;
}

long double atanhl(long double x) noexcept {
    return logl((1 + x) / (1 - x)) / 2.0L;
}

double erf(double x) noexcept {
    auto t      = 1 / (1 + 0.47047 * fabs(x));
    auto poly   = t * (0.3480242 + t * (-0.958798 + t * 0.7478556));
    auto answer = 1 - poly * exp(-x * x);
    if ( x < 0 )
        return -answer;
    else
        return answer;
}

float erff(float x) noexcept {
    auto t      = 1 / (1 + 0.47047f * fabsf(x));
    auto poly   = t * (0.3480242f + t * (-0.958798f + t * 0.7478556f));
    auto answer = 1 - poly * expf(-x * x);
    if ( x < 0 )
        return -answer;
    else
        return answer;
}

long double erfl(long double x) noexcept {
    auto t      = 1 / (1 + 0.47047L * fabsl(x));
    auto poly   = t * (0.3480242L + t * (-0.958798L + t * 0.7478556L));
    auto answer = 1 - poly * expl(-x * x);
    if ( x < 0 )
        return -answer;
    else
        return answer;
}

double erfc(double x) noexcept {
    return 1 - erf(x);
}

float erfcf(float x) noexcept {
    return 1 - erff(x);
}

long double erfcl(long double x) noexcept {
    return 1 - erfl(x);
}

double gamma(double x) noexcept {
    return sqrt(2.0 * M_PI / x) * pow(x / M_E, x);
}

double tgamma(double value) noexcept {
    return Details::gamma(value);
}

float tgammaf(float value) noexcept {
    return Details::gamma(value);
}

long double tgammal(long double value) noexcept {
    return Details::gamma(value);
}

int signgam = 0;

double lgamma(double value) noexcept {
    return lgamma_r(value, &signgam);
}

float lgammaf(float value) noexcept {
    return lgammaf_r(value, &signgam);
}

long double lgammal(long double value) noexcept {
    return lgammal_r(value, &signgam);
}

double lgamma_r(double value, int* sign) noexcept {
    if ( value == 1.0 || value == 2.0 )
        return 0.0;
    if ( isinf(value) || value == 0.0 )
        return static_cast<double>(INFINITY);

    auto result = log(Details::gamma(value));
    *sign       = signbit(result) ? -1 : 1;
    return result;
}

float lgammaf_r(float value, int* sign) noexcept {
    if ( value == 1.0f || value == 2.0f )
        return 0.0f;
    if ( isinf(value) || value == 0.0f )
        return INFINITY;

    auto result = logf(Details::gamma(value));
    *sign       = signbit(result) ? -1 : 1;
    return result;
}

long double lgammal_r(long double value, int* sign) noexcept {
    if ( value == 1.0L || value == 2.0L )
        return 0.0L;
    if ( isinf(value) || value == 0.0L )
        return static_cast<long double>(INFINITY);

    auto result = logl(Details::gamma(value));
    *sign       = signbit(result) ? -1 : 1;
    return result;
}

double ceil(double value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::Up);
}

float ceilf(float value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::Up);
}

long double ceill(long double value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::Up);
}

double floor(double value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::Down);
}

float floorf(float value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::Down);
}

long double floorl(long double value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::Down);
}

double trunc(double x) noexcept {
    if ( fabs(x) < DOUBLE_MAX ) {
        u64 temp;
        asm("fisttpq %[temp]\n"
            "fildq %[temp]"
            : "+t"(x)
            : [temp] "m"(temp));
        return x;
    } else
        return Details::to_integer(x, Details::RoundingMode::ToZero);
}

float truncf(float x) noexcept {
    if ( fabsf(x) < static_cast<float>(MAXFLOAT) ) {
        u64 temp;
        asm("fisttpq %[temp]\n"
            "fildq %[temp]"
            : "+t"(x)
            : [temp] "m"(temp));
        return x;
    } else
        return Details::to_integer(x, Details::RoundingMode::ToZero);
}

long double truncl(long double x) noexcept {
    if ( fabsl(x) < LONG_LONG_MAX ) {
        u64 temp;
        asm("fisttpq %[temp]\n"
            "fildq %[temp]"
            : "+t"(x)
            : [temp] "m"(temp));
        return x;
    } else
        return Details::to_integer(x, Details::RoundingMode::ToZero);
}

double round(double value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::ToEven);
}

float roundf(float value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::ToEven);
}

long double roundl(long double value) noexcept {
    return Details::to_integer(value, Details::RoundingMode::ToEven);
}

long lround(double value) noexcept {
    return static_cast<long>(round(value));
}

long lroundf(float value) noexcept {
    return static_cast<long>(roundf(value));
}

long lroundl(long double value) noexcept {
    return static_cast<long>(roundl(value));
}

long long llround(double value) noexcept {
    return static_cast<long long>(round(value));
}

long long llroundf(float value) noexcept {
    return static_cast<long long>(roundf(value));
}

long long llroundl(long double value) noexcept {
    return static_cast<long long>(roundl(value));
}

long long llroundd(long double value) noexcept {
    return static_cast<long long>(roundl(value));
}

double nearbyint(double value) noexcept {
    return Details::to_integer(value, Details::RoundingMode{ fegetround() });
}

float nearbyintf(float value) noexcept {
    return Details::to_integer(value, Details::RoundingMode{ fegetround() });
}

long double nearbyintl(long double value) noexcept {
    return Details::to_integer(value, Details::RoundingMode{ fegetround() });
}

double rint(double value) noexcept {
    double res;
    asm("frndint" : "=t"(res) : "0"(value));
    return res;
}

float rintf(float value) noexcept {
    float res;
    asm("frndint" : "=t"(res) : "0"(value));
    return res;
}

long double rintl(long double value) noexcept {
    long double res;
    asm("frndint" : "=t"(res) : "0"(value));
    return res;
}

long lrint(double value) noexcept {
    long res;
    asm("fistpl %0" : "+m"(res) : "t"(value) : "st");
    return res;
}

long lrintf(float value) noexcept {
    long res;
    asm("fistpl %0" : "+m"(res) : "t"(value) : "st");
    return res;
}

long lrintl(long double value) noexcept {
    long res;
    asm("fistpl %0" : "+m"(res) : "t"(value) : "st");
    return res;
}

long long llrint(double value) noexcept {
    long long res;
    asm("fistpq %0" : "+m"(res) : "t"(value) : "st");
    return res;
}

long long llrintf(float value) noexcept {
    long long res;
    asm("fistpq %0" : "+m"(res) : "t"(value) : "st");
    return res;
}

long long llrintl(long double value) noexcept {
    long long res;
    asm("fistpq %0" : "+m"(res) : "t"(value) : "st");
    return res;
}

double frexp(double x, int* exp) noexcept {
    *exp = (x == 0) ? 0 : (1 + ilogb(x));
    return scalbn(x, -(*exp));
}

float frexpf(float x, int* exp) noexcept {
    *exp = (x == 0) ? 0 : (1 + ilogbf(x));
    return scalbnf(x, -(*exp));
}

long double frexpl(long double x, int* exp) noexcept {
    *exp = (x == 0) ? 0 : (1 + ilogbl(x));
    return scalbnl(x, -(*exp));
}

double ldexp(double x, int exp) noexcept {
    return Details::scalbn(x, exp);
}

float ldexpf(float x, int exp) noexcept {
    return Details::scalbn(x, exp);
}

long double ldexpl(long double x, int exp) noexcept {
    return Details::scalbn(x, exp);
}

double modf(double x, double* intpart) noexcept {
    return Details::modf(x, intpart);
}

float modff(float x, float* intpart) noexcept {
    return Details::modf(x, intpart);
}

long double modfl(long double x, long double* intpart) noexcept {
    return Details::modf(x, intpart);
}

double scalbn(double x, int exp) noexcept {
    return Details::scalbn(x, exp);
}

float scalbnf(float x, int exp) noexcept {
    return Details::scalbn(x, exp);
}

long double scalbnl(long double x, int exp) noexcept {
    return Details::scalbn(x, exp);
}

double scalbln(double x, long exp) noexcept {
    return Details::scalbn(x, exp);
}

float scalbnlf(float x, long exp) noexcept {
    return Details::scalbn(x, exp);
}

float scalblnf(float x, long exp) noexcept {
    return Details::scalbn(x, exp);
}

long double scalblnl(long double x, long exp) noexcept {
    return Details::scalbn(x, exp);
}

int ilogb(double x) noexcept {
    return Details::ilogb(x);
}

int ilogbf(float x) noexcept {
    return Details::ilogb(x);
}

int ilogbl(long double x) noexcept {
    return Details::ilogb(x);
}

double logb(double x) noexcept {
    return ilogb(x);
}

float logbf(float x) noexcept {
    return static_cast<float>(ilogbf(x));
}

long double logbl(long double x) noexcept {
    return ilogbl(x);
}

double nextafter(double x, double target) noexcept {
    if ( x == target )
        return target;
    else
        return Details::nextafter(x, target >= x);
}

float nextafterf(float x, float target) noexcept {
    if ( x == target )
        return target;
    else
        return Details::nextafter(x, target >= x);
}

long double nextafterl(long double x, long double target) noexcept {
    if ( x == target )
        return target;
    else
        return Details::nextafter(x, target >= x);
}

double nexttoward(double x, long double target) noexcept {
    if ( x == static_cast<double>(target) )
        return static_cast<double>(target);
    else
        return Details::nextafter(x, static_cast<double>(target) >= x);
}

float nexttowardf(float x, long double target) noexcept {
    if ( x == static_cast<float>(target) )
        return static_cast<float>(target);
    else
        return Details::nextafter(x, static_cast<float>(target) >= x);
}

long double nexttowardl(long double x, long double target) noexcept {
    if ( x == target )
        return target;
    else
        return Details::nextafter(x, target >= x);
}

double copysign(double x, double y) noexcept {
    return Details::copysign(x, y);
}

float copysignf(float x, float y) noexcept {
    return Details::copysign(x, y);
}

long double copysignl(long double x, long double y) noexcept {
    return Details::copysign(x, y);
}

double fdim(double, double) noexcept {
    __NOT_IMPLEMENTED(fdim);
    return 0.0;
}

float fdimf(float, float) noexcept {
    __NOT_IMPLEMENTED(fdimf);
    return 0.0f;
}

long double fdiml(long double, long double) noexcept {
    __NOT_IMPLEMENTED(fdiml);
    return 0.0L;
}

double fma(double, double, double) noexcept {
    __NOT_IMPLEMENTED(fma);
    return 0.0;
}

float fmaf(float, float, float) noexcept {
    __NOT_IMPLEMENTED(fmaf);
    return 0.0f;
}

long double fmal(long double, long double, long double) noexcept {
    __NOT_IMPLEMENTED(fmal);
    return 0.0L;
}

double remquo(double, double, int*) noexcept {
    __NOT_IMPLEMENTED(remquo);
    return 0.0;
}

float remquof(float, float, int*) noexcept {
    __NOT_IMPLEMENTED(remquof);
    return 0.0f;
}

long double remquol(long double, long double, int*) noexcept {
    __NOT_IMPLEMENTED(remquol);
    return 0.0L;
}
}