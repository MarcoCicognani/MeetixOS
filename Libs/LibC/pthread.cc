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

#pragma clang diagnostic push
#pragma ide diagnostic   ignored "modernize-use-trailing-return-type"

#include <LibApi/Api.h>
#include <LibC/errno.h>
#include <LibC/pthread.h>
#include <LibTC/Alloc/Box.hh>
#include <LibTC/Core/ErrorOr.hh>
#include <LibTC/Lang/Cxx.hh>
#include <LibTC/Lang/Function.hh>

extern "C" {

struct pthread_t {
    Tid            m_thread_id;
    void*          m_exit_value;
    pthread_attr_t m_pthread_attr;
};

} /* extern "C" */

static thread_local pthread_t s_pthread_self{ s_get_tid() };

struct PThreadArgs {
    void*                  m_arg_ptr{ nullptr };
    pthread_t*             m_pthread{ nullptr };
    Function<void*(void*)> m_user_routine;

    [[nodiscard]] static auto construct_from_args(void* arg_ptr, pthread_t* pthread, Function<void*(void*)> user_routine) -> ErrorOr<Box<PThreadArgs>> {
        return Box<PThreadArgs>::try_construct_from_emplace(arg_ptr, pthread, Cxx::move(user_routine));
    }
};

[[gnu::noinline]] static auto pthread_entry_point(PThreadArgs* pthread_args_ptr) -> void {
    auto pthread_args_box = Box<PThreadArgs>::construct_from_adopt(*pthread_args_ptr);

    pthread_args_box->m_pthread->m_exit_value = pthread_args_box->m_user_routine(pthread_args_box->m_arg_ptr);
}

extern "C" {

int pthread_create(pthread_t* pthread, const pthread_attr_t* pthread_attr, void* (*start_routine)(void*), void* arg_ptr) {
    if ( !pthread ) {
        errno = EINVAL;
        return -1;
    }

    /* construct the pthread arguments */
    auto error_or_pthread_args = PThreadArgs::construct_from_args(arg_ptr, pthread, start_routine);
    if ( error_or_pthread_args.is_error() ) {
        errno = error_or_pthread_args.unwrap_error().code();
        return -1;
    }

    auto pthread_args         = error_or_pthread_args.unwrap();
    auto create_thread_status = CREATE_THREAD_STATUS_FAILED;
    auto entry_point          = bit_cast<void*>(&pthread_entry_point);
    pthread->m_thread_id      = s_create_thread_ds(entry_point, pthread_args.leak_ptr(), &create_thread_status);

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
    return &s_pthread_self;
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

} /* extern "C" */

#pragma clang diagnostic pop
