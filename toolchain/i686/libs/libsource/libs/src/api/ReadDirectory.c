/*********************************************************************************
* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                  *
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
 * Reads the next entry of the directory.
 *
 * @param iterator:         the directory iterator
 * @return a directory entry structure, or nullptr otherwise
 *
 * @security-level APPLICATION
 */
FsDirectoryEntry *ReadDirectory(FsDirectoryIterator *iterator)
{
	return ReadDirectoryS(iterator, NULL);
}

/**
 * Reads the next entry of the directory.
 *
 * @param iterator:         the directory iterator
 * @param outStatus:    	is filled with the status code
 * @return a directory entry structure, or nullptr otherwise
 *
 * @security-level APPLICATION
 */
FsDirectoryEntry *ReadDirectoryS(FsDirectoryIterator *iterator, FsReadDirectoryStatus *outStatus)
{
	// prepare data
	SyscallFsReadDirectory data;
	data.iterator = iterator;

	// performs the call
	syscall(SYSCALL_FS_READ_DIRECTORY, (uint32_t) &data);

	// fill out and return entry buffer on success
	if (outStatus) *outStatus = data.status;
	if (data.status == FS_READ_DIRECTORY_SUCCESSFUL) return &iterator->entryBuffer;
	return NULL;
}
