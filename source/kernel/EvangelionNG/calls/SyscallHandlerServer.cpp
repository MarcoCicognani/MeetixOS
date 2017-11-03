/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#include "eva/calls/calls.h"
#include <calls/SyscallHandler.hpp>

#include <logger/logger.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/communication/MessageController.hpp>

/**
 * Implement a standart interface to communicate with the kernel Servers
 */
SYSCALL_HANDLER(serverManage)
{
    SyscallServerManage *data = (SyscallServerManage*) SYSCALL_DATA(currentThread->cpuState);

    // check identifier validity
    if (*data->identifier)
    {
        // get the server
        Process *server = Tasking::getServer(data->identifier);
        if (server)
        {
            // send a message to the server
            MessageSendStatus status = MessageController::sendMessage(server->main->id, currentThread->id, (uint8_t*)&data->buffer, sizeof(ServerManageBuffer), MESSAGE_TRANSACTION_NONE);
            if (status == MESSAGE_SEND_STATUS_SUCCESSFUL)
            {
                Tasking::increaseWaitPriority(server->main);
                data->status = SERVER_MANAGE_STATUS_COMMAND_SEND_SUCCESS;

                return Tasking::schedule();
            }

            else data->status = SERVER_MANAGE_STATUS_COMMAND_SEND_FAIL;
        }

        else data->status = SERVER_MANAGE_STATUS_SERVER_NOT_FOUND;
    }

    else data->status = SERVER_MANAGE_STATUS_INVALID_IDENTIFIER;

    return currentThread;
}

/**
 * Register the current process as a kernel server and insert
 * it into a special list of tasking interface.
 * Modify the security level to at least {SECURITY_LEVEL_SERVER}
 */
SYSCALL_HANDLER(registerAsServer)
{
    SyscallRegisterAsServer *data = (SyscallRegisterAsServer*) SYSCALL_DATA(currentThread->cpuState);
    data->status = REGISTER_AS_SERVER_STATUS_UNKNOWN;

    // only main can register, because the call
    // register as server the entire process
    if (currentThread->type == THREAD_TYPE_MAIN)
    {
        // set the new identifier
        currentThread->setIdentifier(data->identifier);

        // register the process
        Process *process = currentThread->process;
        if (Tasking::addServer(process))
        {
            // the minimum security level permitted to apply for a server is the SECURITY_LEVEL_SERVER
            //if (process->securityLevel != data->securityLevel)
            //    ThreadManager::applySecurityLevel(process->main->cpuState, data->securityLevel <= SECURITY_LEVEL_SERVER ? data->securityLevel : SECURITY_LEVEL_SERVER);
            data->status = REGISTER_AS_SERVER_STATUS_SUCCESS;
        }

        else data->status = REGISTER_AS_SERVER_STATUS_ALREADY_REGISTERED;
    }

    else data->status = REGISTER_AS_SERVER_STATUS_NON_MAIN;

    return currentThread;
}
