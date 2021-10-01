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

#pragma once

#include <Tasking/Lock.hh>

namespace Tasking {

/* ------------------------------------------ C++ types ----------------------------------------- */

/**
 * @brief Scoped managed <Lock>
 */
class LockGuard {
public:
    LockGuard(Lock& lock) : m_lock{ lock } {
        m_lock.lock();
    }

    ~LockGuard() {
        m_lock.unlock();
    }

private:
    Lock& m_lock;
};

} /* namespace Tasking */