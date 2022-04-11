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
inline constexpr bool IsUnion = __is_union(T);

template<typename T>
concept Union = IsUnion<T>;

} /* namespace Trait */

using Trait::IsUnion;
using Trait::Union;

} /* namespace TC */