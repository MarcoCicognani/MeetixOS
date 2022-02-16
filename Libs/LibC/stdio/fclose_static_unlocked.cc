/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include "stdio_internal.hh"

#include <cstdio>
#include <cstring>

int fclose_static_unlocked(FILE* stream) {
    fflush_unlocked(stream);
    if ( stream->m_impl_close(stream) )
        return EOF;

    /* destroy the buffer if created by the library */
    if ( (stream->m_flags & FILE_FLAG_BUFFER_SET) && stream->m_io_buffer
         && (stream->m_flags & FILE_FLAG_BUFFER_OWNER_LIBRARY) ) {
        delete[] stream->m_io_buffer;
    }

    /* clean the stream object & remove from the open streams */
    memset(stream, 0, sizeof(FILE));
    open_file_list_remove(stream);
    return 0;
}
