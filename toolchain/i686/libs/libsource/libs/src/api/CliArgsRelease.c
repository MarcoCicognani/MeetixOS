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
 * Returns and releases the command line arguments for the executing process.
 * This buffer must have a length of at least {PROCESS_COMMAND_LINE_ARGUMENTS_BUFFER_LENGTH} bytes.
 * If no arguments were supplied for the executing process, the buffer is null-terminated only.
 *
 * @param buffer:       target buffer to store the arguments to
 *
 * @security-level KERNEL
 */
void CliArgsRelease(char *buffer)
{
	// prepare data
	SyscallCliArgsRelease data;
	data.buffer = buffer;

	// perform call
	syscall(SYSCALL_RELEASE_CLI_ARGUMENTS, (uint32_t) &data);
}
