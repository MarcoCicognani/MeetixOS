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

#include <errno.h>
#include <stdio.h>

extern "C" int feof(FILE* stream) {
    s_atomic_lock(&stream->m_lock);
    int res;
    if ( stream->m_impl_eof ) {
        res = stream->m_impl_eof(stream);
    } else {
        errno = ENOTSUP;
        res   = EOF;
    }
    stream->m_lock = false;
    return res;
}
