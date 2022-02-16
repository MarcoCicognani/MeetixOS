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

usize s_get_thread_ids(Tid* buffer, usize buffer_len) {
    return s_get_thread_ids_t(buffer, buffer_len, THREAD_TYPE_MAIN | THREAD_TYPE_VM86 | THREAD_TYPE_SUB);
}

usize s_get_thread_ids_t(Tid* buffer, usize buffer_len, ThreadType thread_type) {
    if ( buffer && buffer_len > 0 ) {
        SyscallGetThreadIDs data{ buffer, thread_type, buffer_len };
        do_syscall(SYSCALL_GET_THREAD_IDS_LIST, (usize)&data);

        return data.m_stored_ids_count;
    }
    return 0;
}