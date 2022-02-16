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

void s_lower_free(void* memory) {
    SyscallLowerFree data{ memory };
    do_syscall(SYSCALL_MEMORY_LOWER_FREE, (usize)&data);
}
