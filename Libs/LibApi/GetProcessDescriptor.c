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
 * Call the kernel to get the descriptor of a specific thread
 *
 * @param id:			the id of the thread that want to get the descriptor
 * @param descriptor:	pointer to a valid descriptor
 * @return whether the operation success
 */
uint8_t GetProcessDescriptor(Pid id, ProcessDescriptor* descriptor) {
    // check validity
    if ( descriptor ) {
        // copy pid
        descriptor->main.id = id;

        // performs the call
        syscall(SYSCALL_GET_PROCESS_DESCRIPTOR, (uint32_t)descriptor);
        return descriptor->main.found;
    }

    return false;
}