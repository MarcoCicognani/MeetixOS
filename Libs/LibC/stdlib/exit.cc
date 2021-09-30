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

#include "../libc_main_internal.hh"

#include <Api.h>
#include <cstdlib>

void exit(int code) {
    libc_fini();
    s_exit(code);
    __builtin_unreachable();
}
