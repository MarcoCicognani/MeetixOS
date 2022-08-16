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

usize s_cli_args_release(char* buffer) {
    SyscallCliArgsRelease data{ buffer };
    do_syscall(SYSCALL_PROCESS_RELEASE_CLI_ARGUMENTS, (usize)&data);
    return data.m_len;
}
