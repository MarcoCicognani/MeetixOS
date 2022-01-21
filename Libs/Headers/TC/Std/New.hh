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

inline void* operator new(long unsigned int, void* placement_memory) {
    return placement_memory;
}

inline void* operator new[](long unsigned int, void* placement_memory) {
    return placement_memory;
}