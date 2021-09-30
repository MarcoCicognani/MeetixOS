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

#include <cerrno>
#include <cstdio>

off_t ftello_unlocked(FILE* stream) {
    off_t res;
    if ( stream->m_impl_tell ) {
        res = stream->m_impl_tell(stream);
    } else {
        errno = ENOTSUP;
        res   = EOF;
    }
    return res;
}
