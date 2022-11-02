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

#include <LibTC/Forward.hh>

/* clang-format off */

template<typename T>
struct NumericLimits final {
    /* Empty Body */
};
template<>
struct NumericLimits<bool> final {
    static constexpr auto min() -> bool       { return false; }
    static constexpr auto max() -> bool       { return true; }
    static constexpr auto is_signed() -> bool { return false; }
};
template<>
struct NumericLimits<char> final {
    static constexpr auto min() -> char       { return -__SCHAR_MAX__ - 1; }
    static constexpr auto max() -> char       { return __SCHAR_MAX__; }
    static constexpr auto is_signed() -> bool { return true; }
};
template<>
struct NumericLimits<signed char> final {
    static constexpr auto min() -> signed char { return -__SCHAR_MAX__ - 1; }
    static constexpr auto max() -> signed char { return __SCHAR_MAX__; }
    static constexpr auto is_signed() -> bool  { return true; }
};
template<>
struct NumericLimits<signed short> final {
    static constexpr auto min() -> signed short { return -__SHRT_MAX__ - 1; }
    static constexpr auto max() -> signed short { return __SHRT_MAX__; }
    static constexpr auto is_signed() -> bool   { return true; }
};
template<>
struct NumericLimits<signed int> final {
    static constexpr auto min() -> signed int { return -__INT_MAX__ - 1; }
    static constexpr auto max() -> signed int { return __INT_MAX__; }
    static constexpr auto is_signed() -> bool { return true; }
};
template<>
struct NumericLimits<signed long> final {
    static constexpr auto min() -> signed long { return -__LONG_MAX__ - 1; }
    static constexpr auto max() -> signed long { return __LONG_MAX__; }
    static constexpr auto is_signed() -> bool  { return true; }
};
template<>
struct NumericLimits<signed long long> final {
    static constexpr auto min() -> signed long long { return -__LONG_MAX__ - 1; }
    static constexpr auto max() -> signed long long { return __LONG_MAX__; }
    static constexpr auto is_signed() -> bool       { return true; }
};
template<>
struct NumericLimits<unsigned char> final {
    static constexpr auto min() -> unsigned char { return 0; }
    static constexpr auto max() -> unsigned char { return __SCHAR_MAX__ * 2u + 1; }
    static constexpr auto is_signed() -> bool    { return false; }
};
template<>
struct NumericLimits<unsigned short> final {
    static constexpr auto min() -> unsigned short { return 0; }
    static constexpr auto max() -> unsigned short { return __SHRT_MAX__ * 2u + 1; }
    static constexpr auto is_signed() -> bool     { return false; }
};
template<>
struct NumericLimits<unsigned int> final {
    static constexpr auto min() -> unsigned int { return 0; }
    static constexpr auto max() -> unsigned int { return __INT_MAX__ * 2u + 1; }
    static constexpr auto is_signed() -> bool   { return false; }
};
template<>
struct NumericLimits<unsigned long> final {
    static constexpr auto min() -> unsigned long { return 0; }
    static constexpr auto max() -> unsigned long { return __LONG_MAX__ * 2uL + 1; }
    static constexpr auto is_signed() -> bool    { return false; }
};
template<>
struct NumericLimits<unsigned long long> final {
    static constexpr auto min() -> unsigned long long { return 0; }
    static constexpr auto max() -> unsigned long long { return __LONG_LONG_MAX__ * 2uLL + 1; }
    static constexpr auto is_signed() -> bool         { return false; }
};

#ifndef IN_KERNEL

template<>
struct NumericLimits<float> final {
    static constexpr auto lowest() -> float   { return -__FLT_MAX__; }
    static constexpr auto min() -> float      { return __FLT_MIN__; }
    static constexpr auto max() -> float      { return __FLT_MAX__; }
    static constexpr auto epsilon() -> float  { return __FLT_EPSILON__; }
    static constexpr auto is_signed() -> bool { return true; }
};
template<>
struct NumericLimits<double> final {
    static constexpr auto lowest() -> double  { return -__DBL_MAX__; }
    static constexpr auto min() -> double     { return __DBL_MIN__; }
    static constexpr auto max() -> double     { return __DBL_MAX__; }
    static constexpr auto epsilon() -> double { return __DBL_EPSILON__; }
    static constexpr auto is_signed() -> bool { return true; }
};

template<>
struct NumericLimits<long double> final {
    static constexpr auto lowest() -> long double  { return -__LDBL_MAX__; }
    static constexpr auto min() -> long double     { return __LDBL_MIN__; }
    static constexpr auto max() -> long double     { return __LDBL_MAX__; }
    static constexpr auto epsilon() -> long double { return __LDBL_EPSILON__; }
    static constexpr auto is_signed() -> bool      { return true; }
};

#endif

/* clang-format on */
