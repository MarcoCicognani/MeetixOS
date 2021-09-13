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

#include "eva/user.h"

/**
 * Maps the given physical address to the executing processes address space so
 * it can access it directly.
 *
 * @param address:     the physical memory address that should be mapped
 * @param size:        the size that should be mapped
 * @return a pointer to the mapped area within the executing processes address space
 *
 * @security-level DRIVER
 */
void* MapMmio(void* address, uint32_t size) {
    // prepare data
    SyscallMapMmio data;
    data.physicalAddress = address;
    data.size            = size;

    // performs the call
    syscall(SYSCALL_MAP_MMIO_AREA, (uint32_t)&data);

    // return the data
    return data.virtualAddress;
}
