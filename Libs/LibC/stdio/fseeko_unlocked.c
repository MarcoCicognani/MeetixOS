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

int fseeko_unlocked(FILE* stream, off_t offset, int whence) {
    int res;
    if ( stream->m_impl_seek ) {
        res = stream->m_impl_seek(stream, offset, whence);
    } else {
        errno = ENOTSUP;
        res   = EOF;
    }
    return res;
}
