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

#include <LibTC/Trait/IsArithmetic.hh>
#include <LibTC/Trait/IsNullPtr.hh>
#include <LibTC/Trait/IsVoid.hh>

namespace TC {
namespace Trait {

template<typename T>
inline constexpr bool IsFundamental = IsArithmetic<T> || IsVoid<T> || IsNullPtr<T>;

template<typename T>
concept Fundamental = IsFundamental<T>;

} /* namespace Trait */

using Trait::Fundamental;
using Trait::IsFundamental;

} /* namespace TC */