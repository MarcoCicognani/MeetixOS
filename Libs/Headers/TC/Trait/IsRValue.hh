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

namespace TC::Trait {

template<typename T>
inline constexpr bool IsRValue = false;

template<typename T>
inline constexpr bool IsRValue<T&&> = true;

} /* namespace TC::Trait */