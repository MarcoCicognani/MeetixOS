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

#ifndef CHUNKALLOCATOR_HPP_
#define CHUNKALLOCATOR_HPP_

#include "eva/stdint.h"
#include "eva/types.h"
#include <logger/logger.hpp>

/**
 * minimum size is a byte
 */
#define CHUNK_ALLOCATOR_MINIMUM_ALLOCATION_SIZE	8

/**
 * chunck descriptor
 */
struct ChunkHeader
{
	ChunkHeader *next; 	// next chunck node
	uint8_t used;		// flag that determines if the node is busy
	uint32_t size;		// size of the node
};

/**
 * this class provide virtual memory managing
 * with an old conception of memory format, infact it divides
 * the memory with an array of linked structres
 */
class ChunkAllocator
{
private:
	/**
	 * internal data
	 */
	ChunkHeader *first; // first node of memory allocated

	/**
	 * Merges contiguous free blocks
	 */
	void merge();

public:
	/**
	 * constructor
	 */
	ChunkAllocator() : first(0) {}

	/**
	 * initialize the memory allocator from range addresses
	 *
	 * @param start:	the starting address
	 * @param end:		the ending address
	 */
	void initialize(VirtualAddress start, VirtualAddress end);

	/**
	 * expands the size of memory managed by the object
	 *
	 * @param size:		the size of the expansion
	 */
	void expand(VirtualAddress size);

	/**
	 * allocate a size of memory on space managed by the object
	 *
	 * @param size:		the size of the memory to allocate
	 * @return the allocated pointer or 0 on fails
	 */
	void *allocate(uint32_t size);

	/**
	 * frees a memory pointer
	 *
	 * @param memory:		the pointer that have to be deallocated
	 * @return the freed size
	 */
	uint32_t free(void *memory);

};

#endif
