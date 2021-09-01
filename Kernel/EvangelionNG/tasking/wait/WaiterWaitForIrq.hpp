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

#ifndef EVA_MULTITASKING_WAIT_MANAGER_INTERRUPTS
#define EVA_MULTITASKING_WAIT_MANAGER_INTERRUPTS

#include <tasking/wait/waiter.hpp>
#include <system/interrupts/handling/InterruptRequestHandler.hpp>

/**
 * Waiter implementation used for task
 * that register an userspace interrupt handler and wait a trigger of it
 */
class WaiterWaitForIrq : public Waiter
{
private:
	// the interrupt to handle
	uint32_t interrupt;

public:

	/**
	 * filled constructor
	 *
	 * @param _interrupt:	the interrupt to wait
	 */
	WaiterWaitForIrq(uint32_t _interrupt) : interrupt(_interrupt) {}

	/**
	 * implementation of check waiting method
	 *
	 * @param task:		the task that wait
	 * @return true if task must keep waiting
	 */
	virtual bool checkWaiting(Thread *task)
	{
		bool fired = InterruptRequestHandler::pollIrq((uint32_t) interrupt);

		// interrupt is fired
		if (fired) return false;

		// keep waiting for interrupt
		else return true;
	}

	/**
	 * @return the name of the waiter
	 */
	virtual const char *debugName(){
		return "WaiterWaitForIrq";
	}

};

#endif
