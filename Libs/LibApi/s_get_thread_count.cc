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

usize s_get_thread_count() {
    return s_get_thread_count_t(THREAD_TYPE_MAIN | THREAD_TYPE_VM86 | THREAD_TYPE_SUB);
}

usize s_get_thread_count_t(ThreadType thread_type) {
    SyscallGetThreadCount data{ thread_type };
    do_syscall(SYSCALL_GET_THREAD_COUNT, (usize)&data);
    return data.m_count;
}