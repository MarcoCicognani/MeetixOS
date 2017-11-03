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
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

/**
 * Opens a file.
 *
 * @param path:            the path to the file
 * @return the file descriptor for the opened file
 *
 * @security-level APPLICATION
 */
File_t Open(const char *name)
{
	return OpenFMS(name, 0, 0, NULL);
}

/**
 * Opens a file.
 *
 * @param path:             the path to the file
 * @param flags:			the flags for open mode
 * @return the file descriptor for the opened file
 *
 * @security-level APPLICATION
 */
File_t OpenF(const char *name, int32_t flags)
{
	return OpenFMS(name, flags, 0, NULL);
}

/**
 * Opens a file.
 *
 * @param path:             the path to the file
 * @param flags:       		the flags for open mode
 * @param outStatus:   		filled with one of the {FsOpenStatus} codes
 * @return the file descriptor for the opened file
 *
 * @security-level APPLICATION
 */
File_t OpenFS(const char *name, int32_t flags, FsOpenStatus *outStatus)
{
	return OpenFMS(name, flags, 0, outStatus);
}

/**
 * Opens a file.
 *
 * @param path:             the path to the file
 * @param flags:       		the flags for open mode
 * @param mode:        		the mode that kernel use to open the file
 * @param outStatus:   		filled with one of the {FsOpenStatus} codes
 * @return the file descriptor for the opened file
 *
 * @security-level APPLICATION
 */
File_t OpenFMS(const char *name, int32_t flags, int32_t mode, FsOpenStatus *outStatus)
{
	// prepare data
	SyscallFsOpen data;
	data.path = (char*) name;
	data.mode = mode;
	data.flags = flags;

	// performs the call
	syscall(SYSCALL_FS_OPEN, (uint32_t) &data);

	// fill out and return
	if (outStatus) *outStatus = data.status;
	return data.fd;
}
