/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef MEETIX_LIBRARY_TASKING_LOCK
#define MEETIX_LIBRARY_TASKING_LOCK

#include <eva.h>

/**
 * class that permits to create an high level locker
 */
class Lock {
protected:
    uint8_t locked{ false }; // internal locker flag

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
        AtomicLock(&locked);
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

#endif
