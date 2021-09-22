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

void s_cli_args_store(ProcessCreationIdentifier process, char* arguments) {
    SyscallCliArgsStore data{ process, arguments };
    do_syscall(SYSCALL_PROCESS_STORE_CLI_ARGUMENTS, (usize)&data);
}
