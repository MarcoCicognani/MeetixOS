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

int fdopen_static(int fd, const char* mode, FILE* stream) {
    int flags = parse_mode_flags(mode);
    if ( flags == EOF )
        return EOF;

    /* fill the stream object */
    stream->m_fd          = fd;
    stream->m_flags       = flags;
    stream->m_buffer_mode = _IONBF;
    stream->m_impl_close  = stdio_impl_close;
    stream->m_impl_read   = stdio_impl_read;
    stream->m_impl_write  = stdio_impl_write;
    stream->m_impl_seek   = stdio_impl_seek;
    stream->m_impl_tell   = stdio_impl_tell;
    stream->m_impl_fileno = stdio_impl_fileno;
    stream->m_impl_reopen = stdio_impl_reopen;
    stream->m_impl_error  = stdio_impl_error;
    stream->m_impl_eof    = stdio_impl_eof;
    stream->m_lock        = false;

    /* add the stream to the open streams */
    open_file_list_add(stream);
    return 0;
}
