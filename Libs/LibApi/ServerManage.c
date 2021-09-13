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
 * Provides a standard interface to communicate with servers,
 * first the call send a message to the server identified by <serverIdentifier>,
 * if it exists, then wait with a receive message the response of the server, that provides data by
 * the <data> parameter
 *
 * @param serverIdentifier:			the name of the server
 * @param command:					a command to send to the server
 * @param data:						a pointer the data that process the server
 * called
 * @param length:					the length of the data buffer
 * @return one of the {ServerManageStatus} codes.
 */
ServerManageStatus ServerManage(const char*         serverIdentifier,
                                ServerManageCommand command,
                                void*               data,
                                uint32_t            length) {
    // prepare the data
    SyscallServerManage callData;
    callData.identifier     = (char*)serverIdentifier;
    callData.buffer.command = command;
    callData.buffer.length  = length;
    callData.buffer.data    = data;

    // performs the call
    syscall(SYSCALL_SERVER_MANAGE, (uint32_t)&callData);

    // check send status
    if ( callData.status == SERVER_MANAGE_STATUS_COMMAND_SEND_SUCCESS ) {
        // now wait the server response
        uint32_t bufsize = sizeof(ServerManageBuffer) + sizeof(MessageHeader);
        uint8_t  buffer[bufsize];
        if ( ReceiveMessage(buffer, bufsize) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
            // message received
            ServerManageBuffer* serverResponse = (ServerManageBuffer*)MESSAGE_CONTENT(buffer);
            memcopy(data, serverResponse->data, length);
            return SERVER_MANAGE_STATUS_COMMAND_SUCCESS;
        }

        return SERVER_MANAGE_STATUS_NO_RESPONSE;
    }

    return callData.status;
}