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

int fflush(FILE* stream) {
    /* if no stream passed, flush all streams */
    if ( !stream ) {
        int   ret         = 0;
        FILE* open_stream = g_open_stream_list;
        while ( open_stream ) {
            ret |= fflush(open_stream);
            open_stream = open_stream->m_next_stream;
        }

        return ret;
    }

    /* lock file and perform flush */
    s_atomic_lock(&stream->m_lock);
    int res        = fflush_unlocked(stream);
    stream->m_lock = false;
    return res;
}
