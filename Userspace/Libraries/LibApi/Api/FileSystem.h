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
#include <Api/Types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Types
 */
typedef int FileHandle; /* Open file handle */
typedef unsigned int FsVirtID;   /* VFS node id */
typedef unsigned long long FsPhysID;   /* Filesystem node id */

#define FD_NONE ((FileHandle)-1)

/**
 * @brief Limits
 */
#define PATH_MAX     4096
#define FILENAME_MAX 512

/**
 * @brief File mode flags
 */
#define FILE_FLAGS_MODE_RANGE    (0xFFFF) /* Flags are in this range */
#define FILE_FLAG_MODE_READ      (1 << 0) /* Read mode */
#define FILE_FLAG_MODE_WRITE     (1 << 1) /* Write mode */
#define FILE_FLAG_MODE_BINARY    (1 << 2) /* Binary mode */
#define FILE_FLAG_MODE_TEXTUAL   (1 << 3) /* Textual mode */
#define FILE_FLAG_MODE_TRUNCATE  (1 << 4) /* Truncate mode */
#define FILE_FLAG_MODE_APPEND    (1 << 5) /* Append mode */
#define FILE_FLAG_MODE_CREATE    (1 << 6) /* Create mode */
#define FILE_FLAG_MODE_EXCLUSIVE (1 << 7) /* Exclusive mode */
/* currently, 16 bits are reserved for flags. adjust mode mask if necessary */

/**
 * @brief s_seek modes
 */
typedef enum {
    FS_SEEK_SET, /* Set absolute offset */
    FS_SEEK_CUR, /* Set to current offset plus amount */
    FS_SEEK_END  /* Set offset to EOF plus offset */
} FsSeekMode;

/**
 * @brief Types of filesystem nodes
 */
typedef enum {
    FS_NODE_TYPE_NONE,
    FS_NODE_TYPE_ROOT,
    FS_NODE_TYPE_MOUNTPOINT,
    FS_NODE_TYPE_FOLDER,
    FS_NODE_TYPE_FILE,
    FS_NODE_TYPE_PIPE
} FsNodeType;

/**
 * @brief Stat attributes
 */
typedef struct {
    unsigned int mode;
} A_PACKED FsStatAttributes;

/**
 * @brief Filesystem delegate creation status
 */
typedef enum {
    FS_REGISTER_AS_DELEGATE_SUCCESSFUL,
    FS_REGISTER_AS_DELEGATE_FAILED_EXISTING,
    FS_REGISTER_AS_DELEGATE_FAILED_DELEGATE_CREATION
} FsRegisterAsDelegateStatus;

/**
 * @brief Transaction IDs
 */
typedef long long                  FsTransactionID;
static const FsTransactionID FS_TRANSACTION_NO_REPEAT_ID = -1;

/**
 * @brief Status codes for transactions
 */
typedef enum {
    FS_TRANSACTION_WAITING,  /* transaction is waiting for the delegate */
    FS_TRANSACTION_FINISHED, /* transaction is finished */
    FS_TRANSACTION_REPEAT    /* transaction must call handler again */
} FsTransactionStatus;

/**
 * @brief Status codes for the CreateNode system call
 */
typedef enum {
    FS_CREATE_NODE_STATUS_CREATED,
    FS_CREATE_NODE_STATUS_UPDATED,
    FS_CREATE_NODE_STATUS_FAILED_NO_PARENT
} FsCreateNodeStatus;

/**
 * @brief Status codes for internal use during discovery
 */
typedef enum {
    FS_DISCOVERY_SUCCESSFUL,
    FS_DISCOVERY_NOT_FOUND,
    FS_DISCOVERY_BUSY,
    FS_DISCOVERY_ERROR
} FsDiscoveryStatus;

/**
 * @brief Types of request messages that the kernel might send to a tasked fs delegate
 */
typedef enum {
    FS_TASKED_DELEGATE_REQUEST_TYPE_DISCOVER,
    FS_TASKED_DELEGATE_REQUEST_TYPE_READ,
    FS_TASKED_DELEGATE_REQUEST_TYPE_WRITE,
    FS_TASKED_DELEGATE_REQUEST_TYPE_GET_LENGTH,
    FS_TASKED_DELEGATE_REQUEST_TYPE_READ_DIRECTORY,
    FS_TASKED_DELEGATE_REQUEST_TYPE_OPEN,
    FS_TASKED_DELEGATE_REQUEST_TYPE_CLOSE
} FsTaskedDelegateRequestType;

/**
 * @brief Status codes for the {fsOpen} system call
 */
typedef enum {
    FS_OPEN_SUCCESSFUL,
    FS_OPEN_NOT_FOUND,
    FS_OPEN_ERROR,
    FS_OPEN_BUSY
} FsOpenStatus;

/**
 * @brief Status codes for the {fsRead} system call
 */
typedef enum {
    FS_READ_SUCCESSFUL,
    FS_READ_INVALID_FD,
    FS_READ_BUSY,
    FS_READ_ERROR
} FsReadStatus;

/**
 * @brief Status codes for the {fsWrite} system call
 */
typedef enum {
    FS_WRITE_SUCCESSFUL,
    FS_WRITE_INVALID_FD,
    FS_WRITE_NOT_SUPPORTED,
    FS_WRITE_BUSY,
    FS_WRITE_ERROR
} FsWriteStatus;

