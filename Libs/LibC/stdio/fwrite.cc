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

#include "stdio_internal.hh"

#include <stdio.h>

extern "C" usize fwrite(const void* ptr, usize size, usize count, FILE* stream) {
    s_atomic_lock(&stream->m_lock);
    auto len       = fwrite_unlocked(ptr, size, count, stream);
    stream->m_lock = false;
    return len;
}
