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

#include <malloc.h>
#include <stdio.h>
#include <string.h>

int fclose_static_unlocked(FILE* stream) {
    fflush_unlocked(stream);
    if ( stream->m_impl_close(stream) )
        return EOF;

    /* destroy the buffer if created by the library */
    if ( (stream->m_flags & FILE_FLAG_BUFFER_SET) && (stream->m_io_buffer != NULL)
         && (stream->m_flags & FILE_FLAG_BUFFER_OWNER_LIBRARY) ) {
        free(stream->m_io_buffer);
    }

    /* clean the stream object & remove from the open streams */
    memset(stream, 0, sizeof(FILE));
    open_file_list_remove(stream);
    return 0;
}
