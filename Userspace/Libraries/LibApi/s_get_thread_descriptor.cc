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

bool s_get_thread_descriptor(Tid thread_id, ThreadDescriptor* descriptor) {
    if ( descriptor ) {
        descriptor->m_tid = thread_id;
        do_syscall(SYSCALL_GET_THREAD_DESCRIPTOR, (usize)descriptor);

        return descriptor->found;
    }
    return false;
}