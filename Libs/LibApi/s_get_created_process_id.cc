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

Pid s_get_created_process_id(ProcessCreationIdentifier process) {
    SyscallGetCreatedProcessID data{ process };
    do_syscall(SYSCALL_GET_CREATED_PROCESS_ID, (usize)&data);
    return data.m_proc_id;
}
