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

#include <LibTC/Trait/RemoveConstVolatile.hh>

namespace TC {
namespace Trait {
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

} /* namespace Trait */

using Trait::IsFloatingPoint;

} /* namespace TC */