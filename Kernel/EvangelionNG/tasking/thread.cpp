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

#include "tasking/thread.hpp"
#include "tasking/process.hpp"
#include "utils/string.hpp"
#include "EvangelionNG.hpp"
#include "logger/logger.hpp"
#include "tasking/wait/WaiterPerformInterruption.hpp"
#include "debug/DebugInterfaceKernel.hpp"

/**
 * counter of the assigned thread id
 */
static Tid taskIdCounter = 0;

/**
 * creates a new thread with the provided type code
 *
 * @param type:		the type of thread (main/vm86/sub)
 */
Thread::Thread(ThreadType _type)
{
	// assign a new id
	id = taskIdCounter++;
	type = _type;
	priority = THREAD_PRIORITY_NORMAL;

	// reset execution infos
	waitCount = 0;
	rounds = 0;
	scheduler = 0;

	// set as alive the thread
	alive = true;
	cpuState = 0;
	identifier = 0;

	// no process
	process = 0;

	// set as null all thread infos
	userThreadAddr = 0;
	userData = 0;
	threadEntry = 0;

	// no local storage
	tlsCopyVirt = 0;

	// reset stacks
	userStackAreaStart = 0;
	kernelStackPageVirt = 0;
	kernelStackEsp0 = 0;

	// no waiters
	waitManager = 0;
	interruptionInfo = 0;

	// only vm86 have virtual 8086 informations
	if (type == THREAD_TYPE_VM86) vm86Information = new ThreadInformationVm86();
	else vm86Information = 0;
}

/**
 * thread destructor
 */
Thread::~Thread()
{
	if (identifier) delete identifier;
	if (waitManager) delete waitManager;
	if (vm86Information) delete vm86Information;
	if (interruptionInfo) delete interruptionInfo;
}

/**
 * @return the vm86Information instance
 */
ThreadInformationVm86 *Thread::getVm86Information()
{
	// only vm86 thread have the infos
	if (type == THREAD_TYPE_VM86 && vm86Information) return vm86Information;

	// trigger the panic
	EvaKernel::panic("tried to retrieve vm86 information from the non-vm86-process type task %i", id);
	return 0; // not reached
}

/**
 * @return the thread name
 */
const char *Thread::getIdentifier()
{
	return identifier;
}

/**
 * set a new name for the task
 *
 * @param newIdentifier:	the new provided identifier
 */
void Thread::setIdentifier(const char *newIdentifier)
{
	// only if is provided a value
	if (newIdentifier)
	{
		// get the length of string
		uint32_t newlength = String::length(newIdentifier);

		if (type == THREAD_TYPE_MAIN || type == THREAD_TYPE_VM86)
		{
			// remove first
			if (identifier) delete identifier;

			// allocate and copy on it new identifier
			identifier = new char[newlength + 1];
			String::copy(identifier, newIdentifier);
		}

		// set identifier for thread
		else if (type == THREAD_TYPE_SUB)
		{
			// if there is an old buffer
			if (identifier)
			{
				// get the length of old identifier
				uint32_t oldlength = String::length(identifier);
				char oldID[oldlength + 1];

				// copy on temporary buffer and delete the old
				String::copy(oldID, identifier);
				delete identifier;

				// and concat old and new identifier
				identifier = new char[newlength + oldlength + 1];
				String::concat(oldID, newIdentifier, identifier);
			}

			// if there isn't
			else
			{
				// allocate and copy on it new identifier
				identifier = new char[newlength + 1];
				String::copy(identifier, newIdentifier);
			}
		}

		DEBUG_INTERFACE_TASK_SET_IDENTIFIER(this->id, identifier);
	}
}

/**
 * create a new wait manager
 *
 * @param waitManager:		new wait manager
 */
void Thread::wait(Waiter *newWaitManager)
{
	// replace waiter
	if (waitManager) delete waitManager;
	else scheduler->moveToWaitQueue(this);

	waitManager = newWaitManager;
	DEBUG_INTERFACE_TASK_SET_STATUS(this->id, newWaitManager->debugName());
}

/**
 * @return true if the waiting is ended, false otherwise
 */
