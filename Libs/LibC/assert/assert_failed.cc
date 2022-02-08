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

extern "C" A_NORETURN void assert_failed(const char* message) {
    fprintf(stderr, "ASSERTION FAILED: %s\n", message);
    fflush(stderr);

    exit(EXIT_FAILURE);
}
