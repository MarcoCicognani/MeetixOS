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

#include <Api/Kernel.h>
#include <Api/system.h>

__BEGIN_C

/**
 * @brief s_exit system call data
 */
typedef struct {
    int m_exit_code;
} A_PACKED SyscallExit;

/**
 * @brief s_kill system call data
 */
typedef struct {
    Pid        m_proc_id;
    KillStatus m_kill_status;
} A_PACKED SyscallKill;

/**
 * @brief s_get_pid system call data
 */
typedef struct {
    Pid m_proc_id;
} A_PACKED SyscallGetPid;

/**
 * @brief s_get_parent_pid system call data
 */
typedef struct {
    Pid m_proc_id;
    Pid m_parent_proc_id;
} A_PACKED SyscallGetParentPid;

/**
 * @brief s_get_tid system call id
 */
typedef struct {
    Tid m_thread_id;
} A_PACKED SyscallGetTid;

/**
 * @brief s_get_task_identifier system call data
 */
typedef struct {
    Tid  m_thread_id;
    char m_thread_name[STRING_IDENTIFIER_MAX_LENGTH];
} A_PACKED SyscallGetIdentifier;

/**
 * @brief s_get_pid_for_tid system call data
 */
typedef struct {
    Tid m_thread_id;
    Pid m_proc_id;
} A_PACKED SyscallGetPidForTid;

/**
 * @brief s_sleep system call data
 */
typedef struct {
    usize m_ms_amount;
} A_PACKED SyscallSleep;

/**
 * @brief s_wait_for_irq system call data
 */
typedef struct {
    u8 m_irq;
} A_PACKED SyscallWaitForIrq;

/**
 * @brief s_atomic_lock system call data
 */
typedef struct {
    u8* m_atom_1;
    u8* m_atom_2;
    u8  m_set_on_finish : 1;
    u8  m_is_try        : 1;
    u8  m_was_set       : 1;
} A_PACKED SyscallAtomicLock;

/**
 * @brief s_task_register_id system call data
 */
typedef struct {
    const char* m_identifier;
    bool        m_success;
    char        m_selected_identifier[STRING_IDENTIFIER_MAX_LENGTH];
} A_PACKED SyscallTaskIDRegister;

/**
 * @brief s_task_get_id system call data
 */
typedef struct {
    const char* m_identifier;
    Tid         m_thread_id;
} A_PACKED SyscallTaskIDGet;

/**
 * @brief s_millis system call data
 */
typedef struct {
    u64 m_millis_amount;
} A_PACKED SyscallMillis;

/**
 * @brief s_fork system call data
 */
typedef struct {
    Pid m_forked_proc_id;
} A_PACKED SyscallFork;

/**
 * @brief s_join system call data
 */
typedef struct {
    Tid m_thread_id;
} A_PACKED SyscallJoin;

/**
 * @brief s_register_irq_handler system call data
 */
typedef struct {
    u8                       m_irq;
    Address                  m_user_handler;
    Address                  m_return_callback;
    RegisterIrqHandlerStatus m_register_status;
} A_PACKED SyscallRegisterIrqHandler;

/**
 * @brief s_register_signal_handler system call data
 */
typedef struct {
    int                         m_signal;
    Address                     m_user_handler;
    Address                     m_return_callback;
    Address                     m_previous_handler;
    RegisterSignalHandlerStatus m_register_status;
} A_PACKED SyscallRegisterSignalHandler;

/**
 * @brief s_raise_signal system call data
 */
typedef struct {
    int               m_signal;
    Pid               m_proc_id;
    RaiseSignalStatus m_raise_status;
} A_PACKED SyscallRaiseSignal;

__END_C
