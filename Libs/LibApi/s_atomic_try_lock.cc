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

bool s_atomic_try_lock(bool* atom) {
    return __atomic_lock(atom, nullptr, true, true);
}

bool s_atomic_try_lock_dual(bool* atom1, bool* atom2) {
    return __atomic_lock(atom1, atom2, true, true);
}
