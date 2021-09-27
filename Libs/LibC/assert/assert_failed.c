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

void assert_failed(const char* file, int line, const char* function, const char* expr) {
    fprintf(stderr, "Assertion failed: %s:%lu: %s: %s\n", file, line, function, expr);
    klog("Assertion failed: %s:%lu: %s: %s\n", file, line, function, expr);
}
