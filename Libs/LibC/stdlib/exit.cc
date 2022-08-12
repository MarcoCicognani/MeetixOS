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

#include "../libc_main_internal.hh"

#include <LibApi/Api.h>
#include <LibC/stdlib.h>

extern "C" void exit(int code) {
    libc_fini();
    s_exit(code);
    __builtin_unreachable();
}
