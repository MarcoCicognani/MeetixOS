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

#ifndef EVA_MULTITASKING_WAIT_MANAGER_RECEIVE_MESSAGE
#define EVA_MULTITASKING_WAIT_MANAGER_RECEIVE_MESSAGE

#include "logger/logger.hpp"
#include "tasking/communication/MessageController.hpp"

#include <tasking/wait/waiter.hpp>

/**
 * Waiter implementation used for tasks that want
 * to receive syncronous messages from another thread
 */
class WaiterReceiveMessage : public Waiter {
private:
    // pointer to the system call data
    SyscallReceiveMessage* data;

public:
    /**
     * filled constructor
     *
     * @param _data:	the system call data
     */
    WaiterReceiveMessage(SyscallReceiveMessage* _data) : data(_data) {
    }

    /**
     * implementation of check waiting method
     *
     * @param task:		the task that wait
     * @return true if task must keep waiting
     */
    virtual bool checkWaiting(Thread* task) {
        // if break is issued, stop sleeping
        if ( data->breakCondition != 0 && *data->breakCondition ) {
            data->status = MESSAGE_RECEIVE_STATUS_INTERRUPTED;
            return false;
        }

        data->status = MessageController::receiveMessage(task->id,
                                                         data->buffer,
                                                         data->maximum,
                                                         data->transaction);

        // if queue is empty, continue sleeping
        if ( data->status == MESSAGE_RECEIVE_STATUS_QUEUE_EMPTY )
            return true;

        // stop sleeping
        return false;
    }

    /**
     * @return the name of the waiter
     */
    virtual const char* debugName() {
        return "WaiterReceiveMessage";
    }
};

#endif
