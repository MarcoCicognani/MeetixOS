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

int pthread_set_detachstate(pthread_attr_t* pthread_attr, int detach_state) {
    if ( !pthread_attr
         || (detach_state != PTHREAD_CREATE_DETACHED && detach_state != PTHREAD_CREATE_JOINABLE) ) {
        errno = EINVAL;
        return -1;
    }

    *pthread_attr = detach_state;
    return 0;
}