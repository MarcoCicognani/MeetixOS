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
 * Creates an empty process.
 *
 * @param securityLevel:    the security level to use
 * @return the process creation identifier for the created process
 *
 * @security-level KERNEL
 */
ProcessCreationIdentifier CreateEmptyProcess(SecurityLevel securityLevel)
{
	// prepare data
	SyscallCreateEmptyProcess data;
	data.securityLevel = securityLevel;

	// perform call
	syscall(SYSCALL_CREATE_EMPTY_PROCESS, (uint32_t) &data);

	// return the result
	return data.processObject;
}
