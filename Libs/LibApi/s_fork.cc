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

Tid s_fork() {
    SyscallFork data;
    do_syscall(SYSCALL_PROCESS_FORK, (usize)&data);
    return data.m_forked_proc_id;
}
