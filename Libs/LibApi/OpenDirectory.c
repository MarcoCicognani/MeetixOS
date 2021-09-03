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
#include <malloc.h>
#include <stdarg.h>

/**
 * Opens a directory.
 *
 * @param path:             path of the directory
 * @return pointer to a directory iterator, or nullptr otherwise
 *
 * @security-level APPLICATION
 */
FsDirectoryIterator *OpenDirectory(const char *path)
{
	return OpenDirectoryS(path, NULL);
}

/**
 * Opens a directory.
 *
 * @param path:             path of the directory
 * @param outStatus:    	is filled with the out status
 * @return pointer to a directory iterator, or nullptr otherwise
 *
 * @security-level APPLICATION
 */
FsDirectoryIterator *OpenDirectoryS(const char *path, FsOpenDirectoryStatus *outStatus)
{
	// create the iterator
	FsDirectoryIterator *iterator = (FsDirectoryIterator*) malloc(sizeof(FsDirectoryIterator));
	iterator->entryBuffer.name = (char*) malloc(FILENAME_MAX);

	// prepare the call data
	SyscallFsOpenDirectory data;
	data.path = (char*) path;
	data.iterator = iterator;

	// performs the call
	syscall(SYSCALL_FS_OPEN_DIRECTORY, (uint32_t) &data);

	// fill out and return iterator on success
	if (outStatus) *outStatus = data.status;
	if (data.status == FS_OPEN_DIRECTORY_SUCCESSFUL) return iterator;

	// destroy the iterator
	free(iterator);
	return NULL;
}
