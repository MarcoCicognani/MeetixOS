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
    if ( stream->m_impl_fileno )
        stream->m_impl_fileno(stream);
    else {
        errno = ENOTSUP;
        return -1;
    }
}
