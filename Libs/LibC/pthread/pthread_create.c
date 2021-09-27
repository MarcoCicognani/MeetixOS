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
#include <stdlib.h>

typedef struct {
    void*      m_arg_ptr;
    pthread_t* m_pthread;
    void* (*m_user_routine)(void*);
} pthread_routine_args_t;

static A_NOINLINE void pthread_entry_point(pthread_routine_args_t* args) {
    args->m_pthread->m_exit_value = args->m_user_routine(args->m_arg_ptr);
    free(args);
}

int pthread_create(pthread_t*            pthread,
                   const pthread_attr_t* pthread_attr,
                   void* (*start_routine)(void*),
                   void* arg_ptr) {
    if ( !pthread ) {
        errno = EINVAL;
        return -1;
    }

    pthread_routine_args_t* t = malloc(sizeof(pthread_routine_args_t));
    if ( t ) {
        t->m_user_routine = start_routine;
        t->m_pthread      = pthread;
        t->m_arg_ptr      = arg_ptr;

        CreateThreadStatus create_thread_status;
        pthread->m_thread_id
            = s_create_thread_ds((void*)pthread_entry_point, t, &create_thread_status);

        /* copy the attribute value */
        if ( pthread_attr )
            pthread->m_pthread_attr = *pthread_attr;
        else
            pthread->m_pthread_attr = PTHREAD_CREATE_JOINABLE;

        /* check return status */
        if ( create_thread_status == CREATE_THREAD_STATUS_SUCCESSFUL ) {
            return 0;
        } else {
            errno = ECANCELED;
            return -1;
        }
    } else {
        errno = ENOMEM;
        return -1;
    }
}