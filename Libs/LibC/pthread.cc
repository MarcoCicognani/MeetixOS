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

#include <LibApi/Api.h>
#include <LibC/errno.h>
#include <LibC/pthread.h>

struct pthread_t {
    Tid            m_thread_id;
    void*          m_exit_value;
    pthread_attr_t m_pthread_attr;
};

using PThreadFn = void* (*)(void*);

struct PThreadArgs {
    void*      m_arg_ptr{ nullptr };
    pthread_t* m_pthread{ nullptr };
    PThreadFn  m_user_routine{ nullptr };

    PThreadArgs(void* arg_ptr, pthread_t* pthread, PThreadFn user_routine)
        : m_arg_ptr{ arg_ptr }
        , m_pthread{ pthread }
        , m_user_routine{ user_routine } {
    }
};

static A_NOINLINE void pthread_entry_point(PThreadArgs* pthread_args) {
    pthread_args->m_pthread->m_exit_value = pthread_args->m_user_routine(pthread_args->m_arg_ptr);
    delete pthread_args;
}

extern "C" {

int pthread_create(pthread_t* pthread, const pthread_attr_t* pthread_attr, void* (*start_routine)(void*), void* arg_ptr) {
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

pthread_t* pthread_self(void) {
    return new pthread_t{ s_get_tid() };
}

int pthread_join(pthread_t* pthread, void** ret_val) {
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

int pthread_kill(pthread_t* pthread, int) {
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

void pthread_exit(void*) {
    return;
}

int pthread_attr_init(pthread_attr_t* pthread_attr) {
    if ( !pthread_attr ) {
        errno = EINVAL;
        return -1;
    }

    /* set the only value supported */
    *pthread_attr = PTHREAD_CREATE_JOINABLE;
    return 0;
}

int pthread_attr_destroy(pthread_attr_t* pthread_attr) {
    if ( !pthread_attr ) {
        errno = EINVAL;
        return -1;
    }

    /* destroy the value */
    *pthread_attr = PTHREAD_CREATE_DETACHED;
    return 0;
}

int pthread_set_detachstate(pthread_attr_t* pthread_attr, int detach_state) {
    if ( !pthread_attr || (detach_state != PTHREAD_CREATE_DETACHED && detach_state != PTHREAD_CREATE_JOINABLE) ) {
        errno = EINVAL;
        return -1;
    }

    *pthread_attr = detach_state;
    return 0;
}
}