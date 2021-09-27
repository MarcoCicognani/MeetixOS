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

void s_configure_process(ProcessCreationIdentifier process, ProcessConfiguration configuration) {
    SyscallConfigureProcess data{ process, configuration };
    do_syscall(SYSCALL_PROCESS_CONFIGURE, (usize)&data);
}
