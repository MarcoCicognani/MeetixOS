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

#ifndef EVA_MULTITASKING_WAIT_MANAGER_SLEEP
#define EVA_MULTITASKING_WAIT_MANAGER_SLEEP

#include <tasking/wait/waiter.hpp>
#include <tasking/tasking.hpp>
#include <logger/logger.hpp>

/**
 * Waiter implementation used for threads that performs the {Sleep} system call
 */
class WaiterSleep : public Waiter
{
private:
	/**
	 * internal data
	 */
	Scheduler *timingScheduler;		// scheduler that schedule this thread
	uint64_t timeStart;				// start time
	uint64_t timeSleep;				// time to sleep

public:
	/**
	 * filled constructor
	 *
	 * @param task:		the task that want to sleep
	 * @param ms:		the amount of milliseconds that task want to sleep
	 */
	WaiterSleep(Thread *task, uint64_t ms) : timingScheduler(task->scheduler), timeStart(timingScheduler->getMilliseconds()), timeSleep(ms) {}

	/**
	 * implementation of check waiting method
	 *
	 * @param task:		the task that wait
	 * @return true if task must keep waiting
	 */
	virtual bool checkWaiting(Thread *task)
	{
		uint64_t diff = timingScheduler->getMilliseconds() - timeStart;

		// keep sleeping
		if (diff < timeSleep) return true;

		// sleeping time finisched
		return false;
	}

	/**
	 * @return the name of the waiter
	 */
	virtual const char *debugName(){
		return "WaiterSleep";
	}

};

#endif
