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
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include "__internal.h"
#include "eva/user.h"

/**
 * Retrieves ramdisk entry info for a specific node.
 *
 * @param nodeID:   ramdisk node id
 * @param out:      is filled with the info
 *
 * @security-level APPLICATION
 */
void RamdiskInfo(int node, RamdiskEntryInfo* out) {
    // create data
    SyscallRamdiskInfo data;
    data.nodeID = node;

    // performs the call
    syscall(SYSCALL_RAMDISK_INFO, (uint32_t)&data);

    // copy out the data
    out->length = data.length;
    memcopy((char*)out->name, (char*)data.name, strlength(data.name));
    out->type = data.type;
}
