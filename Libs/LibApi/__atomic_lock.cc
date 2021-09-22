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

bool __atomic_lock(u8* atom1, u8* atom2, bool set_on_finish, bool try_only) {
    SyscallAtomicLock data{ atom1, atom2, set_on_finish, try_only, false };
    do_syscall(SYSCALL_LOCK_ATOMIC, (usize)&data);
    return data.m_was_set;
}
