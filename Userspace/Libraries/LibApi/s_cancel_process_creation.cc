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

void s_cancel_process_creation(ProcessCreationIdentifier process) {
    SyscallCancelProcessCreation data{ process };
    do_syscall(SYSCALL_CANCEL_PROCESS_CREATION, (usize)&data);
}
