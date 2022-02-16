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
#include <LibTC/Trait/RemoveConstVolatile.hh>

namespace TC {
namespace Trait {

template<typename T>
inline constexpr bool IsVoid = IsSame<void, RemoveConstVolatile<T>>;

} /* namespace Trait */

using Trait::IsVoid;

} /* namespace TC */