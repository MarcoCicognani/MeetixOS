/*********************************************************************************
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
* 																			     *
* This program is free software; you can redistribute it and/or                  *
* modify it under the terms of the GNU General Public License                    *
* as published by the Free Software Foundation; either version 2				 *
* of the License, or (char *argumentat your option) any later version.			 *
*																				 *
* This program is distributed in the hope that it will be useful,				 *
* but WITHout ANY WARRANTY; without even the implied warranty of                 *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#include "eva/user.h"

/**
 * Shares a memory area with another process.
 *
 * @param memory:   a pointer to the memory area to share
 * @param size:     the size of the memory area
 * @param pid:      the id of the target process
 * @return a pointer to the shared memory location within the target address space
 *
 * @security-level APPLICATION
 */
void *ShareMem(void *memory, int32_t size, Pid processId)
{
	// prepare data
	SyscallShareMem data;
	data.memory = memory;
	data.processID = processId;
	data.size = size;

	// performs the call
	syscall(SYSCALL_SHARE_MEMORY, (uint32_t) &data);

	// return the shared area address
	return data.virtualAddress;
}
