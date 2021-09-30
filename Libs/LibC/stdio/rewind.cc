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

extern "C" void rewind(FILE* stream) {
    s_atomic_lock(&stream->m_lock);
    fseeko_unlocked(stream, 0, SEEK_SET);
    clearerr_unlocked(stream);
    stream->m_lock = false;
}
