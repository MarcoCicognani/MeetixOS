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

#include <Api.h>

void s_get_task_identifier(Tid thread_id, char* out) {
    SyscallGetIdentifier data{ thread_id };
    do_syscall(SYSCALL_THREAD_GET_NAME, (usize)&data);

    usize len = string_len(data.m_thread_name);
    if ( len > 0 )
        memory_copy(out, data.m_thread_name, len);
}
