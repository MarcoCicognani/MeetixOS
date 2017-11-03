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

#ifndef EVA_GLOBAL_IPC_MESSAGE
#define EVA_GLOBAL_IPC_MESSAGE

#include "eva/common.h"
#include "eva/kernel.h"
#include "eva/stdint.h"

__BEGIN_C

// message transactions
typedef uint32_t MessageTransaction;
#define MESSAGE_TRANSACTION_NONE			((MessageTransaction)0)
#define MESSAGE_TRANSACTION_FIRST			((MessageTransaction)1)

/**
 * message header
 */
typedef struct _MessageHeader
{
	Tid sender;
	MessageTransaction transaction;
	size_t length;
	struct _MessageHeader *previous;
	struct _MessageHeader *next;
}__attribute__((packed)) MessageHeader;

/**
 * use it to extract the pointer to the message that the thread have received
 */
#define MESSAGE_CONTENT(message)			(((uint8_t*) message) + sizeof(MessageHeader))

/**
 * messaging bounds
 */
#define MESSAGE_MAXIMUM_LENGTH				(8192)
#define MESSAGE_MAXIMUM_QUEUE_CONTENT		(8192 * 64)

/**
 * modes for message sending
 */
typedef int MessageSendMode;
static const MessageSendMode MESSAGE_SEND_MODE_BLOCKING = 0;
static const MessageSendMode MESSAGE_SEND_MODE_NON_BLOCKING = 1;

/**
 * modes for message receiving
 */
typedef int MessageReceiveMode;
static const MessageReceiveMode MESSAGE_RECEIVE_MODE_BLOCKING = 0;
static const MessageReceiveMode MESSAGE_RECEIVE_MODE_NON_BLOCKING = 1;

/**
 * status for message sending
 */
typedef int MessageSendStatus;
static const MessageSendStatus MESSAGE_SEND_STATUS_SUCCESSFUL = 1;
static const MessageSendStatus MESSAGE_SEND_STATUS_QUEUE_FULL = 2;
static const MessageSendStatus MESSAGE_SEND_STATUS_FAILED = 3;
static const MessageSendStatus MESSAGE_SEND_STATUS_EXCEEDS_MAXIMUM = 4;

/**
 * status for message receiving
 */
typedef int MessageReceiveStatus;
static const MessageReceiveStatus MESSAGE_RECEIVE_STATUS_SUCCESSFUL = 1;
static const MessageReceiveStatus MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY = 2;
static const MessageReceiveStatus MESSAGE_RECEIVE_STATUS_FAILED = 3;
static const MessageReceiveStatus MESSAGE_RECEIVE_STATUS_FAILED_NOT_PERMITTED = 4;
static const MessageReceiveStatus MESSAGE_RECEIVE_STATUS_EXCEEDS_BUFFER_SIZE = 5;
static const MessageReceiveStatus MESSAGE_RECEIVE_STATUS_INTERRUPTED = 6;

__END_C

#endif
