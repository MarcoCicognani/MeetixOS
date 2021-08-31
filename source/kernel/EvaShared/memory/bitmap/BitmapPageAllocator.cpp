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

#include <memory/bitmap/BitmapPageAllocator.hpp>

#include <multiboot/multiboot.hpp>
#include <memory/paging.hpp>

#include <logger/logger.hpp>

/**
 * initializes the allocator from a bitmap begin address
 *
 * @param bitmapAddress:	the first index of the bitmap
 */
void BitMapPageAllocator::initialize(BitMapEntry *bitmapAddress)
{
	// copy pointer
	bitmap = bitmapAddress;

	logDebug("%! initializing to bitmap %h", "bitmap", bitmap);
	for (uint32_t i = 0; i < BITMAP_LENGTH; i++)
		bitmap[i] = 0;
	logDebug("%! map zeroed", "physical");
}

/**
 * Marks the given page-aligned address as free in the bitmap.
 *
 * @param address:		the address of the page to free
 */
void BitMapPageAllocator::markFree(uint32_t address)
{
	uint32_t index = ADDRESS_TO_BITMAP_INDEX(address);
	uint32_t bit = ADDRESS_TO_BITMAP_BIT(address);

	BITMAP_SET(bitmap, index, bit);
}

/**
 * Returns the next free page.
 *
 * @return address of a free page
 */
uint32_t BitMapPageAllocator::allocate()
{
	// parse all the bitmap length
	for (uint32_t i = 0; i < BITMAP_LENGTH; i++)
	{
		// If the entry is more than zero there is at least one free
		if (bitmap[i] > 0)
		{
			// parse each bit per entry
			for (uint32_t b = 0; b < BITMAP_BITS_PER_ENTRY; b++)
			{
				// If this bit is set, this page is free
				if (BITMAP_IS_SET(bitmap, i, b))
				{
					// Page found, clear if necessary and return
					BITMAP_UNSET(bitmap, i, b);
					return BITMAP_TO_ADDRESS(i, b);
				}
			}
		}
	}

	return 0;
}
