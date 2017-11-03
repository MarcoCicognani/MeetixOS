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
 * Returns the id of a child of the given node at the given index.
 *
 * @param nodeID:       ramdisk node id
 * @param index:        child index
 * @return if the child exists the child node id, otherwise -1
 *
 * @security-level APPLICATION
 */
int RamdiskChildAt(int node, int index)
{
	// prepare data
	SyscallRamdiskChildAt data;
	data.nodeID = node;
	data.index = index,

	// performs call
	syscall(SYSCALL_RAMDISK_CHILD_AT, (uint32_t) &data);

	// return the id
	return data.childID;
}
