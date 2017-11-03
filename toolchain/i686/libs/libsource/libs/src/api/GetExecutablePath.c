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
 * Retrieves the directory of the executable when available, otherwise an empty
 * string is written to the buffer.
 *
 * @param path:     buffer of at least {PATH_MAX} bytes size
 *
 * @security-level APPLICATION
 */
void GetExecutablePath(char *buffer)
{
	// prepare data
	SyscallFsGetExecutablePath data;
	data.buffer = buffer;

	// perform the call
	syscall(SYSCALL_GET_EXECUTABLE_PATH, (uint32_t) &data);
}
