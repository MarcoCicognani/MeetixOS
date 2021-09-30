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

#include "pthread_internal.hh"

#include <Api.h>
#include <cerrno>
#include <pthread.h>

extern "C" int pthread_join(pthread_t* pthread, void** ret_val) {
    if ( !pthread ) {
        errno = EINVAL;
        return -1;
    }

    if ( pthread->m_pthread_attr != PTHREAD_CREATE_JOINABLE ) {
        errno = EINVAL;
        return -1;
    }

    /* join the thread */
    s_join(pthread->m_thread_id);
    if ( *ret_val )
        *ret_val = pthread->m_exit_value;
    return 0;
}