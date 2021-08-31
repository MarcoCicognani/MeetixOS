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

#ifndef EVA_MULTITASKING_WAIT_MANAGER_JOIN
#define EVA_MULTITASKING_WAIT_MANAGER_JOIN

#include <tasking/wait/waiter.hpp>
#include <logger/logger.hpp>
#include <tasking/tasking.hpp>

/**
 * Waiter implementation used for thread joining,
 * when a thread call the {Join} system call the caller is stored in
 * the waiting list with this waiter, that check if the joined thread is still alive
 */
class WaiterJoin : public Waiter
{
private:
	// the task id that the waiter still wait
	uint32_t waitTask;

public:

	/**
	 * filled constructor
	 *
	 * @param _waitTask:	the task that the waiter wait
	 */
	WaiterJoin(uint32_t _waitTask) : waitTask(_waitTask) {}

	/**
	 * implementation of check waiting method
	 *
	 * @param task:		the task that wait
	 * @return true if task must keep waiting
	 */
	virtual bool checkWaiting(Thread *task)
	{
		Thread *other = Tasking::getTaskById(waitTask);
		return (other != 0 && other->alive);
	}

	/**
	 * @return the name of the waiter
	 */
	virtual const char *debugName(){
		return "WaiterJoin";
	}

};

#endif
