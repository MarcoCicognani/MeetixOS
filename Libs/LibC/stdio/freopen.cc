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

#include <errno.h>
#include <stdio.h>

extern "C" FILE* freopen(const char* filename, const char* mode, FILE* stream) {
    s_atomic_lock(&stream->m_lock);
    FILE* res;
    if ( stream->m_impl_reopen ) {
        res = stream->m_impl_reopen(filename, mode, stream);
    } else {
        errno = ENOTSUP;
        res   = nullptr;
    }
    stream->m_lock = false;
    return res;
}
