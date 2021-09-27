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

FILE* stdio_impl_reopen(const char* filename, const char* mode, FILE* stream) {
    /* if no filename specified, attempt to change file mode */
    if ( !filename ) {
        /* parse mode flags */
        int new_mode = parse_mode_flags(mode);
        if ( new_mode == EOF )
            return NULL;

        /* TODO ask kernel to change the flags */
        stream->m_flags &= ~FILE_FLAGS_MODE_RANGE;
        stream->m_flags |= new_mode;
        return stream;
    }

    /* close file */
    int close = fclose_static_unlocked(stream);
    if ( close != 0 )
        return NULL;

    /* open it again */
    return fopen_static(filename, mode, stream);
}
