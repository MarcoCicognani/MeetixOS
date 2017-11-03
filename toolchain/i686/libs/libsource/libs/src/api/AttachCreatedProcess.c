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
 * Attaches and therefore starts a process during process creation at the given execution point.
 *
 * @param process:      the process creation identifier
 * @param eip:          execution entry point
 *
 * @security-level KERNEL
 */
void AttachCreatedProcess(ProcessCreationIdentifier process, uint32_t eip)
{
	// create call data
	SyscallAttachCreatedProcess data;
	data.eip = eip;
	data.processObject = process;

	// perform syscall
	syscall(SYSCALL_ATTACH_CREATED_PROCESS, (uint32_t) &data);
}
