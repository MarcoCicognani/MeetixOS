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

#include <TC/Std/Move.hh>

namespace std {

template<typename T>
void swap(T& left, T& right) noexcept {
    T tmp{ std::move(left) };
    left  = std::move(right);
    right = std::move(tmp);
}

} /* namespace std */