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

#include <LibApi/Api.h>
#include <LibC/sys/stat.h>

extern "C" mode_t umask(mode_t) {
    __NOT_IMPLEMENTED(umask);
    return -1;
}
