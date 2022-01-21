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

#include <TC/Traits/RemoveConstVolatile.hh>

namespace TC::Traits {
namespace Details {

template<typename T>
inline constexpr bool IsFloatingPoint = false;

template<>
inline constexpr bool IsFloatingPoint<float> = true;

template<>
inline constexpr bool IsFloatingPoint<double> = true;

template<>
inline constexpr bool IsFloatingPoint<long double> = true;

} /* namespace Details */

template<typename T>
inline constexpr bool IsFloatingPoint = Details::IsFloatingPoint<RemoveConstVolatile<T>>;

} /* namespace TC::Traits */