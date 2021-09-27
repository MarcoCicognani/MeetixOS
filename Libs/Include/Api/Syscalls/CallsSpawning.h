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
 * @brief s_create_thread system call data
 */
typedef struct {
    void*              m_library_entry;
    void*              m_user_entry;
    void*              m_user_data;
    const char*        m_thread_name;
    CreateThreadStatus m_thread_status;
    Tid                m_thread_id;
} A_PACKED SyscallCreateThread;

/**
 * @brief s_get_thread_entry system call data
 */
typedef struct {
    void* m_user_entry;
    void* m_user_data;
} A_PACKED SyscallGetThreadEntry;

/**
 * @brief s_create_empty_process system call data
 */
typedef struct {
    SecurityLevel             m_security_level;
    ProcessCreationIdentifier m_process_creation_identifier;
} A_PACKED SyscallCreateEmptyProcess;

/**
 * @brief s_create_pages_in_space system call data
 */
typedef struct {
    ProcessCreationIdentifier m_process_creation_identifier;
    Address                   m_target_space_virtual_address;
    usize                     m_pages_count;
    Address                   m_shared_virtual_ptr;
} A_PACKED SyscallCreatePagesInSpace;

/**
 * @brief s_write_tls_master_for_process system call data
 */
typedef struct {
    ProcessCreationIdentifier m_process_creation_identifier;
    u8*                       m_tls_content;
    usize                     m_copy_size;
    usize                     m_total_size;
    usize                     m_alignment;
    bool                      m_success;
} A_PACKED SyscallWriteTlsMasterForProcess;

/**
 * @brief Process configuration struct
 */
typedef struct {
    const char* m_source_path;
} A_PACKED ProcessConfiguration;

/**
 * @brief s_configure_process system call data
 */
typedef struct {
    ProcessCreationIdentifier m_process_creation_identifier;
    ProcessConfiguration      m_process_configuration;
    bool                      m_success;
} A_PACKED SyscallConfigureProcess;

/**
 * @brief s_attach_created_process system call data
 */
typedef struct {
    Address                   m_instruction_ptr;
    ProcessCreationIdentifier m_process_creation_identifier;
} A_PACKED SyscallAttachCreatedProcess;

/**
 * @brief s_cancel_process_creation system call data
 */
typedef struct {
    ProcessCreationIdentifier m_process_creation_identifier;
} A_PACKED SyscallCancelProcessCreation;

/**
 * @brief s_get_created_process_id system call data
 */
typedef struct {
    ProcessCreationIdentifier m_process_creation_identifier;
    Pid                       m_proc_id;
} A_PACKED SyscallGetCreatedProcessID;

/**
 * @brief s_cli_args_store system call data
 */
typedef struct {
    ProcessCreationIdentifier m_process_creation_identifier;
    const char*               m_arguments;
} A_PACKED SyscallCliArgsStore;

/**
 * @brief s_cli_args_release system call data
 */
typedef struct {
    char* m_out_buffer;
} A_PACKED SyscallCliArgsRelease;

__END_C
