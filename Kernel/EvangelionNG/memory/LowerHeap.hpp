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

#ifndef MEMORY_LOWERMEM_ALLOC
#define MEMORY_LOWERMEM_ALLOC

#include "eva/stdint.h"

#include <stddef.h>

/**
 * lower heap Chunk descriptor
 */
struct LowerHeapHeader {
    LowerHeapHeader* next;
    uint8_t          used;
    uint32_t         size;
};

/**
 * class to manage lower heap allocation
 */
class LowerHeap {
public:
    /**
     * create allocation area for the lower heap
     *
     * @param start:		the first index of the area
     * @param end:			the end index of the area
     */
    static void addArea(uint32_t start, uint32_t end);

    /**
     * allocate memory on the lower heap of provided size
     *
     * @param size:		the size to be allocated
     * @return the index of the allocated Chunk
     */
    static void* allocate(int32_t size);

    /**
     * frees a memory area
     *
     * @param memory:		pointer to the memory to deallocate
     */
    static void free(void* memory);
};

#endif
