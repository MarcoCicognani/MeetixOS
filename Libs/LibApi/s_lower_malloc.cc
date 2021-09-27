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

void* s_lower_malloc(usize size) {
    SyscallLowerMalloc data{ size };
    do_syscall(SYSCALL_MEMORY_LOWER_ALLOCATE, (usize)&data);
    return data.m_region_ptr;
}
