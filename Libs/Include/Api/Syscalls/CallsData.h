/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <Api/Common.h>
#include <Api/Syscalls/CallsFilesystem.h>
#include <Api/Syscalls/CallsLog.h>
#include <Api/Syscalls/CallsMemory.h>
#include <Api/Syscalls/CallsMessaging.h>
#include <Api/Syscalls/CallsServer.h>
#include <Api/Syscalls/CallsSpawning.h>
#include <Api/Syscalls/CallsTasking.h>
#include <Api/Syscalls/CallsVm86.h>

__BEGIN_C

/**
 * @brief System call numbers
 */
enum SyscallNumber
{
    /**
     * @brief Thread management system calls
     */
    SYSCALL_THREAD_CREATE,
    SYSCALL_THREAD_GET_ENTRY,
    SYSCALL_THREAD_JOIN,
    SYSCALL_THREAD_YIELD,
    SYSCALL_THREAD_GET_ID,
    SYSCALL_THREAD_GET_NAME,
    SYSCALL_THREAD_SET_NAME,
    SYSCALL_THREAD_GET_ID_FOR_NAME,
    SYSCALL_THREAD_SLEEP,
    SYSCALL_THREAD_EXIT,
    SYSCALL_THREAD_KILL,
    SYSCALL_EXIT_THREAD,

    /**
     * @brief Process management system calls
     */
    SYSCALL_PROCESS_FORK,
    SYSCALL_PROCESS_GET_ID,
    SYSCALL_PROCESS_GET_PARENT_ID,
    SYSCALL_PROCESS_GET_ID_FOR_THREAD_ID,
    SYSCALL_PROCESS_CALL_VM86,
    SYSCALL_PROCESS_STORE_CLI_ARGUMENTS,
    SYSCALL_PROCESS_RELEASE_CLI_ARGUMENTS,

    /**
     * @brief Scheduler system calls
     */
    SYSCALL_SCHEDULER_GET_MILLISECONDS,

    /**
     * @brief Signals management system calls
     */
    SYSCALL_SIGNAL_RAISE,
    SYSCALL_SIGNAL_REGISTER_HANDLER,
    SYSCALL_SIGNAL_IRQ_REGISTER_HANDLER,
    SYSCALL_SIGNAL_IRQ_WAIT,
    SYSCALL_SIGNAL_RESTORE_STATE,

    /**
     * @brief Lock synchronization
     */
    SYSCALL_LOCK_ATOMIC,

    /**
     * @brief Process creation/configuration system calls
     */
    SYSCALL_PROCESS_CONFIGURE,
    SYSCALL_PROCESS_CREATE_EMPTY,
    SYSCALL_CREATE_PAGES_IN_SPACE,
    SYSCALL_GET_CREATED_PROCESS_ID,
    SYSCALL_ATTACH_CREATED_PROCESS,
    SYSCALL_CANCEL_PROCESS_CREATION,
    SYSCALL_WRITE_TLS_MASTER_FOR_PROCESS,

    /**
     * @brief IPC messages system calls
     */
    SYSCALL_MESSAGE_SEND,
    SYSCALL_MESSAGE_RECEIVE,

    /**
     * @brief Memory management system calls
     */
    SYSCALL_MEMORY_SBRK,
    SYSCALL_MEMORY_UNMAP,
    SYSCALL_MEMORY_SHARE,
    SYSCALL_MEMORY_MAP_MMIO,
    SYSCALL_MEMORY_ALLOCATE,
    SYSCALL_MEMORY_LOWER_FREE,
    SYSCALL_MEMORY_LOWER_ALLOCATE,

    /**
     * Syscalls for serial s_log management
     */
    SYSCALL_LOG,
    SYSCALL_LOG_TOGGLE_VIDEO,

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

    /* TODO REMOVE THESE */
    SYSCALL_SERVER_MANAGE,
    SYSCALL_REGISTER_AS_SERVER,

    SYSCALL_COUNT
};

__END_C
