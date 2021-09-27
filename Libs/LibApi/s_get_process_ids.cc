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

usize s_get_process_ids(Pid* buffer, usize buffer_len) {
    if ( buffer && buffer_len > 0 ) {
        SyscallGetProcessIDs data{ buffer, buffer_len };
        do_syscall(SYSCALL_GET_PROCESS_IDS_LIST, (usize)&data);

        return data.m_stored_ids_count;
    }
    return 0;
}