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

#include <TC/Std/Forward.hh>
#include <TC/Std/Move.hh>

namespace std {

template<typename T, typename U = T>
constexpr T exchange(T& slot, U&& value) {
    T old_value{ move(slot) };
    slot = forward<U>(value);
    return old_value;
}

} /* namespace std */