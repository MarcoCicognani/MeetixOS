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

#include <memory/collections/AddressStack.hpp>
#include <memory/KernelHeap.hpp>
#include <logger/logger.hpp>
#include <EvangelionNG.hpp>

/**
 * push a new address into the stack
 *
 * @param address:		the new address to add to stack
 */
void AddressStack::push(Address address)
{
	// first ever? create current
	if (!current) current = new AddressStackFrame();

	// put address on frame
	current->entries[position++] = address;

	// create new frame when at the end
	if (position == ADDRESS_STACK_FRAME_ENTRIES)
	{
		current->next = new AddressStackFrame();
		current->next->previous = current;
		current = current->next;
		position = 0;
	}
}

/**
 * remove the last address inserted
 *
 * @return:		the last added address
 */
Address AddressStack::pop()
{
	// if no frames, return nothing
	if (!current) return 0;

	// if at foot of frame, try to jump back
	if (!position)
	{
		// return nothing if no previous frame
		if (!current->previous) return 0;

		// remove frame
		current = current->previous;
		delete current->next;
		current->next = 0;

		position = ADDRESS_STACK_FRAME_ENTRIES;
	}

	return current->entries[--position];
}
