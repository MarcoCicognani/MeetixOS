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
 * Sets the working directory for the current process.
 *
 * @param path:         buffer of at least {PATH_MAX} bytes size containing the new working directory
 * @return one of the {SetWorkingDirectoryStatus} codes
 *
 * @security-level APPLICATION if no process given, otherwise KERNEL
 */
SetWorkingDirectoryStatus SetWorkingDirectory(const char *path)
{
	SetWorkingDirectoryP(path, NULL);
}

/**
 * Sets the working directory for the current process.
 *
 * @param path:         buffer of at least {PATH_MAX} bytes size containing the new working directory
 * @param process:      process to set working directory for during spawning
 * @return one of the {SetWorkingDirectoryStatus} codes
 *
 * @security-level APPLICATION if no process given, otherwise KERNEL
 */
SetWorkingDirectoryStatus SetWorkingDirectoryP(const char *path, ProcessCreationIdentifier process)
{
	// prepare data
	SyscallFsSetWorkingDirectory data;
	data.path = (char*) path;
	data.process = process;

	// performs the call
	syscall(SYSCALL_SET_WORKING_DIRECTORY, (uint32_t) &data);

	// return the result
	return data.result;
}
