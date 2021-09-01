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

#ifndef EVA_SHARED_MEMORY_BITMAPPHYSICALPAGEALLOCATOR
#define EVA_SHARED_MEMORY_BITMAPPHYSICALPAGEALLOCATOR

#include "eva/stdint.h"
#include <memory/bitmap/bitmap.hpp>

/**
 * Physical page allocator implementation using a bitmap
 */
class BitMapPageAllocator
{
private:
	/**
	 * internal data
	 */
	BitMapEntry *bitmap; // pointer to a vector of bitmaps

public:

	/**
	 * initializes the allocator from a bitmap begin address
	 *
	 * @param bitmapAddress:	the first index of the bitmap
	 */
	void initialize(BitMapEntry *bitmapAddress);

	/**
	 * Marks the given page-aligned address as free in the bitmap.
	 *
	 * @param address:		the address of the page to free
	 */
	void markFree(uint32_t address);

	/**
	 * Returns the next free page.
	 *
	 * @return address of a free page
	 */
	uint32_t allocate();
};

#endif
