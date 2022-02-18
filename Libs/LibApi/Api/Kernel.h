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
#include <Api/StdInt.h>
#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque pointer used by the kernel and the spawner to create new processes
 */
typedef void* ProcessCreationIdentifier;

/**
 * @brief Thread/Process unique identifier
 */
typedef i32 Tid;
typedef Tid Pid;

/**
 * @brief Process security levels
 */
typedef enum {
    SECURITY_LEVEL_KERNEL,
    SECURITY_LEVEL_DRIVER,
    SECURITY_LEVEL_SERVER,
    SECURITY_LEVEL_APPLICATION
} SecurityLevel;

/**
 * @brief Required by the System V ABI for x86 to have thread-local-storage
 */
typedef struct UserThread {
    struct UserThread* m_self;
} UserThread;

/**
 * @brief VM86 call statuses
 */
typedef enum {
    VM86_CALL_STATUS_SUCCESSFUL,
    VM86_CALL_STATUS_FAILED_NOT_PERMITTED
} Vm86CallStatus;

/**
 * @brief VM86 Registers
 */
typedef struct {
    u16 ax;
    u16 bx;
    u16 cx;
    u16 dx;
    u16 si;
    u16 di;
    u16 ds;
    u16 es;
} A_PACKED VM86Registers;

/**
 * @brief Thread types
 */
typedef u8              ThreadType;
static const ThreadType THREAD_TYPE_MAIN = 1;
static const ThreadType THREAD_TYPE_SUB  = 2;
static const ThreadType THREAD_TYPE_VM86 = 4;

/**
 * @brief Thread priorities
 */
typedef enum {
    THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_IDLE
} ThreadPriority;

/**
 * @brief Initial thread stack size in 4KiB pages
 */
#define THREAD_USER_STACK_RESERVED_VIRTUAL_PAGES 16

/**
 * @brief Maximum size in bytes for a pipe
 */
#define PIPE_DEFAULT_CAPACITY 0x400

#ifdef __cplusplus
}
#endif
