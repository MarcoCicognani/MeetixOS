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

#include <eva.h>
#include <events/EventProcessor.hpp>
#include <gui/uispech.hpp>
#include <interface/CommandMessageResponderThread.hpp>
#include <stdio.h>
#include <zipNET.hpp>

/**
 *
 */
void CommandMessageResponderThread_t::run() {
    TaskRegisterID("messageResponder");
    EventProcessor_t* eventProcessor = ZipNET::instance()->eventProcessor;
    while ( true ) {
        // wait until messages are added
        AtomicLock(&bufferEmpty);

        // process all
        while ( buffer.size() > 0 ) {
            // get reference to response from the queue
            CommandMessageResponse_t& response = buffer.back();
            SendMessageT(response.target, response.message, response.length, response.transaction);

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
void CommandMessageResponderThread_t::sendResponse(CommandMessageResponse_t& response) {
    buffer.push_back(response);
    bufferEmpty = false;
}
