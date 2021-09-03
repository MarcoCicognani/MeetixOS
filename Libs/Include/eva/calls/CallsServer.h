/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_API_CALLS_SERVER
#define EVA_API_CALLS_SERVER

#include <eva/kernel.h>
#include <eva/system.h>

__BEGIN_C

/**
 * parameter struct used by {RegisterAsServer} Syscall
 *
 * @field status:			filled with the return code of the call
 * @field identifier:		the identifier of the server to register
 * @field securityLevel:	the new security level to apply for the server
 */
typedef struct {
    char*                  identifier;
    SecurityLevel          securityLevel;
    RegisterAsServerStatus status;
} __attribute__((packed)) SyscallRegisterAsServer;

/**
 * parameter struct used by {ServerManage} syscall
 *
 * @field identifier:   the identifier of the server
 * @field buffer:       the buffer for the command
 * @field status:       the resultant state of the command sending
 */
typedef struct {
    char*              identifier;
    ServerManageBuffer buffer;
    ServerManageStatus status;
} __attribute__((packed)) SyscallServerManage;

__END_C

#endif
