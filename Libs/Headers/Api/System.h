/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <Api/Common.h>
#include <Api/FileSystem.h>
#include <Api/Kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Required to provide the <s_spawn> function. The spawning process shall register itself
 * with this identifier to be accessible via ipc messaging.
 */
#define SPAWNER_IDENTIFIER "spawner"

/**
 * @brief Spawner commands
 */
#define SPAWN_COMMAND_SPAWN_REQUEST    1
#define SPAWN_COMMAND_SPAWN_RESPONSE   2
#define SPAWN_COMMAND_REBOOT_MACHINE   3
#define SPAWN_COMMAND_SHUTDOWN_MACHINE 4

/**
 * @brief Spawner response status codes
 */
typedef enum {
    SPAWN_STATUS_SUCCESSFUL,
    SPAWN_STATUS_IO_ERROR,
    SPAWN_STATUS_MEMORY_ERROR,
    SPAWN_STATUS_FORMAT_ERROR,
    SPAWN_STATUS_UNKNOWN
} SpawnStatus;

/**
 * @brief Spawner command header
 */
typedef struct {
    u32 m_command;
} A_PACKED SpawnCommandHeader;

/**
 * @brief Request/Response to s_spawn new binary program
 */
typedef struct {
    SpawnCommandHeader m_command_header;
    SecurityLevel      m_security_level;
    usize              m_path_len;
    usize              m_args_len;
    usize              m_workdir_len;
    FileHandle         m_stdin;
    FileHandle         m_stdout;
    FileHandle         m_stderr;
} A_PACKED SpawnCommandSpawnRequest;

typedef struct {
    SpawnStatus m_spawn_status;
    Pid         m_new_process_id;
    FileHandle  m_stdin_write;
    FileHandle  m_stdout_read;
    FileHandle  m_stderr_read;
} A_PACKED SpawnCommandSpawnResponse;

/*
 * Request for halting machine
 */
typedef struct {
    SpawnCommandHeader m_command_header;
} A_PACKED SpawnerRequestHaltMachine;

/**
 * @brief Process configuration buffer lengths
 */
#define CLIARGS_BUFFER_LENGTH        1024
#define CLIARGS_SEPARATOR            0x1F // ASCII character: UNIT SEPARATOR
#define STRING_IDENTIFIER_MAX_LENGTH 1024

/**
 * @brief IRQ handler return statuses
 */
typedef enum {
    REGISTER_IRQ_HANDLER_STATUS_SUCCESSFUL,
    REGISTER_IRQ_HANDLER_STATUS_NOT_PERMITTED
} RegisterIrqHandlerStatus;

/**
 * @brief Signal handler return statuses
 */
typedef enum {
    REGISTER_SIGNAL_HANDLER_STATUS_SUCCESSFUL,
    REGISTER_SIGNAL_HANDLER_STATUS_INVALID_SIGNAL
} RegisterSignalHandlerStatus;

/**
 * @brief Raise signal return statuses
 */
typedef enum {
    RAISE_SIGNAL_STATUS_SUCCESSFUL,
    RAISE_SIGNAL_STATUS_INVALID_SIGNAL,
    RAISE_SIGNAL_STATUS_INVALID_TARGET
} RaiseSignalStatus;

/**
 * @brief s_kill return statuses
 */
typedef enum {
    KILL_STATUS_SUCCESSFUL,
    KILL_STATUS_NOT_FOUND,
    KILL_STATUS_FAILED
} KillStatus;

/**
 * @brief Create thread return status
 */
typedef enum {
    CREATE_THREAD_STATUS_SUCCESSFUL,
    CREATE_THREAD_STATUS_FAILED
} CreateThreadStatus;

// for <RegisterAsServer>
typedef u8 RegisterAsServerStatus;
#define REGISTER_AS_SERVER_STATUS_UNKNOWN            ((RegisterAsServerStatus)0)
#define REGISTER_AS_SERVER_STATUS_SUCCESS            ((RegisterAsServerStatus)1)
#define REGISTER_AS_SERVER_STATUS_NON_MAIN           ((RegisterAsServerStatus)2)
#define REGISTER_AS_SERVER_STATUS_ALREADY_REGISTERED ((RegisterAsServerStatus)3)

// for <ServerManage>
typedef u32 ServerManageCommand;
typedef u8  ServerManageStatus;
#define SERVER_MANAGE_STATUS_INVALID_COMMAND      ((ServerManageStatus)0)
#define SERVER_MANAGE_STATUS_COMMAND_SEND_SUCCESS ((ServerManageStatus)1)
#define SERVER_MANAGE_STATUS_COMMAND_SEND_FAIL    ((ServerManageStatus)2)
#define SERVER_MANAGE_STATUS_NO_RESPONSE          ((ServerManageStatus)3)
#define SERVER_MANAGE_STATUS_COMMAND_SUCCESS      ((ServerManageStatus)4)
#define SERVER_MANAGE_STATUS_SERVER_NOT_FOUND     ((ServerManageStatus)5)
#define SERVER_MANAGE_STATUS_INVALID_IDENTIFIER   ((ServerManageStatus)6)

/**
 * Server Manage buffer, used in s_server_manage
 * to store the information to send to the server
 *
 * @field data:	        the buffer for the command
 * @field length:		the size of the buffer
 * @field status:       the resultant state of the command
 * @field command:      the command to send to the server
 */
typedef struct {
    void*               data;
    uint32_t            length;
    ServerManageStatus  status;
    ServerManageCommand command;
} A_PACKED ServerManageBuffer;

#ifdef __cplusplus
}
#endif
