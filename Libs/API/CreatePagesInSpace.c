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
 * Creates pages in a process during process creation and maps the created pages in the
 * executing processes address space.
 *
 * @param process:          the process creation identifier
 * @param virtualAddress:   the address to map to
 * @param pages:            number of pages to map
 * @return the virtual address of the pages mapped to the executing processes address space
 *
 * @security-level KERNEL
 */
void *CreatePagesInSpaces(ProcessCreationIdentifier process, uint32_t virtualAddress, int numberOfPages)
{
	// prepare data
	SyscallCreatePagesInSpace data;
	data.processObject = process;
	data.targetSpaceVirtualAddress = virtualAddress;
	data.numberOfPages = numberOfPages;

	// performs call
	syscall(SYSCALL_CREATE_PAGES_IN_SPACE, (uint32_t) &data);

	// return the address
	return (void*) data.resultVirtualAddress;
}
