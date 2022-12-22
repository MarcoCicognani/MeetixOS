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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "bugprone-reserved-identifier"

#include "lib.hh"
#include "stdio.hh"

auto libc_init() -> void {
    stdio_init();
}

auto libc_fini() -> void {
    stdio_fini();
}

#pragma clang diagnostic pop /* bugprone-reserved-identifier */
