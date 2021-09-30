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

extern "C" void fseterr(FILE* stream) {
    s_atomic_lock(&stream->m_lock);
    if ( stream->m_impl_set_err )
        stream->m_impl_set_err(stream);
    else
        errno = ENOTSUP;
    stream->m_lock = false;
}
