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

bool s_get_process_descriptor(Pid proc_id, ProcessDescriptor* descriptor) {
    if ( descriptor ) {
        descriptor->m_main_thread.m_tid = proc_id;
        do_syscall(SYSCALL_GET_PROCESS_DESCRIPTOR, (usize)descriptor);

        return descriptor->m_main_thread.found;
    }
    return false;
}