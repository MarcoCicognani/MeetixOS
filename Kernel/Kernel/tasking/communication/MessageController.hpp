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

#ifndef IPC_MAILBOX
#define IPC_MAILBOX

#include "Api/IPC.h"
#include "Api/StdInt.h"

/**
 * Maximum messages count in a single queue
 */
#define MESSAGE_QUEUE_SIZE 64

/**
 * Message head, manteins the pointer of the last
 * and the first message head of a message queue
 */
struct MessageQueueHead {
public:
    /**
     * empty constructor
     */
    MessageQueueHead() : first(0), last(0), total(0) {
    }

    /**
     * internal data
     */
    MessageHeader* first; // pointer to the first message
    MessageHeader* last;  // pointer to the last message
    size_t         total; // messages count
};

/**
 * Static class to manage messaging queues between threads
 * in Ghost/Evangelion arch is permitted messages between threads of the same process.
 *
 * Messages are not old in a queue into the thread descriptor, but into a global map indicized by
 * thread identifiers
 */
class MessageController {
public:
    /**
     * Delete the message queue of the thread provided by id
     *
     * @param tid:		the identifier of the thread that is to be cleared
     */
    static void clear(Tid tid);

    /**
     * Copy the <content> buffer provided by the <source> thread to the message queue of the
     * <target> thread
     *
     * @param target:		the identifier of the thread that have to receive the message
     * @param source:		the identifier of the thread sender of the the message
     * @param contentLen:	the size of the buffer
     * @param tx:			the transaction identifier to distingue the messages
     */
    static MessageSendStatus
    sendMessage(Tid target, Tid source, void* content, size_t contentLen, MessageTransaction tx);

    /**
     * Copy the last received message on the message queue of the target on <out> buffer
     * if <tx> isn't {MESSAGE_TRANSACTION_NONE} is copied to <out> the first message with <tx> id
     *
     * @param target:		the identifier of the thread that have to receive the message
     * @param out:			the pointer to the userspace instance of the MessageHeader where
     * write the last message
     * @param max:			the size of the <out> buffer
     * @param tx:			the transaction identifier
     */
    static MessageReceiveStatus
    receiveMessage(Tid target, MessageHeader* out, size_t max, MessageTransaction tx);
};

#endif
