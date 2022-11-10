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

u64 s_millis() {
    SyscallMillis data;
    do_syscall(SYSCALL_SCHEDULER_GET_MILLISECONDS, (usize)&data);
    return data.m_millis_amount;
}
