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
 * Returns the number of children of a ramdisk entry.
 *
 * @param nodeID:   ramdisk node id
 * @return the number of children
 *
 * @security-level APPLICATION
 */
int RamdiskChildCount(int node)
{
	// prepare data
	SyscallRamdiskChildCount data;
	data.nodeID = node;

	// performs call
	syscall(SYSCALL_RAMDISK_CHILD_COUNT, (uint32_t) &data);

	// return the result
	return data.count;
}
