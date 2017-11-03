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
 * Creates a thread-local-storage area for a process and copies/zeroes the given amount of bytes
 * from the content.
 *
 * @param process:      the process creation identifier
 * @param content:      master copy content
 * @param copysize:     number of bytes to copy from content
 * @param zerosize:     total size (including bytes to zero)
 * @param alignment:    tls alignment
 * @return whether writing was successful
 *
 * @security-level KERNEL
 */
uint8_t WriteTlsMasterForProcess(ProcessCreationIdentifier process, uint8_t *content, uint32_t copysize, uint32_t totalsize, uint32_t alignment)
{
	// prepare data
	SyscallWriteTlsMasterForProcess data;
	data.processObject = process;
	data.content = content;
	data.copysize = copysize;
	data.totalsize = totalsize;
	data.alignment = alignment;

	// performs the call
	syscall(SYSCALL_WRITE_TLS_MASTER_FOR_PROCESS, (uint32_t) &data);

	// return success flag
	return data.result;
}
