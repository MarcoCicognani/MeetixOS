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
#include <TC/Assertion.hh>

void assert_failed(const char* file, int line, const char* function, const char* expr) {
    TC::Assertion::assertion_failed(expr, file, static_cast<u32>(line), function);
}

namespace TC::Assertion {

void assertion_failed(const char* message, const char* file, u32 line, const char* function) {
    fprintf(stderr, "PANIC: Assertion failed\n%s:%d\n\n%s {\n\t%s\n}\n", file, line, function, message);
    fflush(stderr);

    exit(EXIT_FAILURE);
}

} /* namespace TC::Assertion */