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

KillStatus s_kill(Pid proc_id) {
    SyscallKill data{ proc_id };
    do_syscall(SYSCALL_THREAD_KILL, (usize)&data);
    return data.m_kill_status;
}
