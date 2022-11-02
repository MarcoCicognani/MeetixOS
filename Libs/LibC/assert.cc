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

#include <LibC/assert.h>
#include <LibC/stdio.h>
#include <LibC/stdlib.h>

extern "C" {

A_NORETURN void libc_runtime_panic(const char* file, int line, const char* expression) {
    fprintf(stderr, "\e[31mLibC RunTime Panic\e[0m in %s:%d\n> { %s }\n", file, line, expression);
    fflush(stderr);

    exit(EXIT_FAILURE);
}

} /* extern "C" */
