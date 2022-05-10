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

#include <LibTC/Trait/IsSame.hh>
#include <LibTC/Trait/MakeUnsigned.hh>

namespace TC {
namespace Trait {

template<typename T>
inline constexpr bool IsUnsigned = IsSame<T, MakeUnsigned<T>>;

template<typename T>
concept Unsigned = IsUnsigned<T>;

} /* namespace Trait */

using Trait::IsUnsigned;
using Trait::Unsigned;

} /* namespace TC */