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
 * Repositions the offset within a file.
 *
 * @param fd:               the file descriptor
 * @param off:              the offset
 * @param whence:           one of the {FsSeekMode} constants
 * @param-opt outStatus:    is filled with the status
 * @return if successful returns the new absolute offset, otherwise -1
 *
 * @security-level APPLICATION
 */
int64_t Seek(File_t fd, int64_t off, FsSeekMode mode)
{
	return SeekS(fd, off, mode, NULL);
}

/**
 * Repositions the offset within a file.
 *
 * @param fd:               the file descriptor
 * @param off:              the offset
 * @param whence:           one of the {FsSeekMode} constants
 * @param outStatus:    	is filled with the status
 * @return if successful returns the new absolute offset, otherwise -1
 *
 * @security-level APPLICATION
 */
int64_t SeekS(File_t fd, int64_t off, FsSeekMode mode, FsSeekStatus *outStatus)
{
	// prepare data
	SyscallFsSeek data;
	data.fd = fd;
	data.amount = off;
	data.mode = mode;

	// performs the call
	syscall(SYSCALL_FS_SEEK, (uint32_t) &data);

	// fill out and return
	if (outStatus) *outStatus = data.status;
	return data.result;

}
