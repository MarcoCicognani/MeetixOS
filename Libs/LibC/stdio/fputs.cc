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

#include <stdio.h>
#include <string.h>

extern "C" int fputs(const char* s, FILE* stream) {
    usize len           = strlen(s);
    usize written_bytes = fwrite(s, len, 1, stream);
    if ( written_bytes == len )
        return static_cast<int>(written_bytes);
    else
        return EOF;
}
