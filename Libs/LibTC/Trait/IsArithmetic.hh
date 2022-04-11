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

#include <LibTC/Trait/IsFloatingPoint.hh>
#include <LibTC/Trait/IsIntegral.hh>

namespace TC {
namespace Trait {

template<typename T>
inline constexpr bool IsArithmetic = IsIntegral<T> || IsFloatingPoint<T>;

template<typename T>
concept Arithmetic = IsArithmetic<T>;

} /* namespace Trait */

using Trait::Arithmetic;
using Trait::IsArithmetic;

} /* namespace TC */