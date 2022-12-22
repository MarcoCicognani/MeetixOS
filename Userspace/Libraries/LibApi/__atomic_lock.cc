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

#include "__internal.hh"

#include <Api/User.h>

bool __atomic_lock(bool* atom1, bool* atom2, bool set_on_finish, bool try_only) {
    SyscallAtomicLock data{ atom1, atom2, set_on_finish, try_only, false };
    do_syscall(SYSCALL_LOCK_ATOMIC, (unsigned int)&data);
    return data.m_was_set;
}
