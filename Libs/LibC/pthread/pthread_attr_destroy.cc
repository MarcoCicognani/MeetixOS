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

#include <errno.h>
#include <pthread.h>

extern "C" int pthread_attr_destroy(pthread_attr_t* pthread_attr) {
    if ( !pthread_attr ) {
        errno = EINVAL;
        return -1;
    }

    /* destroy the value */
    *pthread_attr = PTHREAD_CREATE_DETACHED;
    return 0;
}