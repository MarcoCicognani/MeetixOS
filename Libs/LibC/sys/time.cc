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

#include <LibApi/Api.h>
#include <LibC/sys/time.h>

extern "C" {

int gettimeofday(struct timeval*, struct timezone*) {
    __NOT_IMPLEMENTED(gettimeofday);
    return -1;
}
}