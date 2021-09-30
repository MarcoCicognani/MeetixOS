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

#pragma once

#include <Api/Kernel.h>
#include <pthread.h>

__BEGIN_C

/* ------------------------------------------- C types ------------------------------------------ */

struct pthread_t {
    Tid            m_thread_id;
    void*          m_exit_value;
    pthread_attr_t m_pthread_attr;
};

__END_C