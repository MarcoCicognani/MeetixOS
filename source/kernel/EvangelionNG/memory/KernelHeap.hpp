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

#ifndef MEMORY_EVA_HEAP_ALLOCATOR
#define MEMORY_EVA_HEAP_ALLOCATOR

#include <stddef.h>
#include "eva/stdint.h"
#include <memory/paging.hpp>
#include <memory/memory.hpp>

// 1 MiB
#define KERNEL_HEAP_EXPAND_STEP	0x100000

/**
 * The kernel heap is the kernel space memory manager. It is initialized
 * on startup and is used in all calls to new/delete within the kernel.
 */
class KernelHeap
{
public:

	/**
	 * Initializes the kernel heap using the given range of memory.
	 *
	 * @param start:	the start address
	 * @param end:		the end address
	 */
	static void initialize(VirtualAddress start, VirtualAddress end);

	/**
	 * Expands the heap space by {KERNEL_HEAP_EXPAND_STEP} bytes.
	 *
	 * @return whether the operation was successful
	 */
	static bool expandHeap();

	/**
	 * allocate a pointer of size on the kernel heap
	 *
	 * @param size:		size of the space to be allocated
	 * @return the allocated pointer
	 */
	static void *allocate(uint32_t size);

	/**
	 * frees a memory pointer
	 *
	 * @param memory:		the pointer that be deallocated
	 */
	static void free(void *memory);

	/**
	 * @return the amount of used heap memory
	 */
	static uint32_t getUsedMemoryAmount();
};

#endif
