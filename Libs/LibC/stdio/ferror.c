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

#include <errno.h>
#include <stdio.h>

int ferror(FILE* stream) {
    s_atomic_lock(&stream->m_lock);
    int res;
    if ( stream->m_impl_error ) {
        res = stream->m_impl_error(stream);
    } else {
        errno = ENOTSUP;
        res   = EOF;
    }
    stream->m_lock = false;
    return res;
}
