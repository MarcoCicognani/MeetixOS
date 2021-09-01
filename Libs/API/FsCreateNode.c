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
 * Creates a virtual filesystem node.
 *
 * @param parent:           id of the parent node
 * @param name:             the node name
 * @param type:             one of the FsNodeType types
 * @param fsID:             the filesystem id of the node
 * @param outCreatedID:     id of the created node
 * @return one of the {FsCreateNodeStatus} codes
 *
 * @security-level DRIVER
 */
FsCreateNodeStatus FsCreateNode(uint32_t parent, char *name, FsNodeType type, uint64_t fsID, uint32_t *outCreatedID)
{
	// prepare data
	SyscallFsCreateNode data;
	data.parentID = parent;
	data.name = (char*) name;
	data.type = type;
	data.physFsID = fsID;

	// performs the call
	syscall(SYSCALL_FS_CREATE_NODE, (uint32_t) &data);

	// fill out status
	*outCreatedID = data.createdID;
	return data.result;
}
