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

#include "stdio_internal.hh"

#include <stdio.h>

extern "C" int setvbuf(FILE* stream, char* buffer, int mode, usize size) {
    s_atomic_lock(&stream->m_lock);
    auto res       = setvbuf_unlocked(stream, buffer, mode, size);
    stream->m_lock = false;
    return res;
}
