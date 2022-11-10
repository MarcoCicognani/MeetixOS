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

#include <Api/FileSystem.h>
#include <Api/Kernel.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief s_open system call data
 */
typedef struct {
    const char*  m_path;
    i32          m_flags;
    i32          m_mode;
    FsOpenStatus m_open_status;
    FileHandle   m_open_fd;
} A_PACKED SyscallFsOpen;

/**
 * @brief s_read system call data
 */
typedef struct {
    FileHandle   m_open_fd;
    u8*          m_out_buffer;
    usize        m_out_buffer_len;
    FsReadStatus m_read_status;
    usize        m_read_bytes;
} A_PACKED SyscallFsRead;

/**
 * @brief s_write system call data
 */
typedef struct {
    FileHandle    m_open_fd;
    const u8*     m_in_buffer;
    usize         m_in_buffer_len;
    FsWriteStatus m_write_status;
    usize         m_write_bytes;
} A_PACKED SyscallFsWrite;

/**
 * @brief s_close system call data
 */
typedef struct {
    FileHandle    m_open_fd;
    FsCloseStatus m_close_status;
} A_PACKED SyscallFsClose;

/**
 * @brief s_stat system call data
 */
typedef struct {
    const char*      m_path;
    bool             m_follow_symlinks;
    FsStatAttributes m_stat_attributes;
    usize            m_result;
} A_PACKED SyscallFsStat;

/**
 * @brief s_fstat system call data
 */
typedef struct {
    FileHandle       m_open_fd;
    FsStatAttributes m_stat_attributes;
    usize            m_result;
} A_PACKED SyscallFsFstat;

/**
 * @brief s_clone_fd system call data
 */
typedef struct {
    FileHandle      m_source_fd;
    Pid             m_source_proc_id;
    FileHandle      m_target_fd;
    Pid             m_target_proc_id;
    FsCloneFdStatus m_clone_fd_status;
    FileHandle      m_cloned_fd;
} A_PACKED SyscallFsCloneFd;

/**
 * @brief s_pipe system call data
 */
typedef struct {
    FileHandle   m_write_end_fd;
    FileHandle   m_read_end_fd;
    FsPipeStatus m_pipe_status;
} A_PACKED SyscallFsPipe;

/**
 * @brief s_length flags
 */
typedef int SyscallFsLengthMode;
#define SYSCALL_FS_LENGTH_MODE_BY_MASK        1
#define SYSCALL_FS_LENGTH_BY_PATH             ((SyscallFsLengthMode)0x0)
#define SYSCALL_FS_LENGTH_BY_FD               ((SyscallFsLengthMode)0x1)
#define SYSCALL_FS_LENGTH_MODE_SYMLINK_MASK   2
#define SYSCALL_FS_LENGTH_FOLLOW_SYMLINKS     ((SyscallFsLengthMode)0x0)
#define SYSCALL_FS_LENGTH_NOT_FOLLOW_SYMLINKS ((SyscallFsLengthMode)0x2)

/**
 * @brief s_length system call data
 */
typedef struct {
    SyscallFsLengthMode m_length_mode;
    const char*         m_path;
    FileHandle          m_open_fd;
    FsLengthStatus      m_length_status;
    i64                 m_length;
} A_PACKED SyscallFsLength;

/**
 * @brief s_seek system call data
 */
typedef struct {
    FileHandle   m_open_fd;
    i64          m_offset;
    FsSeekMode   m_seek_mode;
    FsSeekStatus m_seek_status;
    i64          m_result;
} A_PACKED SyscallFsSeek;

/**
 * @brief s_tell system call data
 */
typedef struct {
    FileHandle   m_open_fd;
    FsTellStatus m_tell_status;
    i64          m_result;
} A_PACKED SyscallFsTell;

/**
 * @brief s_set_working_directory system call data
 */
typedef struct {
    const char*               m_path;
    ProcessCreationIdentifier m_process_creation_identifier;
    SetWorkingDirectoryStatus m_working_directory_status;
} A_PACKED SyscallFsSetWorkingDirectory;

/**
 * @brief s_get_working_directory system call data
 */
typedef struct {
    char*                     m_out_buffer;
    usize                     m_out_buffer_len;
    GetWorkingDirectoryStatus m_working_directory_status;
} A_PACKED SyscallFsGetWorkingDirectory;

/**
 * @brief s_get_executable_path system call data
 */
typedef struct {
    char* m_out_buffer;
    usize m_len;
} A_PACKED SyscallFsGetExecutablePath;

/**
 * @brief s_register_as_delegate system call data
 */
typedef struct {
    const char*                m_mountpoint_name;
    FsPhysID                   m_phys_mountpoint_id;
    FsVirtID                   m_vfs_mountpoint_id;
    Address                    m_transaction_storage;
    FsRegisterAsDelegateStatus m_register_as_delegate_status;
} A_PACKED SyscallFsRegisterAsDelegate;

/**
 * @brief s_set_transaction_status system call data
 */
typedef struct {
    FsTransactionID     m_transaction_id;
    FsTransactionStatus m_transaction_status;
} A_PACKED SyscallFsSetTransactionStatus;

/**
 * @brief s_create_node system call data
 */
typedef struct {
    FsVirtID           m_vfs_parent_id;
    const char*        m_node_name;
    FsNodeType         m_node_type;
    FsPhysID           m_phys_fs_id;
    FsCreateNodeStatus m_create_node_status;
    FsVirtID           m_vfs_node_id;
} A_PACKED SyscallFsCreateNode;

/**
 * @brief s_open_directory system call data
 */
typedef struct {
    const char*           m_path;
    FsDirectoryIterator*  m_directory_iterator;
    FsOpenDirectoryStatus m_open_directory_status;
} A_PACKED SyscallFsOpenDirectory;

/**
 * @brief s_read_directory system call data
 */
typedef struct {
    FsDirectoryIterator*  m_directory_iterator;
    FsReadDirectoryStatus m_read_directory_status;
} A_PACKED SyscallFsReadDirectory;

/**
 * @brief s_close_directory system call data
 */
typedef struct {
    FsDirectoryIterator* m_directory_iterator;
} A_PACKED SyscallFsCloseDirectory;

#ifdef __cplusplus
}
#endif
