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

extern "C" char* fgets(char* s, int n, FILE* stream) {
    auto p = s;
    --n;
    int c;
    while ( n && (c = fgetc(stream)) != EOF ) {
        *p++ = static_cast<char>(c);
        --n;
    }
    *p = '\0';
    return s;
}
