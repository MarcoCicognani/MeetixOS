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

#include "__internal.hh"

#include <Api/User.h>

void s_exit(int exit_code) {
    SyscallExit data{ exit_code };
    do_syscall(SYSCALL_THREAD_EXIT, (usize)&data);
    __builtin_unreachable();
}

void __exit_thread() {
    do_syscall(SYSCALL_EXIT_THREAD, 0);
    __builtin_unreachable();
}
