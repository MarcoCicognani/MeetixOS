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

#include "eva/calls/calls.h"
#include "eva/kernel.h"
#include <calls/SyscallHandler.hpp>
#include <logger/logger.hpp>

#include <EvangelionNG.hpp>
#include <ramdisk/ramdisk.hpp>
#include <tasking/thread.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <tasking/wait/WaiterCallVm86.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/physical/PPallocator.hpp>
#include <memory/physical/PPreferenceTracker.hpp>
#include <executable/Elf32Loader.hpp>

/**
 * Performs a vm86 call
 */
SYSCALL_HANDLER(callVm86)
{
	SyscallCallVm86 *data = (SyscallCallVm86*) SYSCALL_DATA(currentThread->cpuState);

	if (currentThread->process->securityLevel <= SECURITY_LEVEL_DRIVER)
	{
		// Copy in registers
		Vm86Registers in = data->in;

		// Create temporary out struct
		Vm86Registers *temporaryOut = new Vm86Registers();

		// Create task
		Thread *vm86task = ThreadManager::createProcessVm86(data->interrupt, in, temporaryOut);
		Tasking::addTask(vm86task);
		logDebug("%! task %i creates vm86 task %i to call interrupt %h", "syscall", currentThread->id, vm86task->id, data->interrupt);

		// Set wait
		currentThread->wait(new WaiterCallVm86(data, temporaryOut, vm86task->id));

		data->status = VM86_CALL_STATUS_SUCCESSFUL;
	}

	else
	{
		logWarn("%! task %i tried to do vm86 call but is not permitted", "syscall", currentThread->id);
		data->status = VM86_CALL_STATUS_FAILED_NOT_PERMITTED;
	}

	return Tasking::schedule();
}
