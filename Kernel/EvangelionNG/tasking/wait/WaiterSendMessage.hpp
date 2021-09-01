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

#ifndef EVA_MULTITASKING_WAITER_SEND_MESSAGE
#define EVA_MULTITASKING_WAITER_SEND_MESSAGE

#include "tasking/wait/waiter.hpp"
#include "tasking/communication/MessageController.hpp"
#include "logger/logger.hpp"

/**
 * Waiter implementation used for tasks that want
 * to send syncronous messages from another thread
 */
class WaiterSendMessage : public Waiter
{
private:
	// pointer to the system call data
	SyscallSendMessage *data;

public:
	/**
	 * filled constructor
	 *
	 * @param _data:	the system call data
	 */
	WaiterSendMessage(SyscallSendMessage *_data) : data(_data) {}

	/**
	 * implementation of check waiting method
	 *
	 * @param task:		the task that wait
	 * @return true if task must keep waiting
	 */
	virtual bool checkWaiting(Thread *task)
	{
		// send the message
		data->status = MessageController::sendMessage(data->receiver, task->id, data->buffer, data->length, data->transaction);

		// check if keep block
		if (data->status == MESSAGE_SEND_STATUS_QUEUE_FULL) return true;

		return false;
	}

	/**
	 * @return the name of the waiter
	 */
	virtual const char *debugName(){
		return "WaiterSendMessage";
	}

};

#endif
