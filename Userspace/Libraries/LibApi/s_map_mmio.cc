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

void* s_map_mmio(void* address, usize size) {
    SyscallMapMmio data{ address, size };
    do_syscall(SYSCALL_MEMORY_MAP_MMIO, (usize)&data);
    return data.m_mapped_ptr;
}
