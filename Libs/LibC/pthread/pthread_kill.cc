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

extern "C" int pthread_kill(pthread_t* pthread, int) {
    if ( !pthread ) {
        errno = EINVAL;
        return -1;
    }

    auto kill_status = s_kill(pthread->m_thread_id);
    if ( kill_status == KILL_STATUS_SUCCESSFUL ) {
        return 0;
    } else if ( kill_status == KILL_STATUS_NOT_FOUND ) {
        errno = ENOENT;
        return -1;
    } else {
        errno = EINVAL;
        return -1;
    }
}