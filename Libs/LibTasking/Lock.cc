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

#include <LibTasking/Lock.hh>

namespace Tasking {

void Lock::lock() {
    s_atomic_lock(&m_lock_atom);
}

bool Lock::try_lock() {
    return s_atomic_try_lock(&m_lock_atom);
}

void Lock::unlock() {
    m_lock_atom = false;
}

} /* namespace Tasking */