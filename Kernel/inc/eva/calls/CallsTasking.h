/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
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

#ifndef EVA_API_CALLS_TASKINGCALLS
#define EVA_API_CALLS_TASKINGCALLS

#include "eva/kernel.h"
#include "eva/system.h"

__BEGIN_C

/**
 * @field code
 * 		the exit code
 */
typedef struct
{
	uint32_t code;
}__attribute__((packed)) SyscallExit;

/**
 * @field pid
 * 		id of the process to kill
 */
typedef struct
{
	Pid pid;

	KillStatus status;
}__attribute__((packed)) SyscallKill;

/**
 * @field id
 * 		the resulting id
 */
typedef struct
{
	Pid id;
}__attribute__((packed)) SyscallGetPid;

/**
 * @field id
 * 		the resulting id
 */
typedef struct
{
	Pid pid;

	Pid parentPid;
}__attribute__((packed)) SyscallGetParentPid;


/**
 * @field id
 * 		the resulting id
 */
typedef struct
{
	Tid id;
}__attribute__((packed)) SyscallGetTid;

/*
 * @field tid
 *		the provided id
 *
 * @field identifier
 *		the identifier returned
 */
typedef struct
{
	char identifier[STRING_IDENTIFIER_MAX_LENGTH];
	Tid id;
}__attribute__((packed)) SyscallGetIdentifier;

/**
 * @field id
 * 		the resulting id
 */
typedef struct
{
	Tid tid;

	Pid pid;
}__attribute__((packed)) SyscallGetPidForTid;

/**
 * @field milliseconds
 * 		the number of milliseconds to sleep
 */
typedef struct
{
	uint64_t milliseconds;
}__attribute__((packed)) SyscallSleep;

/**
 * @field irq
 * 		the IRQ to wait for
 */
typedef struct
{
	uint8_t irq;
}__attribute__((packed)) SyscallWaitForIrq;

/**
 * @field atom#
 * 		the atoms
 *
 * @field setOnFinish
 * 		whether or not to set the atom once finished
 *
 * @field tryOnly
 * 		whether or not to only try setting the atom
 *
 * @field wasSet
 * 		whether the atom was set, when trying only
 */
typedef struct
{
	uint8_t *atom1;
	uint8_t *atom2;
	uint8_t setOnFinish : 1;
	uint8_t tryOnly : 1;
	uint8_t wasSet : 1;
}__attribute__((packed)) SyscallAtomicLock;

/**
 * @field identifier
 * 		the identifier
 *
 * @field successful
 * 		whether the registration was successful
 */
typedef struct
{
	char *identifier;
	uint8_t success;
	char selectedID[STRING_IDENTIFIER_MAX_LENGTH];
}__attribute__((packed)) SyscallTaskIDRegister;

/**
 * @field identifier
 * 		the identifier
 *
 * @field resultTaskId
 * 		the task id, or -1 if not successful
 */
typedef struct
{
	char *identifier;
	Tid resultTaskID;
}__attribute__((packed)) SyscallTaskIDGet;

/**
 * @field code
 * 		the exit code
 */
typedef struct
{
	uint64_t millis;
}__attribute__((packed)) SyscallMillis;

/**
 * @field forkedID
 * 		id of the forked process
 */
typedef struct
{
	Pid forkedID;
}__attribute__((packed)) SyscallFork;

/**
 * @field taskId
 * 		id of the task to wait for
 */
typedef struct
{
	Tid taskID;
}__attribute__((packed)) SyscallJoin;

/**
 * @field irq
 * 		irq to register for
 * @field handler
 * 		address of the handler function
 * @field callback
 * 		address of the function to be returned to when
 * 		the handler function returns
 * @field status
 * 		result of the command
 */
typedef struct
{
	uint8_t irq;
	uintptr_t handler;
	uintptr_t callback;
	RegisterIrqHandlerStatus status;
}__attribute__((packed)) SyscallRegisterIrqHandler;

/**
 * @field signal
 * 		signal to register for
 * @field handler
 * 		address of the handler function
 * @field callback
 * 		address of the function to be returned to when
 * 		the handler function returns
 * @field previous_handler
 * 		address of the previously registered handler
 * @field status
 * 		result of the command
 */
typedef struct
{
	int signal;
	uintptr_t handler;
	uintptr_t callback;
	uintptr_t previousHandler;
	RegisterSignalHandlerStatus status;
}__attribute__((packed)) SyscallRegisterSignalHandler;

/**
 * @field signal
 * 		signal to raise
 * @field process
 * 		target process
 * @field status
 * 		result of the command
 */
typedef struct
{
	int signal;
	Pid process;
	RaiseSignalStatus status;
}__attribute__((packed)) SyscallRaiseSignal;

__END_C

#endif
