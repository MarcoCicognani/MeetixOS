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

ProcessCreationIdentifier s_create_empty_process(SecurityLevel security_level) {
    SyscallCreateEmptyProcess data{ security_level };
    do_syscall(SYSCALL_PROCESS_CREATE_EMPTY, (usize)&data);
    return data.m_process_creation_identifier;
}
