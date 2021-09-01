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

#include "eva.h"

/**
 * Closes a file.
 *
 * @param fd:   the file descriptor to close
 * @return one of the {FsCloseStatus} codes
 *
 * @security-level APPLICATION
 */
FsCloseStatus Close(File_t file)
{
	// fill data
	SyscallFsClose data;
	data.fd = file;

	// perform call
	syscall(SYSCALL_FS_CLOSE, (uint32_t) &data);

	// return status
	return data.status;
}
