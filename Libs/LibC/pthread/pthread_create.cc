/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
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
#include <malloc.h>
#include <new>
#include <pthread.h>

using PThreadFn = void* (*)(void*);

struct PThreadArgs {
    void*      m_arg_ptr{ nullptr };
    pthread_t* m_pthread{ nullptr };
    PThreadFn  m_user_routine{ nullptr };

    PThreadArgs(void* arg_ptr, pthread_t* pthread, PThreadFn user_routine)
        : m_arg_ptr{ arg_ptr }
        , m_pthread{ pthread }
        , m_user_routine{ user_routine } {}
};

static A_NOINLINE void pthread_entry_point(PThreadArgs* pthread_args) {
    pthread_args->m_pthread->m_exit_value = pthread_args->m_user_routine(pthread_args->m_arg_ptr);
    delete pthread_args;
}

extern "C" int
pthread_create(pthread_t* pthread, const pthread_attr_t* pthread_attr, void* (*start_routine)(void*), void* arg_ptr) {
    if ( !pthread ) {
        errno = EINVAL;
        return -1;
    }

    /* start the PThread */
    CreateThreadStatus create_thread_status;
    pthread->m_thread_id = s_create_thread_ds(reinterpret_cast<void*>(pthread_entry_point),
                                              new PThreadArgs{ arg_ptr, pthread, start_routine },
                                              &create_thread_status);

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
}
