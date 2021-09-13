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

#ifndef EVA_MEMORY_PAGE_POOL
#define EVA_MEMORY_PAGE_POOL

#include "eva/stdint.h"

#include <memory/bitmap/bitmap.hpp>
#include <memory/bitmap/BitmapPageAllocator.hpp>
#include <memory/memory.hpp>
#include <memory/paging.hpp>

/**
 * Physical Page Allocator, this class manage allocation of physical addresses and memory pages
 */
class PPallocator {
public:
    /**
     * create ranges from bitmap ranges
     *
     * @param bitmapStart:		the start address
     * @param bitmapEnd:		the end address
     */
    static void initializeFromBitmap(PhysicalAddress bitmapStart, PhysicalAddress bitmapEnd);

    /**
     * allocate a new physical address and return it
     *
     * @return the new allocated physical address
     */
    static PhysicalAddress allocate();

    /**
     * free the provided address
     *
     * @param base:		the physical address to be freed
     */
    static void free(PhysicalAddress base);

    /**
     * get the physical memory size
     *
     * @return avaible pages
     */
    static uint32_t getInitialAmount();

    /**
     * get the free pages Count
     *
     * @return the page free count
     */
    static uint32_t getFreePageCount();
};

#endif
