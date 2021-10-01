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

#ifndef LIBC_BUILDING_LIBSTDCXX
#    include "pthread_internal.hh"

#    include <Api.h>
#    include <pthread.h>

pthread_t* pthread_self(void) {
    return new pthread_t{ s_get_tid() };
}
#else
#    include <pthread.h>

pthread_t* pthread_self(void) {
    return nullptr;
}
#endif
