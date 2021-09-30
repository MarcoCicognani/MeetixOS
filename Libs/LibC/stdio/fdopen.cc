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

#include <stdio.h>

extern "C" FILE* fdopen(int fd, const char* mode) {
    /* fill the stream */
    auto stream = new FILE{};
    if ( fdopen_static(fd, mode, stream) == EOF )
        return nullptr;
    return stream;
}
