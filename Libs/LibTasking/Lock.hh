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

#pragma once

#include <Api.h>

namespace Tasking {

/* ------------------------------------------ C++ types ----------------------------------------- */

/**
 * @brief C++ wrapper for low-level <s_atomic_lock()> system calls
 */
class Lock {
public:
    /**
     * @brief Constructor and destructor
     */
    Lock() = default;
    ~Lock() { unlock(); }

    /**
     * @brief Locks the critical region
     */
    void lock();

    /**
     * @brief Tries to lock the critical region and returns <true> on success
     */
    bool try_lock();

    /**
     * @brief Manually unlocks the critical region
     */
    void unlock();

    /**
     * @brief Returns whether the lock is in a locked state
     */
    [[nodiscard]] bool is_locked() const { return m_lock_atom; }

private:
    bool m_lock_atom{ false };
};

} /* namespace Tasking */