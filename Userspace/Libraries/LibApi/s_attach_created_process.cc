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

void s_attach_created_process(ProcessCreationIdentifier process, Address eip) {
    SyscallAttachCreatedProcess data{ eip, process };
    do_syscall(SYSCALL_ATTACH_CREATED_PROCESS, (unsigned int)&data);
}
