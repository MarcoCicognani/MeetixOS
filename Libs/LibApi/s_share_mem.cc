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

void* s_share_mem(void* memory, usize size, Pid proc_id) {
    SyscallShareMem data{ memory, size, proc_id };
    do_syscall(SYSCALL_MEMORY_SHARE, (usize)&data);
    return data.m_shared_ptr;
}
