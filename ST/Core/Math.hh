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

#include <ST/Forward.hh>

#include <ST/Core/Assertions.hh>
#include <ST/Lang/IntTypes.hh>

namespace Math {

template<typename T>
[[nodiscard]]
constexpr auto clamp(T const& value, T const& min, T const& max) -> T {
    verify_greater_equal$(max, min);
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
    verify_not_reached$();
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
