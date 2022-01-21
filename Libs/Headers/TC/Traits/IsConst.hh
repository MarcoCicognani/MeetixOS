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

namespace TC::Traits {

template<typename T>
inline constexpr bool IsConst = false;

template<typename T>
inline constexpr bool IsConst<T const> = true;

} /* namespace TC::Traits */