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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void assert_failed(const char* file, int line, const char* function, const char* expr) {
    fprintf(stderr, "PANIC: Assertion failed\n%s:%d\n\n%s {\n\t%s\n}\n", file, line, function, expr);
    fflush(stderr);

    exit(EXIT_FAILURE);
}
