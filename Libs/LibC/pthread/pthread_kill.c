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

#include "pthread_internal.h"

#include <Api.h>
#include <errno.h>
#include <pthread.h>

int pthread_kill(pthread_t* pthread, A_UNUSED int sig) {
    if ( !pthread ) {
        errno = EINVAL;
        return -1;
    }

    KillStatus kill_status = s_kill(pthread->m_thread_id);
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