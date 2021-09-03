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
#include <stdarg.h>

/**
 * Clones a file descriptor in a process to another processes. Creates a new file
 * descriptor in the target process.
 *
 * @param sourceFd:         source file descriptor
 * @param sourcePid:        id of the source process
 * @param targetPid:        id of the target process
 * @return the resulting file descriptor
 *
 * @security-level APPLICATION
 */
File_t CloneFd(File_t sourceFd, Pid sourcePid, Pid targetpid)
{
	return CloneFdTS(sourceFd, sourcePid, -1, targetpid, 0);
}

/**
 * Clones a file descriptor in a process to another processes. Creates a new file
 * descriptor in the target process.
 *
 * @param sourceFd:         source file descriptor
 * @param sourcePid:        id of the source process
 * @param targetPid:        id of the target process
 * @param outStatus:   		is filled with the status code
 * @return the resulting file descriptor
 *
 * @security-level APPLICATION
 */
File_t CloneFdS(File_t sourceFd, Pid sourcePid, Pid targetpid, FsClonefdStatus *outStatus)
{
	return CloneFdTS(sourceFd, sourcePid, -1, targetpid, outStatus);
}

/**
 * Clones a file descriptor in a process to another processes file descriptor value.
 *
 * @param sourceFd:         source file descriptor
 * @param sourcePid:        id of the source process
 * @param targetFd:         target file descriptor
 * @param targetPid:        id of the target process
 * @return the target file descriptor
 *
 * @security-level APPLICATION
 */
File_t CloneFdT(File_t sourceFd, Pid sourcePid, File_t targetFd, Pid targetpid)
{
	return CloneFdTS(sourceFd, sourcePid, targetFd, targetpid, 0);
}

/**
 * Clones a file descriptor in a process to another processes file descriptor value.
 *
 * @param sourceFd:         source file descriptor
 * @param sourcePid:        id of the source process
 * @param targetFd:         target file descriptor
 * @param targetPid:        id of the target process
 * @param outStatus:    	is filled with the status code
 * @return the target file descriptor
 *
 * @security-level APPLICATION
 */
File_t CloneFdTS(File_t sourceFd, Pid sourcePid, File_t targetFd, Pid targetpid, FsClonefdStatus *outStatus)
{
	// prepare data
	SyscallFsClonefd data;
	data.sourceFd = sourceFd;
	data.sourcePid = sourcePid;
	data.targetFd = targetFd;
	data.targetPid = targetpid;

	// perform call
	syscall(SYSCALL_FS_CLONEFD, (uint32_t) &data);

	// fill and return status
	if (outStatus) *outStatus = data.status;
	return data.result;
}