/**
 * @brief Status codes for the {fsClose} system call
 */
typedef enum {
    FS_CLOSE_SUCCESSFUL,
    FS_CLOSE_INVALID_FD,
    FS_CLOSE_BUSY,
    FS_CLOSE_ERROR
} FsCloseStatus;

/**
 * @brief Status codes for the  {fsSeek} system call
 */
typedef enum {
    FS_SEEK_SUCCESSFUL,
    FS_SEEK_INVALID_FD,
    FS_SEEK_ERROR
} FsSeekStatus;

/**
 * @brief Status codes for the {fsTell} system call
 */
typedef enum {
    FS_TELL_SUCCESSFUL,
    FS_TELL_INVALID_FD
} FsTellStatus;

/**
 * @brief Status codes for the {fsLength} system call
 */
typedef enum {
    FS_LENGTH_SUCCESSFUL,
    FS_LENGTH_INVALID_FD,
    FS_LENGTH_NOT_FOUND,
    FS_LENGTH_BUSY,
    FS_LENGTH_ERROR
} FsLengthStatus;

/**
 * @brief Status codes for the s_clone_fd system call
 */
typedef enum {
    FS_CLONE_FD_SUCCESSFUL,
    FS_CLONE_FD_INVALID_SOURCE_FD,
    FS_CLONE_FD_ERROR
} FsCloneFdStatus;

/**
 * @brief Status codes for the {fsPipe} system call
 */
typedef enum {
    FS_PIPE_SUCCESSFUL,
    FS_PIPE_ERROR
} FsPipeStatus;

/**
 * @brief Status codes for the {setWorkingDirectory} system call
 */
typedef enum {
    SET_WORKING_DIRECTORY_SUCCESSFUL,
    SET_WORKING_DIRECTORY_NOT_A_FOLDER,
    SET_WORKING_DIRECTORY_NOT_FOUND,
    SET_WORKING_DIRECTORY_ERROR
} SetWorkingDirectoryStatus;

/**
 * @brief Status codes for the {getWorkingDirectory} system call
 */
typedef enum {
    GET_WORKING_DIRECTORY_SUCCESSFUL,
    GET_WORKING_DIRECTORY_SIZE_EXCEEDED,
    GET_WORKING_DIRECTORY_ERROR
} GetWorkingDirectoryStatus;

/**
 * @brief Status codes & structures for directory reading
 */
typedef enum {
    FS_OPEN_DIRECTORY_SUCCESSFUL,
    FS_OPEN_DIRECTORY_NOT_FOUND,
    FS_OPEN_DIRECTORY_ERROR
} FsOpenDirectoryStatus;

typedef enum {
    FS_READ_DIRECTORY_SUCCESSFUL,
    FS_READ_DIRECTORY_EOD,
    FS_READ_DIRECTORY_ERROR
} FsReadDirectoryStatus;

typedef enum {
    FS_DIRECTORY_REFRESH_SUCCESSFUL,
    FS_DIRECTORY_REFRESH_ERROR,
    FS_DIRECTORY_REFRESH_BUSY
} FsDirectoryRefreshStatus;

typedef struct {
    FsVirtID   m_node_id;
    FsNodeType m_node_type;
    char*      m_name;
} FsDirectoryEntry;

typedef struct {
    FsVirtID         m_node_id;
    unsigned int              m_position;
    FsDirectoryEntry m_entry_buffer;
} FsDirectoryIterator;

/**
 * @brief Transaction storage structures (NOTE limited to 1 page!)
 */
typedef struct {
    FsPhysID parentPhysFsID;
    char     name[FILENAME_MAX];

    FsDiscoveryStatus resultStatus;
} FsTaskedDelegateTransactionStorageDiscovery;

typedef struct {
    FsPhysID physFsID;
    void*    mappedBuffer;
    int64_t  offset;
    int64_t  length;
    VirtAddr mappingStart;
    int      mappingPages;

    int64_t      resultRead;
    FsReadStatus resultStatus;
} FsTaskedDelegateTransactionStorageRead;

typedef struct {
    FsPhysID physFsID;
    void*    mappedBuffer;
    int64_t  offset;
    int64_t  length;
    VirtAddr mappingStart;
    int      mappingPages;

    int64_t       resultWrite;
    FsWriteStatus resultStatus;
} FsTaskedDelegateTransactionStorageWrite;

typedef struct {
    FsPhysID physFsID;

    int64_t        resultLength;
    FsLengthStatus resultStatus;
} FsTaskedDelegateTransactionStorageGetLength;

typedef struct {
    FsPhysID parentPhysFsID;
    FsPhysID parentVirtFsID;

    FsDirectoryRefreshStatus resultStatus;
} FsTaskedDelegateTransactionStorageDirectoryRefresh;

typedef struct {
    FsPhysID physFsID;
    char     name[FILENAME_MAX];

    FsOpenStatus resultStatus;
} FsTaskedDelegateTransactionStorageOpen;

typedef struct {
    FsPhysID physFsID;

    FsCloseStatus resultStatus;
} FsTaskedDelegateTransactionStorageClose;

#ifdef __cplusplus
}
#endif
