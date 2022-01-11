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

#include <Api/StdInt.h>

inline void* operator new(usize, void* placement_memory) {
    return placement_memory;
}

inline void* operator new[](usize, void* placement_memory) {
    return placement_memory;
}