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

#ifndef EVA_INTERRUPTS_EXCEPTION_HANDLER
#define EVA_INTERRUPTS_EXCEPTION_HANDLER

#include <system/ProcessorState.hpp>
#include <tasking/thread.hpp>

/**
 * manage the exception interrupts
 */
class InterruptExceptionHandler
{
public:
	/**
	 * handle by code the types of execeptions
	 *
	 * @param currentThread:	the thread that have caused the exception
	 * @return a new thread if the exeception is resolved, healt the system otherwise
	 */
	static Thread *handle(Thread *currentThread);

	/**
	 * Handles a general protection fault. If the current task is a VM86 task, the fault is redirected
	 * to the Virtual8086 monitor.
	 *
	 * @param currentThread:		the thread that have caused the exception
	 * @return a new thread
	 */
	static Thread *handleGeneralProtectionFault(Thread *currentThread);

	/**
	 * handle the page fault execeptions
	 *
 	 * @param currentThread:		the thread that have caused the exception
 	 * @return a new thread
 	 */
	static Thread *handlePageFault(Thread *currentThread);

	/**
	 * handle dividing execeptions
	 *
	 * @param currentThread:		the thread that have caused the exception
	 * @return a new thread
	 */
	static Thread *handleDivideError(Thread *currentThread);

	/**
	 * handle the invalid operations code execeptions
	 *
	 * @param currentThread:		the thread that have caused the exception
	 * @return a new thread
	 */
	static Thread *handleInvalidOperationCode(Thread *currentThread);

	/**
	 * @return the cr2 regex value
	 */
	static uint32_t getCR2();

	/**
	 * print dump informations of the provided thread
	 *
	 * @param currentThread:		the thread to be dumped
	 */
	static void dump(Thread *currentThread);

	/**
	 * print the cpu state
	 *
	 * @param cpuState:		the cpu state to be printed
	 */
	static void printStackTrace(ProcessorState *cpuState);
};

#endif
