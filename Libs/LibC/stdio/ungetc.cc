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

extern "C" int ungetc(int c, FILE* stream) {
    s_atomic_lock(&stream->m_lock);
    auto res       = fungetc_unlocked(c, stream);
    stream->m_lock = false;
    return res;
}
