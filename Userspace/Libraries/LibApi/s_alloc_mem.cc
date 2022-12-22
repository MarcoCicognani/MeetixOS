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

void* s_alloc_mem(unsigned int size) {
    SyscallAllocMem data{ size };
    do_syscall(SYSCALL_MEMORY_ALLOCATE, (unsigned int )&data);
    return data.m_region_ptr;
}
