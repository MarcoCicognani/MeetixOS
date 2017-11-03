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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#include "eva/user.h"

/**
 * Receives a message. At maximum <max> bytes will be attempted to be copied to
 * the buffer <buffer>. Note that when receiving a message, a buffer with a size of
 * at least the size of {MessageHeader} plus the size of the sent message
 * must be used.
 *
 * @param buffer:           output buffer
 * @param max:              maximum number of bytes to copy to the buffer
 *
 * @security-level APPLICATION
 */
MessageReceiveStatus ReceiveMessage(void *buffer, size_t max)
{
	return ReceiveMessageTMB(buffer, max, MESSAGE_TRANSACTION_NONE, MESSAGE_RECEIVE_MODE_BLOCKING, NULL);
}

/**
 * Receives a message. At maximum <max> bytes will be attempted to be copied to
 * the buffer <buffer>. Note that when receiving a message, a buffer with a size of
 * at least the size of {MessageHeader} plus the size of the sent message
 * must be used.
 *
 * @param buffer:           output buffer
 * @param max:              maximum number of bytes to copy to the buffer
 * @param mode:         	determines how the function blocks when given, default is {MESSAGE_RECEIVE_MODE_BLOCKING}
 *
 * @security-level APPLICATION
 */
MessageReceiveStatus ReceiveMessageM(void *buffer, size_t max, MessageReceiveMode mode)
{
	return ReceiveMessageTMB(buffer, max, MESSAGE_TRANSACTION_NONE, mode, NULL);
}

/**
 * Receives a message. At maximum <max> bytes will be attempted to be copied to
 * the buffer <buffer>. Note that when receiving a message, a buffer with a size of
 * at least the size of {MessageHeader} plus the size of the sent message
 * must be used.
 *
 * @param buffer:           output buffer
 * @param max:              maximum number of bytes to copy to the buffer
 * @param mode:         	determines how the function blocks when given, default is {MESSAGE_RECEIVE_MODE_BLOCKING}
 * @param tx:           	transaction id
 * @param breakCondition:   can be used to break the waiting process by setting its value to 1
 *
 * @security-level APPLICATION
 */
MessageReceiveStatus ReceiveMessageT(void *buffer, size_t max, MessageTransaction tx)
{
	return ReceiveMessageTMB(buffer, max, tx, MESSAGE_RECEIVE_MODE_BLOCKING, NULL);
}

/**
 * Receives a message. At maximum <max> bytes will be attempted to be copied to
 * the buffer <buffer>. Note that when receiving a message, a buffer with a size of
 * at least the size of {MessageHeader} plus the size of the sent message
 * must be used.
 *
 * @param buffer:           output buffer
 * @param max:              maximum number of bytes to copy to the buffer
 * @param mode:         	determines how the function blocks when given, default is {MESSAGE_RECEIVE_MODE_BLOCKING}
 * @param tx:           	transaction id
 *
 * @security-level APPLICATION
 */
MessageReceiveStatus ReceiveMessageTM(void *buffer, size_t max, MessageTransaction tx, MessageReceiveMode mode)
{
	return ReceiveMessageTMB(buffer, max, tx, mode, NULL);
}

/**
 * Receives a message. At maximum <max> bytes will be attempted to be copied to
 * the buffer <buf>. Note that when receiving a message, a buffer with a size of
 * at least the size of {MessageHeader} plus the size of the sent message
 * must be used.
 *
 * After successful completion, the buffer will contain the message header followed by the content of the message.
 * - to access the header, use the buffer pointer: ((MessageHeader * buffer)
 * - to access the content, use the helper macro:  MESSAGE_CONTENT(buffer)
 *
 * The mode specifies how the function shall block:
 * - {MESSAGE_RECEIVE_MODE_BLOCKING} the executing task will block if no messages are available
 * - {MESSAGE_RECEIVE_MODE_NON_BLOCKING} the function will return {MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY} if the executing tasks message queue is empty
 *
 * When a transaction ID is given, only messages that were sent with the same
 * transaction ID will be received.
 *
 * @param buffer:               output buffer
 * @param max:                  maximum number of bytes to copy to the buffer
 * @param mode:             	determines how the function blocks when given, default is {MESSAGE_RECEIVE_MODE_BLOCKING}
 * @param tx:               	transaction id
 * @param breakCondition:   	can be used to break the waiting process by setting its value to 1
 *
 * @security-level APPLICATION
 */
MessageReceiveStatus ReceiveMessageTMB(void *buffer, size_t max, MessageTransaction tx, MessageReceiveMode mode, uint8_t *breakCondition)
{
	// prepare the data
	SyscallReceiveMessage data;
	data.buffer = (MessageHeader*) buffer;
	data.maximum = max;
	data.mode = mode;
	data.transaction = tx;
	data.breakCondition = breakCondition;

	// performs the call
	syscall(SYSCALL_MESSAGE_RECEIVE, (uint32_t) &data);

	// return the received status
	return data.status;
}
