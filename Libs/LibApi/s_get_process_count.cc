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

usize s_get_process_count() {
    SyscallGetProcessCount data;
    do_syscall(SYSCALL_GET_PROCESS_COUNT, (usize)&data);
    return data.m_count;
}