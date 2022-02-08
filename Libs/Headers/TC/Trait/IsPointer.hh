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

#include <TC/Trait/RemoveConstVolatile.hh>

namespace TC {
namespace Trait {
namespace Details {

template<typename T>
inline constexpr bool IsPointer = false;

template<typename T>
inline constexpr bool IsPointer<T*> = true;

} /* namespace Details */

template<typename T>
inline constexpr bool IsPointer = Details::IsPointer<RemoveConstVolatile<T>>;

} /* namespace Trait */

using Trait::IsPointer;

} /* namespace TC */