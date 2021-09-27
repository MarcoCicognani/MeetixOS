/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
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

#include "RegistrationThread.hpp"

#include <gui/uispech.hpp>
#include <interface/ApplicationExitCleanupThread.hpp>
#include <interface/CommandMessageReceiverThread.hpp>
#include <utils/utils.hpp>

/**
 *
 */
[[noreturn]] void RegistrationThread::run() {
    // register this thread
    if ( !s_task_register_id("registration") ) {
        Utils::log("failed to register task identifier for registration thread");
    }

    // wait for initialization requests
    auto bufferLength = sizeof(MessageHeader) + sizeof(UiInitializeRequest);
    auto buffer       = new uint8_t[bufferLength];

    while ( true ) {
        auto stat = s_receive_message(buffer, bufferLength);

        if ( stat == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
            auto requestMessage = reinterpret_cast<MessageHeader*>(buffer);

            // create handler thread
            auto communicator    = new CommandMessageReceiverThread();
            auto communicatorTid = communicator->start();

            // create a thread that cleans up the ui when the client thread exits
            auto cleanup
                = new ApplicationExitCleanupThread(s_get_pid_for_tid(requestMessage->m_sender_tid),
                                                   communicator);
            auto cleanerTid = cleanup->start();

            // send response
            UiInitializeResponse response;
            response.header.id                  = UI_PROTOCOL_INITIALIZATION;
            response.status                     = UI_PROTOCOL_SUCCESS;
            response.windowServerDelegateThread = communicatorTid;
            response.windowServerCleanUPThread  = cleanerTid;
            s_send_message_t(requestMessage->m_sender_tid,
                             &response,
                             sizeof(UiInitializeResponse),
                             requestMessage->m_transaction);
        }
    }
}
