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

#include <eva/user.h>

/**
 * Register the current process as a kernel
 * server and return one of the {RegisterAsServerStatus} codes.
 * Update the security level of the process and permits to increase it since the
 * {SECURITY_LEVEL_SERVER}
 *
 * @param serverIdentifier:		the name of the server
 * @param slevel:				the new security level for the process
 * @return one of the {RegisterAsServerStatus} codes
 */
RegisterAsServerStatus RegisterAsServer(const char* serverIdentifier, SecurityLevel slevel) {
    // prepare the data
    SyscallRegisterAsServer data;
    data.identifier    = (char*)serverIdentifier;
    data.securityLevel = slevel;

    // performs the syscall
    syscall(SYSCALL_REGISTER_AS_SERVER, (uint32_t)&data);
    return data.status;
}