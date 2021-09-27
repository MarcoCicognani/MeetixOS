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

Pid s_get_pid_for_tid(Tid thread_id) {
    SyscallGetPidForTid data{ thread_id };
    do_syscall(SYSCALL_PROCESS_GET_ID_FOR_THREAD_ID, (usize)&data);
    return data.m_proc_id;
}
