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

#include <Api/User.h>

Tid s_task_get_id(const char* identifier) {
    SyscallTaskIDGet data{ identifier };
    do_syscall(SYSCALL_THREAD_GET_ID_FOR_NAME, (usize)&data);
    return data.m_thread_id;
}
