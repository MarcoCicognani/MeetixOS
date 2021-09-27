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

#include "stdio_internal.h"

#include <stdio.h>

int fseek(FILE* stream, long int offset, int whence) {
    return fseeko(stream, (off_t)offset, whence);
}

int fseeko(FILE* stream, off_t offset, int whence) {
    s_atomic_lock(&stream->m_lock);
    int res        = fseeko_unlocked(stream, offset, whence);
    stream->m_lock = false;
    return res;
}