bool Thread::checkWaiting()
{
	return waitManager->checkWaiting(this);
}

/**
 * destroy the current waiter if exists
 */
void Thread::unwait()
{
	DEBUG_INTERFACE_TASK_SET_STATUS(this->id, "normal");
	if (waitManager)
	{
		delete waitManager;
		waitManager = 0;
		scheduler->moveToRunQueue(this);
	}
}

/**
 * raise the provided signal
 *
 * @param signal:	signal number to handle
 */
void Thread::raiseSignal(int signal)
{
	// get handler from process
	SignalHandler *handler = &(process->signalHandlers[signal]);
	if (handler->handler)
	{

		// get the thread that handles the signal (take this if its the right one)
		Thread *handlingThread = 0;
		if (handler->threadID == this->id) handlingThread = this;
		else handlingThread = Tasking::getTaskById(handler->threadID);

		// let handling thread enter signal handler
		if (handlingThread) handlingThread->enterSignalHandler(handler->handler, signal, handler->callback);
	}

	else
	{
		// do default handling if no handler is registered
		bool kill = false;

		if (signal == SIGSEGV)
		{
			kill = true;
			DEBUG_INTERFACE_TASK_SET_STATUS(this->id, "sigsegv");
		}

		if (kill)
		{
			logInfo("%! thread %i killed", "signal", id);
			alive = false;
			process->main->alive = false;
		}
	}
}

/**
 * create an irq handler
 *
 * @param address:		the address of the handler function
 * @param irq:			irq code
 * @param callback:		callback address
 */
void Thread::enterIrqHandler(uintptr_t address, uint8_t irq, uintptr_t callback)
{
	if (!startPrepareInterruption()) return;

	// tell interruption info that it's about an irq
	interruptionInfo->type = ThreadInterruptionInfoType::IRQ;
	interruptionInfo->handledIrq = irq;

	DEBUG_INTERFACE_TASK_SET_STATUS(this->id, "irq-handling");
	finishPrepareInterruption(address, callback);
}

/**
 * create a signal handler
 *
 * @param address:		the address of the handler function
 * @param signal:		irq signal
 * @param callback:		callback address
 */
void Thread::enterSignalHandler(uintptr_t address, int signal, uintptr_t callback)
{
	if (!startPrepareInterruption()) return;

	// tell interruption info that it's about an irq
	interruptionInfo->type = ThreadInterruptionInfoType::SIGNAL;
	interruptionInfo->handledSignal = signal;

	DEBUG_INTERFACE_TASK_SET_STATUS(this->id, "signal-handling");
	finishPrepareInterruption(address, callback);
}

/**
 * create the interruption info object if doesn't exist and store to it the waitManager
 */
bool Thread::startPrepareInterruption()
{
	// don't try to interrupt twice
	if (interruptionInfo != nullptr) return false;

	// create interruption info struct and store the waiter
	interruptionInfo = new ThreadInterruptionInfo();
	interruptionInfo->waitManager = waitManager;

	return true;
}

/**
 * create the interruption waiter
 *
 * @param address:		address of the interruption
 * @param callback:		callback address
 */
void Thread::finishPrepareInterruption(uintptr_t address, uintptr_t callback)
{
	// append the waiter that does interruption
	waitManager = 0;
	wait(new WaiterPerformInterruption(address, callback));

	// the next time this thread is regularly scheduled, the waiter
	// will store the state and do interruption
}

/**
 * store cpu states when called interruption
 */
void Thread::storeForInterruption()
{
	// store CPU state & it's stack location
	interruptionInfo->cpuState = *cpuState;
	interruptionInfo->cpuStateAddress = cpuState;

}

/**
 * restore the cpu state after interruption
 */
void Thread::restoreInterruptedState()
{
	// set the waiter that was on the thread before interruption
	wait(interruptionInfo->waitManager);

	// restore CPU state
	cpuState = interruptionInfo->cpuStateAddress;
	*cpuState = interruptionInfo->cpuState;

	// remove interruption info
	delete interruptionInfo;
	interruptionInfo = 0;
	DEBUG_INTERFACE_TASK_SET_STATUS(this->id, "quitting-interruption");
}
