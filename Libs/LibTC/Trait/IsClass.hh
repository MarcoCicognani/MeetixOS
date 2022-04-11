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
inline constexpr bool IsClass = __is_class(T);

template<typename T>
concept Class = IsClass<T>;

} /* namespace Trait */

using Trait::Class;
using Trait::IsClass;

} /* namespace TC */