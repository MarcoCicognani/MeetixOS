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
 * Reads bytes from a file on the ramdisk.
 *
 * @param nodeID:       ramdisk node id
 * @param offset:       offset within the file
 * @param buffer:       target buffer
 * @param length:       number of bytes to read
 * @return if successful the number of bytes read, otherwise -1
 *
 * @security-level APPLICATION
 */
int RamdiskRead(int node, uint32_t offset, char* buffer, uint32_t length)
{
	// prepapre the data
	SyscallRamdiskRead data;
	data.nodeID = node;
	data.buffer = (char*) buffer;
	data.offset = offset;
	data.length = length;

	// performs the call
	syscall(SYSCALL_RAMDISK_READ, (uint32_t) &data);

	// return the readed bytes count
	return data.readBytes;
}
