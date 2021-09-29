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

int fileno(FILE* stream) {
    int fd = -1;

    s_atomic_lock(&stream->m_lock);
    if ( stream->m_impl_fileno )
        fd = stream->m_impl_fileno(stream);
    else
        errno = ENOTSUP;
    stream->m_lock = false;

    return fd;
}
