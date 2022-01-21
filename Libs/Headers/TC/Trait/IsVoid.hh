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

#include <TC/Trait/IsSame.hh>
#include <TC/Trait/RemoveConstVolatile.hh>

namespace TC::Trait {

template<typename T>
inline constexpr bool IsVoid = IsSame<void, RemoveConstVolatile<T>>;

} /* namespace TC::Trait */