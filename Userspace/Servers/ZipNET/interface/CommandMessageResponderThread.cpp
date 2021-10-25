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

#include <Api.h>
#include <events/EventProcessor.hpp>
#include <interface/CommandMessageResponderThread.hpp>
#include <Tasking/LockGuard.hh>
#include <zipNET.hpp>

/**
 *
 */
[[noreturn]] void CommandMessageResponderThread::run() {
    s_task_register_id("messageResponder");
    EventProcessor* eventProcessor = ZipNET::instance()->eventProcessor;
    while ( true ) {
        s_atomic_lock(&m_buffer_empty_lock);
        Tasking::LockGuard buffer_guard{ m_buffer_lock };

        // process all
        while ( !buffer.empty() ) {
            // get reference to response from the queue
            CommandMessageResponse_t& response = buffer.back();
            s_send_message_t(response.target,
                             response.message,
                             response.length,
                             response.transaction);

            // delete message buffer
            delete (MessageHeader*)response.message;

            // remove response from queue
            buffer.pop_back();
        }
    }
}

/**
 *
 */
void CommandMessageResponderThread::sendResponse(CommandMessageResponse_t& response) {
    Tasking::LockGuard buffer_guard{ m_buffer_lock };
    buffer.push_back(response);
    m_buffer_empty_lock = false;
}
