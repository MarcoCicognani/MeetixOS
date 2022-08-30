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

#include <LibTC/Assertions.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Math {

template<typename T>
[[nodiscard]]
constexpr auto clamp(T const& value, T const& min, T const& max) -> T {
    VERIFY_GREATER_EQUAL(max, min);
    if ( value > max )
        return max;
    if ( value < min )
        return min;
    return value;
}

template<typename T>
[[nodiscard]]
constexpr auto count_leading_zeroes(T value) -> i32 {
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
[[nodiscard]]
constexpr auto max(T const& a, T const& b) -> T {
    return a < b ? b : a;
}

template<typename T>
[[nodiscard]]
constexpr auto min(T const& a, T const& b) -> T {
    return b < a ? b : a;
}

template<typename T, typename U>
[[nodiscard]]
constexpr auto ceil_div(T a, U b) -> T {
    static_assert(sizeof(T) == sizeof(U));

    T result = a / b;
    if ( (a % b) != 0 )
        ++result;
    return result;
}

} /* namespace Math */

using Math::ceil_div;
using Math::clamp;
using Math::count_leading_zeroes;
using Math::max;
using Math::min;

} /* namespace TC */
