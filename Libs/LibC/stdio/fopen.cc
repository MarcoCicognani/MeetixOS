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

extern "C" FILE* fopen(const char* filename, const char* mode) {
    auto stream = new FILE{};
    stream      = fopen_static(filename, mode, stream);
    if ( !stream ) {
        delete stream;
        return nullptr;
    }
    return stream;
}
