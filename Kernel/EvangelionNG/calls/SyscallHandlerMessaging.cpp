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

#include <calls/SyscallHandler.hpp>

#include <logger/logger.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/wait/WaiterSendMessage.hpp>
#include <tasking/wait/WaiterReceiveMessage.hpp>
#include <tasking/communication/MessageController.hpp>

/**
 * Performs the message send thread by thread
 */
SYSCALL_HANDLER(sendMessage)
{
	SyscallSendMessage *data = (SyscallSendMessage*) SYSCALL_DATA(currentThread->cpuState);

	// send the message
	data->status = MessageController::sendMessage(data->receiver, currentThread->id, data->buffer, data->length, data->transaction);

	// move receiver to top of wait queue
	if (data->status == MESSAGE_SEND_STATUS_SUCCESSFUL)
	{
		Thread *receiver = Tasking::getTaskById(data->receiver);
		if (receiver) Tasking::increaseWaitPriority(receiver);
	}

	// check if block
	else if (data->mode == MESSAGE_SEND_MODE_BLOCKING && data->status == MESSAGE_SEND_STATUS_QUEUE_FULL)
	{
		currentThread->wait(new WaiterSendMessage(data));
		return Tasking::schedule();
	}

	return currentThread;
}

/**
 * Performs the message receive thread by thread
 */
SYSCALL_HANDLER(receiveMessage)
{
	SyscallReceiveMessage *data = (SyscallReceiveMessage*) SYSCALL_DATA(currentThread->cpuState);

	// get message
	data->status = MessageController::receiveMessage(currentThread->id, data->buffer, data->maximum, data->transaction);

	// there was a message, immediate return
	if (data->status == MESSAGE_RECEIVE_STATUS_SUCCESSFUL) return currentThread;
	if (data->status == MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY)
	{
		// check mode to see what to do
		if (data->mode == MESSAGE_RECEIVE_MODE_NON_BLOCKING) return currentThread;

		// perform blocking
		currentThread->wait(new WaiterReceiveMessage(data));
		return Tasking::schedule();
	}

	// something went wrong, immediate return
	return currentThread;
}
