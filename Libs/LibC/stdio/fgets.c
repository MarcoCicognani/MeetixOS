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

char* fgets(char* s, int n, FILE* stream) {
    char* p = s;
    --n;
    int c;
    while ( n && (c = fgetc(stream)) != EOF ) {
        *p++ = (char)c;
        --n;
    }
    *p = '\0';
    return s;
}
