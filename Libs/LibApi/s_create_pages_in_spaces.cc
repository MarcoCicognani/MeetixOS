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

void* s_create_pages_in_spaces(ProcessCreationIdentifier process,
                               Address                   virtual_address,
                               usize                     pages_count) {
    SyscallCreatePagesInSpace data{ process, virtual_address, pages_count };
    do_syscall(SYSCALL_CREATE_PAGES_IN_SPACE, (usize)&data);
    return (void*)data.m_shared_virtual_ptr;
}
