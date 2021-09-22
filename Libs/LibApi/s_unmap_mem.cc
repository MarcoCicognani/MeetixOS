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

void s_unmap_mem(void* area) {
    SyscallUnmap data{ reinterpret_cast<Address>(area) };
    do_syscall(SYSCALL_MEMORY_UNMAP, (usize)&data);
}
