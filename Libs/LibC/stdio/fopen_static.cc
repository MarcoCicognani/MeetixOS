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

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

FILE* fopen_static(const char* filename, const char* mode, FILE* stream) {
    auto flags = parse_mode_flags(mode);
    if ( flags == EOF )
        return nullptr;

    /* perform actual open */
    auto fd = open(filename, flags);
    if ( fd < 0 )
        return nullptr;

    /* create file stream */
    if ( !fdopen_static(fd, mode, stream) )
        return stream;

    /* if it failed, close file */
    close(fd);
    return nullptr;
}
