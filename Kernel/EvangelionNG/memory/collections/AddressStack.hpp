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

#ifndef EVA_MEMORY_ADDRESS_STACK
#define EVA_MEMORY_ADDRESS_STACK

#include "eva/stdint.h"

#include <memory/memory.hpp>
#include <memory/paging.hpp>

/**
 * macro, address for single stack frame
 */
#define ADDRESS_STACK_FRAME_ENTRIES 128

/**
 * descriptor of stack frame
 */
struct AddressStackFrame {
public:
    /**
     * linkage addresses
     */
    AddressStackFrame* next     = 0;
    AddressStackFrame* previous = 0;

    /**
     * address storable for single frame
     */
    Address entries[ADDRESS_STACK_FRAME_ENTRIES];
};

/**
 * class to manage simple linkage list of memory addresses
 */
class AddressStack {
private:
    /**
     * internal properties
     */
    AddressStackFrame* current;  // current frame
    uint32_t           position; // position of current frame on list

public:
    /**
     * constructor
     */
    AddressStack() : current(0), position(0) {
    }

    /**
     * push a new address into the stack
     *
     * @param address:		the new address to add to stack
     */
    void push(Address address);

    /**
     * remove the last address inserted
     *
     * @return:		the last added address
     */
    Address pop();
};

#endif
