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
namespace Math {

template<typename T>
constexpr T clamp(T const& value, T const& min, T const& max);

template<typename T>
constexpr int count_leading_zeroes(T value);

template<typename T>
constexpr T max(T const& a, T const& b);

template<typename T>
constexpr T min(T const& a, T const& b);

} /* namespace Math */

using Math::clamp;
using Math::count_leading_zeroes;
using Math::max;
using Math::min;

} /* namespace TC */

#include <LibTC/Math.hhi>