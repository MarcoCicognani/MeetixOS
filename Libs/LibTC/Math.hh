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

#include <LibTC/Assertions.hh>

namespace TC {
namespace Math {

template<typename T>
constexpr T clamp(T const& value, T const& min, T const& max) {
    VERIFY_GREATER_EQUAL(max, min);
    if ( value > max )
        return max;
    if ( value < min )
        return min;
    return value;
}

template<typename T>
constexpr int count_leading_zeroes(T value) {
    static_assert(sizeof(T) <= sizeof(unsigned long long));
    if constexpr ( sizeof(T) <= sizeof(unsigned int) )
        return __builtin_clz(value) - (32 - (8 * sizeof(T)));
    if constexpr ( sizeof(T) == sizeof(unsigned long) )
        return __builtin_clzl(value);
    if constexpr ( sizeof(T) == sizeof(unsigned long long) )
        return __builtin_clzll(value);
    VERIFY_NOT_REACHED();
}

template<typename T>
constexpr T max(T const& a, T const& b) {
    return a < b ? b : a;
}

template<typename T>
constexpr T min(T const& a, T const& b) {
    return b < a ? b : a;
}

} /* namespace Math */

using Math::clamp;
using Math::count_leading_zeroes;
using Math::max;
using Math::min;

} /* namespace TC */
