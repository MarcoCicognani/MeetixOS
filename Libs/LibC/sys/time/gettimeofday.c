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

#include <Api.h>
#include <sys/time.h>

int gettimeofday(A_UNUSED struct timeval* tp, A_UNUSED struct timezone* tzp) {
    __NOT_IMPLEMENTED(gettimeofday);
    return -1;
}
