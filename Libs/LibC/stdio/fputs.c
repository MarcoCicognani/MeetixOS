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
#include <string.h>

int fputs(const char* s, FILE* stream) {
    usize len           = strlen(s);
    usize written_bytes = fwrite(s, len, 1, stream);
    if ( written_bytes == len )
        return (int)written_bytes;
    else
        return EOF;
}
