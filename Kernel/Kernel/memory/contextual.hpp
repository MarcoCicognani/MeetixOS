/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_SHARED_UTILS_ADDRESS_SPACE_BOUND
#define EVA_SHARED_UTILS_ADDRESS_SPACE_BOUND

#include "EvangelionNG.hpp"
#include "memory/AddressSpace.hpp"

/**
 * Safe wrapper for variables that are bound to a specific address space (context).
 * Is used when the kernel operates on multiple address spaces to make sure that
 * page directory switches are done properly and erroneous reads/writes don't occur.
 */
template<typename T>
class Contextual {
public:
    /**
     * constructor
     */
    Contextual() : value(0), space(0) {
    }

    /**
     * initialized constructor
     *
     * @param page:		the initial contextual page
     * @param space:	the current space
     */
    Contextual(T value, PageDirectory space) : value(value), space(space) {
    }

    /**
     * copy constructor
     *
     * @param rhs:		reference to an initialized object
     */
    Contextual(const Contextual& rhs) {
        this->value = rhs.value;
        this->space = rhs.space;
    }

    /**
     * copy operator
     *
     * @param rhs:		reference to an initialized object
     * @return the object update
     */
    Contextual& operator=(const Contextual& rhs) {
        this->value = rhs.value;
        this->space = rhs.space;
        return *this;
    }

    /**
     * execution operator
     *
     * @return the stored value
     */
    T operator()() const {
        if ( space != 0 ) {
            PageDirectory currentSpace = AddressSpace::getCurrentSpace();
            if ( currentSpace != space )
                EvaKernel::panic("%! tried to access a value from within another context",
                                 "contextual");
        }
        return value;
    }

    /**
     * set new values
     *
     * @param value:		new value
     * @param space:		new space
     */
    void set(T value, PageDirectory space) {
        this->value = value;
        this->space = space;
    }

private:
    /**
     * internal properties
     */
    T             value; // stored value
    PageDirectory space; // current space
};

#endif
