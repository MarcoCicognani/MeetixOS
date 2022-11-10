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

bool s_set_break(isize amount, void** out_break) {
    SyscallSbrk data{ amount };
    do_syscall(SYSCALL_MEMORY_SBRK, (usize)&data);

    if ( out_break )
        *out_break = reinterpret_cast<void*>(data.m_out_address);
    return data.m_success;
}
