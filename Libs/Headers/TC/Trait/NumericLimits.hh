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

namespace TC {
namespace Trait {

template<typename T>
struct NumericLimits {
    /* Empty Body */
};

/* clang-format off */

template<>
struct NumericLimits<bool> {
    static constexpr bool min()       { return false; }
    static constexpr bool max()       { return true; }
    static constexpr bool is_signed() { return false; }
};

template<>
struct NumericLimits<char> {
    static constexpr char min()       { return -__SCHAR_MAX__ - 1; }
    static constexpr char max()       { return __SCHAR_MAX__; }
    static constexpr bool is_signed() { return true; }
};

template<>
struct NumericLimits<signed char> {
    static constexpr signed char min()       { return -__SCHAR_MAX__ - 1; }
    static constexpr signed char max()       { return __SCHAR_MAX__; }
    static constexpr bool        is_signed() { return true; }
};

template<>
struct NumericLimits<signed short> {
    static constexpr signed short min()       { return -__SHRT_MAX__ - 1; }
    static constexpr signed short max()       { return __SHRT_MAX__; }
    static constexpr bool         is_signed() { return true; }
};

template<>
struct NumericLimits<signed int> {
    static constexpr signed int min()       { return -__INT_MAX__ - 1; }
    static constexpr signed int max()       { return __INT_MAX__; }
    static constexpr bool       is_signed() { return true; }
};

template<>
struct NumericLimits<signed long> {
    static constexpr signed long min()       { return -__LONG_MAX__ - 1; }
    static constexpr signed long max()       { return __LONG_MAX__; }
    static constexpr bool        is_signed() { return true; }
};

template<>
struct NumericLimits<signed long long> {
    static constexpr signed long long min()       { return -__LONG_MAX__ - 1; }
    static constexpr signed long long max()       { return __LONG_MAX__; }
    static constexpr bool             is_signed() { return true; }
};

template<>
struct NumericLimits<unsigned char> {
    static constexpr unsigned char min()       { return 0; }
    static constexpr unsigned char max()       { return __SCHAR_MAX__ * 2u + 1; }
    static constexpr bool          is_signed() { return false; }
};

template<>
struct NumericLimits<unsigned short> {
    static constexpr unsigned short min()       { return 0; }
    static constexpr unsigned short max()       { return __SHRT_MAX__ * 2u + 1; }
    static constexpr bool           is_signed() { return false; }
};

template<>
struct NumericLimits<unsigned int> {
    static constexpr unsigned int min()       { return 0; }
    static constexpr unsigned int max()       { return __INT_MAX__ * 2u + 1; }
    static constexpr bool         is_signed() { return false; }
};

template<>
struct NumericLimits<unsigned long> {
    static constexpr unsigned long min()       { return 0; }
    static constexpr unsigned long max()       { return __LONG_MAX__ * 2uL + 1; }
    static constexpr bool          is_signed() { return false; }
};

template<>
struct NumericLimits<unsigned long long> {
    static constexpr unsigned long long min()       { return 0; }
    static constexpr unsigned long long max()       { return __LONG_LONG_MAX__ * 2uLL + 1; }
    static constexpr bool               is_signed() { return false; }
};

#ifndef IN_KERNEL

template<>
struct NumericLimits<float> {
    static constexpr float lowest()    { return -__FLT_MAX__; }
    static constexpr float min()       { return __FLT_MIN__; }
    static constexpr float max()       { return __FLT_MAX__; }
    static constexpr float epsilon()   { return __FLT_EPSILON__; }
    static constexpr bool  is_signed() { return true; }
};

template<>
struct NumericLimits<double> {
    static constexpr double lowest()   { return -__DBL_MAX__; }
    static constexpr double min()      { return __DBL_MIN__; }
    static constexpr double max()      { return __DBL_MAX__; }
    static constexpr double epsilon()  { return __DBL_EPSILON__; }
    static constexpr bool  is_signed() { return true; }
};

template<>
struct NumericLimits<long double> {
    static constexpr long double lowest()    { return -__LDBL_MAX__; }
    static constexpr long double min()       { return __LDBL_MIN__; }
    static constexpr long double max()       { return __LDBL_MAX__; }
    static constexpr long double epsilon()   { return __LDBL_EPSILON__; }
    static constexpr bool        is_signed() { return true; }
};

#endif

/* clang-format on */

} /* namespace Trait */

using Trait::NumericLimits;

} /* namespace TC */