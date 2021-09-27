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
#include <pthread.h>
#include <stdlib.h>

pthread_t* pthread_self(void) {
    pthread_t* pthread = malloc(sizeof(pthread_t));
    if ( pthread ) {
        pthread->m_thread_id = s_get_tid();
        return pthread;
    } else {
        return NULL;
    }
}