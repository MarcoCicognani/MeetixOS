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

#include <LibC/errno.h>
#include <LibTC/BitCast.hh>

extern "C" {
__thread OSError g_errno_value = ENOERR;

int* errno_location() {
    return bit_cast<int*>(&g_errno_value);
}
}
