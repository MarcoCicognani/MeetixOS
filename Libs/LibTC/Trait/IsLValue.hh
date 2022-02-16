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

namespace TC {
namespace Trait {

template<typename T>
inline constexpr bool IsLValue = false;

template<typename T>
inline constexpr bool IsLValue<T&> = true;

} /* namespace Trait */

using Trait::IsLValue;

} /* namespace TC */