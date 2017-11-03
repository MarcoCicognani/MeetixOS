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

/**
 * Retrieves the current offset within a file.
 *
 * @param fd:               the file descriptor
 * @return if successful returns the current absolute offset, otherwise -1
 *
 * @security-level APPLICATION
 */
int64_t Tell(File_t fd)
{
	return TellS(fd, NULL);
}

/**
 *
 */
int64_t TellS(File_t fd, FsTellStatus *outStatus)
{
	// prepapre data
	SyscallFsTell data;
	data.fd = fd;

	// performs the call
	syscall(SYSCALL_FS_TELL, (uint32_t) &data);

	// fill out and return
	if (outStatus) *outStatus = data.status;
	return data.result;
}
