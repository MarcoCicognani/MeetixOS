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

#include <Api.h>

/**
 * class that permits to create an high level locker
 */
class Lock {
protected:
    bool locked{ false }; // internal locker flag

public:
    /**
     * empty constructor
     */
    Lock() = default;

    /**
     * destructor
     */
    virtual ~Lock() = default;

    /**
     * perform the lock
     */
    virtual void lock() {
        s_atomic_lock(&locked);
    }

    /**
     * manually unlock the locker
     */
    virtual void unlock() {
        locked = false;
    }

    /**
     * return the locker flag
     */
    bool isLocked() const {
        return locked;
    }
};
