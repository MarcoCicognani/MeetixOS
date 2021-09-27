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

long int ftell(FILE* stream) {
    return (long int)ftello(stream);
}

off_t ftello(FILE* stream) {
    s_atomic_lock(&stream->m_lock);
    off_t res      = ftello_unlocked(stream);
    stream->m_lock = false;
    return res;
}
