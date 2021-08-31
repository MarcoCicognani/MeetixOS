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

#ifndef EVA_API_CALLS_CALLS
#define EVA_API_CALLS_CALLS

#include "eva/common.h"
#include "eva/calls/CallsMemory.h"
#include "eva/calls/CallsLog.h"
#include "eva/calls/CallsMessaging.h"
#include "eva/calls/CallsRamdisk.h"
#include "eva/calls/CallsSpawning.h"
#include "eva/calls/CallsTasking.h"
#include "eva/calls/CallsVm86.h"
#include "eva/calls/CallsServer.h"
#include "eva/calls/CallsFilesystem.h"

__BEGIN_C

/**
 * Defines a enumaration for the syscalls codes
 */
enum
{
    /**
     * Syscalls for Process/Thread interaction
     */
    SYSCALL_JOIN,
    SYSCALL_YIELD,
    SYSCALL_CALL_VM86,
    SYSCALL_GET_THREAD_ID,
    SYSCALL_GET_PROCESS_ID,
    SYSCALL_GET_THREAD_NAME,
    SYSCALL_GET_MILLISECONDS,
    SYSCALL_REGISTER_THREAD_NAME,
    SYSCALL_GET_PARENT_PROCESS_ID,
    SYSCALL_GET_THREAD_ID_FOR_NAME,
    SYSCALL_GET_PROCESS_ID_FOR_THREAD_ID,

    /**
     * Syscalls for kernel server/drivers
     */
    SYSCALL_SERVER_MANAGE,
    SYSCALL_REGISTER_AS_SERVER,

    /**
     * Syscalls for Process/Thread terminations
     */
    SYSCALL_EXIT,
    SYSCALL_KILL,
    SYSCALL_EXIT_THREAD,

    /**
     * Syscalls for signals and interrupts managing
     */
    SYSCALL_RAISE_SIGNAL,
    SYSCALL_REGISTER_IRQ_HANDLER,
    SYSCALL_REGISTER_SIGNAL_HANDLER,
    SYSCALL_RESTORE_INTERRUPTED_STATE,

    /**
     * Syscalls for Thread waiting
     */
    SYSCALL_SLEEP,
    SYSCALL_ATOMIC_LOCK,
    SYSCALL_WAIT_FOR_IRQ,

    /**
     * Syscalls for Process arguments management
     */
    SYSCALL_STORE_CLI_ARGUMENTS,
    SYSCALL_RELEASE_CLI_ARGUMENTS,

    /**
     * Syscalls for Thread/Process creations
     */
    SYSCALL_FORK,
    SYSCALL_RAMDISK_SPAWN,
    SYSCALL_CREATE_THREAD,
    SYSCALL_GET_THREAD_ENTRY,
    SYSCALL_CONFIGURE_PROCESS,
    SYSCALL_CREATE_EMPTY_PROCESS,
    SYSCALL_CREATE_PAGES_IN_SPACE,
    SYSCALL_GET_CREATED_PROCESS_ID,
    SYSCALL_ATTACH_CREATED_PROCESS,
    SYSCALL_CANCEL_PROCESS_CREATION,
    SYSCALL_WRITE_TLS_MASTER_FOR_PROCESS,

    /**
     * Syscalls for Thread/Process messaging
     */
    SYSCALL_MESSAGE_SEND,
    SYSCALL_MESSAGE_RECEIVE,

    /**
     * Syscalls for memory management
     */
    SYSCALL_SBRK,
    SYSCALL_UNMAP,
    SYSCALL_SHARE_MEMORY,
    SYSCALL_MAP_MMIO_AREA,
    SYSCALL_ALLOCATE_MEMORY,
    SYSCALL_LOWER_MEMORY_FREE,
    SYSCALL_LOWER_MEMORY_ALLOCATE,

    /**
     * Syscalls for serial log management
     */
    SYSCALL_LOG,
    SYSCALL_SET_VIDEO_LOG,

    /**
     * Syscalls for direct ramdisk interaction
     */
    SYSCALL_RAMDISK_FIND,
    SYSCALL_RAMDISK_INFO,
    SYSCALL_RAMDISK_READ,
    SYSCALL_RAMDISK_CHILD_AT,
    SYSCALL_RAMDISK_FIND_CHILD,
    SYSCALL_RAMDISK_CHILD_COUNT,

    /**
     * Syscalls for File operations
     */
    SYSCALL_FS_OPEN,
    SYSCALL_FS_READ,
    SYSCALL_FS_STAT,
    SYSCALL_FS_CLOSE,
    SYSCALL_FS_FSTAT,
    SYSCALL_FS_WRITE,
    SYSCALL_FS_LENGTH,

    /**
     * Syscalls for File System operation
     */
    SYSCALL_FS_PIPE,
    SYSCALL_FS_SEEK,
    SYSCALL_FS_TELL,
    SYSCALL_FS_CLONEFD,
    SYSCALL_FS_OPEN_DIRECTORY,
    SYSCALL_FS_READ_DIRECTORY,

    /**
     * Syscalls for File System userspace driver
     */
    SYSCALL_FS_CREATE_NODE,
    SYSCALL_FS_REGISTER_AS_DELEGATE,
    SYSCALL_FS_SET_TRANSACTION_STATUS,

    /**
     * Syscalls for Path utils
     */
    SYSCALL_GET_EXECUTABLE_PATH,
    SYSCALL_GET_WORKING_DIRECTORY,
    SYSCALL_SET_WORKING_DIRECTORY,

    /**
     * Syscalls for System infos
     */
    SYSCALL_ENAME,
    SYSCALL_SYSINFO,
    SYSCALL_GET_PCI_COUNT,
    SYSCALL_GET_PCI_DEVICE,

    /**
     * Syscalls for Thread infos
     */
    SYSCALL_GET_THREAD_COUNT,
    SYSCALL_GET_THREAD_IDS_LIST,
    SYSCALL_GET_THREAD_DESCRIPTOR,

    /**
     * Syscalls for Process infos
     */
    SYSCALL_GET_PROCESS_COUNT,
    SYSCALL_GET_PROCESS_IDS_LIST,
    SYSCALL_GET_PROCESS_DESCRIPTOR,

    // represent the avaible count of syscalls
    SYSCALL_COUNT
};

__END_C

#endif
