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

#include <stdio.h>

extern "C" void setbuf(FILE* stream, char* buffer) {
    setvbuf(stream, buffer, buffer ? _IONBF : _IOFBF, BUFSIZ);
}
