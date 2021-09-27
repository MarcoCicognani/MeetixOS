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

bool s_task_register_id(const char* new_identifier) {
    return s_task_register_ids(new_identifier, nullptr);
}

bool s_task_register_ids(const char* new_identifier, char* selected) {
    SyscallTaskIDRegister data = { .m_identifier = new_identifier };
    do_syscall(SYSCALL_THREAD_SET_NAME, (usize)&data);

    if ( !data.m_success && selected )
        memory_copy(selected, data.m_selected_identifier, string_len(data.m_selected_identifier));
    return data.m_success;
}
