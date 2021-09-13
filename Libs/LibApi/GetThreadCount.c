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
 * Call the kernel to ask the count of the thread in the system
 *
 * @return the thread count
 */
uint32_t GetThreadCount() {
    return GetThreadCountT(THREAD_TYPE_MAIN | THREAD_TYPE_VM86 | THREAD_TYPE_SUB);
}

/**
 * Call the kernel to ask the count of the thread in the system
 *
 * @param type:		the type of thread that we want the count
 * @return the thread count
 */
uint32_t GetThreadCountT(ThreadType type) {
    // create syscall data
    SyscallGetThreadCount data;
    data.type = type;

    // perform the call
    syscall(SYSCALL_GET_THREAD_COUNT, (uint32_t)&data);
    return data.count;
}