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

void s_atomic_lock(u8* atom) {
    __atomic_lock(atom, NULL, true, false);
}

void s_atomic_lock_dual(u8* atom1, u8* atom2) {
    __atomic_lock(atom1, atom2, true, false);
}
