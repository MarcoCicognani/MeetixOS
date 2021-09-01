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

#include <eva.h>
#include "__internal.h"

/**
 * Retrieves the thread name from thread id number
 *
 * @param threadID:     id of thread that we can get the identifier
 * @param out:          buffer where call store the string
 *
 * @security-level APPLICATION
 */
void TaskGetIdentifier(Tid threadID, char *out)
{
    // create struct
    SyscallGetIdentifier data;
    data.id = threadID;

    // perform call to kernel
    syscall(SYSCALL_GET_THREAD_NAME, (uint32_t) &data);

    // if call success copy on buffer
    if (data.identifier) memcopy(out, data.identifier, strlength(data.identifier));
}
