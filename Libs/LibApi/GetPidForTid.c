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
 * Retrieves the process id for a thread id.
 *
 * @param tid the thread id
 * @return the process id of the thread, or -1
 *
 * @security-level APPLICATION
 */
uint32_t GetPidForTid(uint32_t tid)
{
	// prepare the data
	SyscallGetPidForTid data;
	data.tid = tid;

	// performs the call
	syscall(SYSCALL_GET_PROCESS_ID_FOR_THREAD_ID, (uint32_t) &data);

	// return the pid
	return data.pid;
}
