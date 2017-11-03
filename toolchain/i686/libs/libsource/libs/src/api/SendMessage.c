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
 * Sends a message to the given task. This means that <length> bytes from the
 * buffer <buffer> are copied to a message that is then sent to the <target> task.
 * The message may be no longer than {MESSAGE_MAXIMUM_LENGTH}.
 *
 * @param target:       id of the target task
 * @param buffer:      message content buffer
 * @param length:      number of bytes to copy from the buffer
 * @return one of the {MessageSendStatus} codes
 *
 * @security-level APPLICATION
 */
MessageSendStatus SendMessage(Tid target, void *buffer, size_t length)
{
	return SendMessageTM(target, buffer, length, MESSAGE_TRANSACTION_NONE, MESSAGE_SEND_MODE_BLOCKING);
}

/**
 * Sends a message to the given task. This means that <length> bytes from the
 * buffer <buffer> are copied to a message that is then sent to the <target> task.
 * The message may be no longer than {MESSAGE_MAXIMUM_LENGTH}.
 *
 * @param target:       id of the target task
 * @param buffer:      message content buffer
 * @param length:      number of bytes to copy from the buffer
 * @param mode:    		determines how the function blocks when given, default is {MESSAGE_SEND_MODE_BLOCKING}
 * @return one of the {MessageSendStatus} codes
 *
 * @security-level APPLICATION
 */
MessageSendStatus SendMessageM(Tid target, void *buffer, size_t length, MessageSendMode mode)
{
	return SendMessageTM(target, buffer, length, MESSAGE_TRANSACTION_NONE, mode);
}

/**
 * Sends a message to the given task. This means that <length> bytes from the
 * buffer <buffer> are copied to a message that is then sent to the <target> task.
 * The message may be no longer than {MESSAGE_MAXIMUM_LENGTH}.
 *
 * @param target:       id of the target task
 * @param buffer:      message content buffer
 * @param length:      number of bytes to copy from the buffer
 * @param tx:      		transaction id
 * @return one of the {MessageSendStatus} codes
 *
 * @security-level APPLICATION
 */
MessageSendStatus SendMessageT(Tid target, void *buffer, size_t length, MessageTransaction tx)
{
	return SendMessageTM(target, buffer, length, tx, MESSAGE_SEND_MODE_BLOCKING);
}

/**
 * Sends a message to the given task. This means that <length> bytes from the
 * buffer <buffer> are copied to a message that is then sent to the <target> task.
 * The message may be no longer than {MESSAGE_MAXIMUM_LENGTH}.
 *
 * The mode specifies how the function shall block:
 * - {MESSAGE_SEND_MODE_BLOCKING} the executing task will bock if the target tasks message queue is full
 * - {MESSAGE_SEND_MODE_NON_BLOCKING} the function will return {MESSAGE_SEND_STATUS_QUEUE_FULL} if the target tasks message queue is full
 *
 * @param target:       id of the target task
 * @param buffer:      message content buffer
 * @param length:      number of bytes to copy from the buffer
 * @param mode:    		determines how the function blocks when given, default is {MESSAGE_SEND_MODE_BLOCKING}
 * @param tx:      		transaction id
 * @return one of the {MessageSendStatus} codes
 *
 * @security-level APPLICATION
 */
MessageSendStatus SendMessageTM(Tid target, void *buffer, size_t length, MessageTransaction tx, MessageSendMode mode)
{
	// prepare data
	SyscallSendMessage data;
	data.buffer = buffer;
	data.length = length;
	data.receiver = target;
	data.mode = mode;
	data.transaction = tx;

	// performs the call
	syscall(SYSCALL_MESSAGE_SEND, (uint32_t) &data);

	// return status
	return data.status;
}
