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
    fprintf(stdout, "\033[31mASSERTION FAILED: %s\033[0m\n", message);
    fflush(stdout);

    exit(EXIT_FAILURE);
}
