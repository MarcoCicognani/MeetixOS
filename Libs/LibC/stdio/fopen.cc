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

#ifndef LIBC_BUILDING_LIBSTDCXX
#    include "stdio_internal.hh"

#    include <stdio.h>

extern "C" FILE* fopen(const char* filename, const char* mode) {
    auto stream = new FILE{};
    stream      = fopen_static(filename, mode, stream);
    if ( !stream ) {
        delete stream;
        return nullptr;
    }
    return stream;
}
#else
#    include <stdio.h>

extern "C" FILE* fopen(const char*, const char*) {
    return nullptr;
}
#endif