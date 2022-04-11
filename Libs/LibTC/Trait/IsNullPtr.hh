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

#include <LibTC/Cxx.hh>
#include <LibTC/Trait/IsSame.hh>
#include <LibTC/Trait/RemoveConstVolatile.hh>

namespace TC {
namespace Trait {

template<typename T>
inline constexpr bool IsNullPtr = IsSame<nullptr_t, RemoveConstVolatile<T>>;

} /* namespace Trait */

using Trait::IsNullPtr;

} /* namespace TC */