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
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * */

#ifndef __EVA_SYS_SYSTEM__
#define __EVA_SYS_SYSTEM__

#include "eva/common.h"
#include "eva/kernel.h"
#include "eva/fs.h"

__BEGIN_C

/**
 * Required to provide the <Spawn> function. The spawning process shall
 * register itself with this identifier to be accessible via ipc messaging.
 */
#define SPAWNER_IDENTIFIER		"spawner"

// spawner commands
#define SPAWN_COMMAND_SPAWN_REQUEST		1
#define SPAWN_COMMAND_SPAWN_RESPONSE	2
#define SPAWN_COMMAND_REBOOT_MACHINE  	3
#define SPAWN_COMMAND_SHUTDOWN_MACHINE  4

// status codes for spawning
typedef uint32_t SpawnStatus;
#define SPAWN_STATUS_SUCCESSFUL 		((SpawnStatus) 0)
#define SPAWN_STATUS_IO_ERROR			((SpawnStatus) 1)
#define SPAWN_STATUS_MEMORY_ERROR		((SpawnStatus) 2)
#define SPAWN_STATUS_FORMAT_ERROR		((SpawnStatus) 3)
#define SPAWN_STATUS_UNKNOWN			((SpawnStatus) 4)

// command structs
typedef struct
{
	uint32_t command;
}__attribute__((packed)) SpawnCommandHeader;

/**
 * Rquest/Response to spawn new binary program
 */
typedef struct
{
	SpawnCommandHeader header;
	SecurityLevel securityLevel;
	size_t pathBytes;
	size_t argsBytes;
	size_t workdirBytes;
	File_t stdin;
	File_t stdout;
	File_t stderr;
}__attribute__((packed)) SpawnCommandSpawnRequest;

typedef struct
{
	SpawnStatus status;
	Pid spawnedProcessID;
	File_t stdinWrite;
	File_t stdoutRead;
	File_t stderrRead;
}__attribute__((packed)) SpawnCommandSpawnResponse;

/*
 * Request for halting machine
 */
typedef struct
{
	SpawnCommandHeader header;
}__attribute__((packed)) SpawnerRequestHealtMachine;

// process configuration buffer lengths
#define CLIARGS_BUFFER_LENGTH			1024
#define CLIARGS_SEPARATOR				0x1F // ASCII character: UNIT SEPARATOR
#define STRING_IDENTIFIER_MAX_LENGTH    1024

// for <RegisterIrqHandler>
typedef uint8_t RegisterIrqHandlerStatus;
#define REGISTER_IRQ_HANDLER_STATUS_SUCCESSFUL			((RegisterIrqHandlerStatus) 0)
#define REGISTER_IRQ_HANDLER_STATUS_NOT_PERMITTED		((RegisterIrqHandlerStatus) 1)

// for <RegisterSignalHandler>
typedef uint8_t RegisterSignalHandlerStatus;
#define REGISTER_SIGNAL_HANDLER_STATUS_SUCCESSFUL		((RegisterSignalHandlerStatus) 0)
#define REGISTER_SIGNAL_HANDLER_STATUS_INVALID_SIGNAL 	((RegisterSignalHandlerStatus) 1)

// for <RaiseSignal>
typedef uint8_t RaiseSignalStatus;
#define RAISE_SIGNAL_STATUS_SUCCESSFUL				((RaiseSignalStatus) 0)
#define RAISE_SIGNAL_STATUS_INVALID_SIGNAL 			((RaiseSignalStatus) 1)
#define RAISE_SIGNAL_STATUS_INVALID_TARGET 			((RaiseSignalStatus) 2)

// for <Kill>
typedef uint8_t KillStatus;
#define KILL_STATUS_SUCCESSFUL		((KillStatus) 0)
#define KILL_STATUS_NOT_FOUND		((KillStatus) 1)
#define KILL_STATUS_FAILED			((KillStatus) 2)

// for <CreateThread>
typedef uint8_t CreateThreadStatus;
#define CREATE_THREAD_STATUS_SUCCESSFUL		((CreateThreadStatus) 0)
#define CREATE_THREAD_STATUS_FAILED			((CreateThreadStatus) 1)

// for <RegisterAsServer>
typedef uint8_t RegisterAsServerStatus;
#define REGISTER_AS_SERVER_STATUS_UNKNOWN			 ((RegisterAsServerStatus) 0)
#define REGISTER_AS_SERVER_STATUS_SUCCESS 			 ((RegisterAsServerStatus) 1)
#define REGISTER_AS_SERVER_STATUS_NON_MAIN			 ((RegisterAsServerStatus) 2)
#define REGISTER_AS_SERVER_STATUS_ALREADY_REGISTERED ((RegisterAsServerStatus) 3)

// for <ServerManage>
typedef uint32_t ServerManageCommand;
typedef uint8_t ServerManageStatus;
#define SERVER_MANAGE_STATUS_INVALID_COMMAND		((ServerManageStatus) 0)
#define SERVER_MANAGE_STATUS_COMMAND_SEND_SUCCESS	((ServerManageStatus) 1)
#define SERVER_MANAGE_STATUS_COMMAND_SEND_FAIL		((ServerManageStatus) 2)
#define SERVER_MANAGE_STATUS_NO_RESPONSE			((ServerManageStatus) 3)
#define SERVER_MANAGE_STATUS_COMMAND_SUCCESS		((ServerManageStatus) 4)
#define SERVER_MANAGE_STATUS_SERVER_NOT_FOUND		((ServerManageStatus) 5)
#define SERVER_MANAGE_STATUS_INVALID_IDENTIFIER		((ServerManageStatus) 6)

/**
 * Server Manage buffer, used in ServerManage
 * to store the informations to send to the server
 *
 * @field data:	        the buffer for the command
 * @field length:		the size of the buffer
 * @field status:       the resultant state of the command
 * @field command:      the command to send to the server
 */
typedef struct
{
	void *data;
	uint32_t length;
	ServerManageStatus status;
	ServerManageCommand command;
}__attribute__((packed)) ServerManageBuffer;

__END_C

#endif
