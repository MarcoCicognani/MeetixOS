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
#include <string.h>

/**
 * Retrieves the length of a file in bytes.
 *
 * @param fd:                   the file descriptor
 * @param-opt outStatus:        is filled with the status code
 * @return the length in bytes
 *
 * @security-level APPLICATION
 */
int64_t Length(File_t fd)
{
	return LengthS(fd, 0);
}

/**
 * Retrieves the length of a file in bytes.
 *
 * @param fd:                   the file descriptor
 * @param outStatus:       	 	is filled with the status code
 * @return the length in bytes
 *
 * @security-level APPLICATION
 */
int64_t LengthS(File_t fd, FsLengthStatus *outStatus)
{
	// prepare the data
	SyscallFsLength data;
	data.mode = SYSCALL_FS_LENGTH_BY_FD;
	data.fd = fd;

	// performs the call
	syscall(SYSCALL_FS_LENGTH, (uint32_t) &data);

	// fill out and return
	if (outStatus) *outStatus = data.status;
	return data.length;
}

/**
 * Retrieves the length of a file in bytes.
 *
 * @param path:                     path of the file
 * @return the length in bytes
 *
 * @security-level APPLICATION
 */
int64_t Flength(const char *path)
{
	return FlengthSS(path, true, 0);
}

/**
 * Retrieves the length of a file in bytes.
 *
 * @param path:                     path of the file
 * @param followSymlinks:       	whether to follow symbolic links or not
 * @return the length in bytes
 *
 * @security-level APPLICATION
 */
int64_t FlengthS(const char *path, uint8_t followSymlinks)
{
	return FlengthSS(path, followSymlinks, 0);
}

/**
 * Retrieves the length of a file in bytes.
 *
 * @param path:                     path of the file
 * @param followSymlinks:      		whether to follow symbolic links or not
 * @param outStatus:           		is filled with the status code
 * @return the length in bytes
 *
 * @security-level APPLICATION
 */
int64_t FlengthSS(const char *path, uint8_t followSymlinks, FsLengthStatus *outStatus)
{
	// prepare data
	SyscallFsLength data;
	int symlinkFlag = (followSymlinks ? SYSCALL_FS_LENGTH_FOLLOW_SYMLINKS : SYSCALL_FS_LENGTH_NOT_FOLLOW_SYMLINKS);
	data.mode = symlinkFlag | SYSCALL_FS_LENGTH_BY_PATH;
	data.path = (char*) path;

	// performs the call
	syscall(SYSCALL_FS_LENGTH, (uint32_t) &data);

	// fill out and return
	if (outStatus) *outStatus = data.status;
	return data.length;
}
