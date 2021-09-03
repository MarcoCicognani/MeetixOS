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

#ifndef __EVA_SYS_KERNEL__
#define __EVA_SYS_KERNEL__

#include <eva/common.h>
#include <eva/stdint.h>
#include <stdarg.h>
#include <stddef.h>

__BEGIN_C

/**
 * Type of a process creation identifier
 */
typedef void* ProcessCreationIdentifier;

/**
 * Thread & process ids
 */
typedef int32_t Tid;
typedef Tid     Pid;

/**
 * Process execution security levels
 */
typedef uint8_t SecurityLevel;
#define SECURITY_LEVEL_KERNEL      0
#define SECURITY_LEVEL_DRIVER      1
#define SECURITY_LEVEL_SERVER      2
#define SECURITY_LEVEL_APPLICATION 3

/**
 * Required by the System V ABI for x86 to have thread-local-storage.
 */
typedef struct _UserThread {
    struct _UserThread* self;
} UserThread;

/**
 * VM86 related
 */
typedef uint8_t Vm86CallStatus;
#define VM86_CALL_STATUS_SUCCESSFUL           ((Vm86CallStatus)0)
#define VM86_CALL_STATUS_FAILED_NOT_PERMITTED ((Vm86CallStatus)1)

typedef struct {
    uint16_t ax;
    uint16_t bx;
    uint16_t cx;
    uint16_t dx;
    uint16_t si;
    uint16_t di;
    uint16_t ds;
    uint16_t es;
} __attribute__((packed)) Vm86Registers;

/**
 * Task types
 */
typedef uint8_t         ThreadType;
static const ThreadType THREAD_TYPE_MAIN = 1;
static const ThreadType THREAD_TYPE_SUB  = 2;
static const ThreadType THREAD_TYPE_VM86 = 4;

/**
 * Task priority
 */
typedef uint8_t             ThreadPriority;
static const ThreadPriority THREAD_PRIORITY_NORMAL = 0;
static const ThreadPriority THREAD_PRIORITY_IDLE   = 1;

/**
 * Task setup constants
 */
#define THREAD_USER_STACK_RESERVED_VIRTUAL_PAGES 16

/**
 * Pipes
 */
#define PIPE_DEFAULT_CAPACITY 0x400

__END_C

#endif
