/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
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

#ifndef EVA_API_CALLS_MEMORYCALLS
#define EVA_API_CALLS_MEMORYCALLS

#include "eva/stdint.h"
#include "eva/kernel.h"

__BEGIN_C

/**
 * @field size
 * 		the required size in bytes
 *
 * @field virtualResult
 * 		the virtual address of the allocated area in the current processes
 * 		address space. this address is page-aligned. if allocation
 * 		fails, this field is 0.
 *
 * @security-level APPLICATION
 */
typedef struct
{
	uint32_t size;

	void *virtualResult;
}__attribute__((packed)) SyscallAllocMem;

/**
 * @field memory
 * 		the memory area to share
 *
 * @field size
 *		the minimum size to share
 *
 * @field processId
 *		the id of the target process
 *
 * @field virtualAddress
 * 		the resulting page-aligned virtual address in the target
 * 		processes address space. if sharing fails, this field is 0.
 *
 * @security-level APPLICATION
 */
typedef struct
{
	void *memory;
	uint32_t size;
	Pid processID;

	void *virtualAddress;
}__attribute__((packed)) SyscallShareMem;

/**
 * @field physicalAddress
 * 		the page-aligned physical address of the area
 *
 * @field size
 * 		the minimum size to map
 *
 * @field virtualAddress
 * 		the resulting page-aligned virtual address in the current
 * 		processes address space. if mapping fails, this field is 0.
 *
 * @security-level DRIVER
 */
typedef struct
{
	void *physicalAddress;
	uint32_t size;

	void *virtualAddress;
}__attribute__((packed)) SyscallMapMmio;

/**
 * @field virtualBase
 * 		the address of the area to free
 *
 * @security-level APPLICATION
 */
typedef struct
{
	uint32_t virtualBase;
}__attribute__((packed)) SyscallUnmap;

/**
 * @field size
 * 		the size to allocate
 *
 * @field result
 * 		the resulting area address
 *
 * @security-level DRIVER
 */
typedef struct
{
	uint32_t size;

	void *result;
}__attribute__((packed)) SyscallLowerMalloc;

/**
 * @field memory
 * 		the memory to free
 *
 * @security-level DRIVER
 */
typedef struct
{
	void *memory;
}__attribute__((packed)) SyscallLowerFree;

/**
 * @field amount
 * 		the amount to increase/decrease the heap
 *
 * @field address
 * 		the previous heap break result (as expected by sbrk)
 *
 * @field successful
 * 		whether or not the call was successful
 *
 * @security-level APPLICATION
 */
typedef struct
{
	int32_t amount;

	uint32_t address;
	uint8_t successful;
}__attribute__((packed)) SyscallSbrk;

__END_C

#endif
