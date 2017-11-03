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
 * Call the kernel to store on <buffer>, 
 * of size <length> the tids of the threads
 *
 * @param buffer:		an userspace buffer of integers
 * @param length:		the size of the buffer, use the <GetThreadCount> call to know the size
 * @return the index of the last copied element
 */
uint32_t GetThreadTids(Tid *buffer, uint32_t length)
{
	return GetThreadTidsT(buffer, length, THREAD_TYPE_MAIN | THREAD_TYPE_VM86 | THREAD_TYPE_SUB);
}

/**
 * Call the kernel to store on <buffer>, 
 * of size <length> the tids of the threads
 *
 * @param buffer:		an userspace buffer of integers
 * @param length:		the size of the buffer, use the <GetThreadCount> call to know the size
 * @param type:			the type of thread to get
 * @return the index of the last copied element
 */
uint32_t GetThreadTidsT(Tid *buffer, uint32_t length, ThreadType type)
{
	// check buffer and length validity
	if (buffer && length > 0)
	{
		// create data
		SyscallGetThreadIDs data;
		data.type = type;
		data.idBuffer = buffer;
		data.idBufferSize = length;

		// perform the call
		syscall(SYSCALL_GET_THREAD_IDS_LIST, (uint32_t) &data);
		return data.filledIds;
	}

	return 0;
}