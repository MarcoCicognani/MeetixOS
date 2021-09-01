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

#include <memory/LowerHeap.hpp>
#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <memory/allocators/ChunkAllocator.hpp>

/**
 * container for allocated addresses
 */
static ChunkAllocator allocator;

/**
 * create allocation area for the lower heap
 *
 * @param start:		the first index of the area
 * @param end:			the end index of the area
 */
void LowerHeap::addArea(uint32_t start, uint32_t end)
{
	allocator.initialize(start, end);
	logDebug("%! using area %h to %h", "lowermem", start, end);
}

/**
 * allocate memory on the lower heap of provided size
 *
 * @param size:		the size to be allocated
 * @return the index of the allocated Chunk
 */
void *LowerHeap::allocate(int32_t size)
{
	void *allocated = allocator.allocate(size);
	if (allocated) return allocated;

	logInfo("%! out of memory", "loweralloc");
	return 0;
}

/**
 * frees a memory area
 *
 * @param memory:		pointer to the memory to deallocate
 */
void LowerHeap::free(void *mem)
{
	allocator.free(mem);
}
