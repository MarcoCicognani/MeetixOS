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
#pragma ide diagnostic ignored "modernize-use-trailing-return-type"

#include <LibC/errno.h>
#include <LibTC/Lang/Cxx.hh>

extern "C" {

__thread ErrnoCode g_errno_value = ENOERR;

int* errno_location() {
    return Cxx::bit_cast<int*>(&g_errno_value);
}

} /* extern "C" */

#pragma clang diagnostic pop
