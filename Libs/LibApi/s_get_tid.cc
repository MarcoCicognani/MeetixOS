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

#include <Api/User.h>

Tid s_get_tid() {
    SyscallGetTid data;
    do_syscall(SYSCALL_THREAD_GET_ID, (usize)&data);
    return data.m_thread_id;
}
