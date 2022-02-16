/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include "__internal.hh"

#include <Api/User.h>

ServerManageStatus
s_server_manage(const char* server_identifier, ServerManageCommand manage_command, void* data_ptr, usize data_len) {
    SyscallServerManage callData;
    callData.identifier     = (char*)server_identifier;
    callData.buffer.command = manage_command;
    callData.buffer.length  = data_len;
    callData.buffer.data    = data_ptr;

    // performs the call
    do_syscall(SYSCALL_SERVER_MANAGE, (usize)&callData);

    // check send status
    if ( callData.status == SERVER_MANAGE_STATUS_COMMAND_SEND_SUCCESS ) {
        // now wait the server response
        uint32_t bufsize = sizeof(ServerManageBuffer) + sizeof(MessageHeader);
        uint8_t  buffer[bufsize];
        if ( s_receive_message(buffer, bufsize) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
            // message received
            ServerManageBuffer* serverResponse = (ServerManageBuffer*)MESSAGE_CONTENT(buffer);
            memory_copy(data_ptr, serverResponse->data, data_len);
            return SERVER_MANAGE_STATUS_COMMAND_SUCCESS;
        }

        return SERVER_MANAGE_STATUS_NO_RESPONSE;
    }

    return callData.status;
}