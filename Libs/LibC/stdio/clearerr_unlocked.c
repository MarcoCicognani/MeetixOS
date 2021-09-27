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

void clearerr_unlocked(FILE* stream) {
    if ( stream->m_impl_clear_err ) {
        stream->m_impl_clear_err(stream);
    } else {
        errno = ENOTSUP;
    }
}
