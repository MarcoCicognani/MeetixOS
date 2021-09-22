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

#include "debug/DebugInterfaceKernel.hpp"

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <memory/physical/PPallocator.hpp>

/**
 * memory informations
 */
static uint32_t freePageCount = 0; // number of free memory pages
static uint32_t initialAmount = 0; // amount of memory

static BitMapEntry         bitmap[BITMAP_LENGTH];
static BitMapPageAllocator physicalAllocator;

/**
 * create ranges from bitmap ranges
 *
 * @param bitmapStart:		the start address
 * @param bitmapEnd:		the end address
 */
void PPallocator::initializeFromBitmap(PhysAddr bitmapStart, PhysAddr bitmapEnd) {
    // Check bitmap length
    logDebug("%! checking bitmap", "ppa");
    uint32_t bitmapBytes         = bitmapEnd - bitmapStart;
    uint32_t bitmapExpectedBytes = BITMAP_LENGTH * sizeof(BitMapEntry);

    if ( bitmapBytes == bitmapExpectedBytes ) {
        logDebug("%! bitmap check successful", "ppa");
    }

    else
        EvaKernel::panic("%! bitmap has wrong length", "ppa");

    // Copy the bitmap and count the pages
    logDebug("%! copying bitmap", "ppa");

    physicalAllocator.initialize(bitmap);
    Memory::copy(bitmap, (void*)bitmapStart, BITMAP_SIZE);

    // Count free pages
    for ( uint32_t i = 0; i < BITMAP_LENGTH; i++ )
        for ( uint8_t b = 0; b < BITMAP_BITS_PER_ENTRY; b++ )
            if ( BITMAP_IS_SET(bitmap, i, b) )
                ++freePageCount;

    // first paging is max amount of ram;
    initialAmount = freePageCount;
    DEBUG_INTERFACE_SYSTEM_INFORMATION("memory.freePageCount", freePageCount);
    logDebug("%! bitmap analyzed, got %i free pages", "ppa", freePageCount);
}

/**
 * allocate a new physical address and return it
 *
 * @return the new allocated physical address
 */
PhysAddr PPallocator::allocate() {
    PhysAddr page = physicalAllocator.allocate();
    if ( !page ) {
        logInfo("%! critical: physical page allocator has no pages left", "ppa");
        EvaKernel::panic("%! out of physical memory", "ppa");
    }

    --freePageCount;

    DEBUG_INTERFACE_MEMORY_SET_PAGE_USAGE(page, 1);
    return page;
}

/**
 * free the provided address
 *
 * @param base:		the physical address to be freed
 */
void PPallocator::free(PhysAddr page) {
    physicalAllocator.markFree(page);
    ++freePageCount;

    DEBUG_INTERFACE_MEMORY_SET_PAGE_USAGE(page, 0);
}

/**
 * get the physical memory size
 *
 * @return avaible pages
 */
uint32_t PPallocator::getInitialAmount() {
    return initialAmount;
}
/**
 * get the free pages Count
 *
 * @return the page free count
 */
uint32_t PPallocator::getFreePageCount() {
    return freePageCount;
}
