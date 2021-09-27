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

static isize vcbprintf_vfprintf_callback(void* param, const char* str, usize str_len) {
    return (isize)fwrite_unlocked(str, 1, str_len, (FILE*)param);
}

int vfprintf_unlocked(FILE* stream, const char* format, va_list arg) {
    if ( !(stream->m_flags & FILE_FLAG_MODE_WRITE) ) {
        errno = EBADF;
        stream->m_flags |= FILE_FLAG_ERROR;
        return EOF;
    }

    return vcbprintf(stream, vcbprintf_vfprintf_callback, format, arg);
}
