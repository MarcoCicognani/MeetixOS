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

bool s_write_tls_master_for_process(ProcessCreationIdentifier process,
                                    u8*                       content,
                                    usize                     copy_size,
                                    usize                     total_size,
                                    usize                     alignment) {
    SyscallWriteTlsMasterForProcess data{ process, content, copy_size, total_size, alignment };
    do_syscall(SYSCALL_WRITE_TLS_MASTER_FOR_PROCESS, (usize)&data);
    return data.m_success;
}
