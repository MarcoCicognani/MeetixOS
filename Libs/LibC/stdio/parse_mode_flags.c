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

int parse_mode_flags(const char* mode) {
    int flags = 0;
    switch ( *mode ) {
        case 'r':
            flags = FILE_FLAG_MODE_READ;
            break;
        case 'w':
            flags = FILE_FLAG_MODE_WRITE | FILE_FLAG_MODE_CREATE | FILE_FLAG_MODE_TRUNCATE;
            break;
        case 'a':
            flags = FILE_FLAG_MODE_WRITE | FILE_FLAG_MODE_CREATE | FILE_FLAG_MODE_APPEND;
            break;
        default:
            errno = EINVAL;
            return EOF;
    }

    while ( *(++mode) ) {
        switch ( *mode ) {
            case '+':
                flags |= FILE_FLAG_MODE_READ | FILE_FLAG_MODE_WRITE;
                break;
            case 't':
                flags &= ~FILE_FLAG_MODE_BINARY;
                flags |= FILE_FLAG_MODE_TEXTUAL;
                break;
            case 'b':
                flags &= ~FILE_FLAG_MODE_TEXTUAL;
                flags |= FILE_FLAG_MODE_BINARY;
                break;
            default:
                errno = EINVAL;
                return EOF;
        }
    }

    return flags;
}
