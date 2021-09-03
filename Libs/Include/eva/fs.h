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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef EVA_SHARED_FILESYSTEM_FSSTD
#define EVA_SHARED_FILESYSTEM_FSSTD

#include <eva/common.h>
#include <eva/stdint.h>
#include <eva/types.h>

__BEGIN_C

/**
 * Types
 */
typedef int32_t  File_t;   // a file descriptor
typedef uint32_t FsVirtID; // a vfs node id
typedef uint64_t FsPhysID; // a physical filesystem node identifier

#define FD_NONE ((File_t)-1)

/**
 * Limit constants
 */
#define PATH_MAX     4096
#define FILENAME_MAX 512

/**
 * File mode flags
 */
#define FILE_FLAGS_MODE_RANGE    (0xFFFF) // flags are in this range
#define FILE_FLAG_MODE_READ      (1 << 0) // read mode
#define FILE_FLAG_MODE_WRITE     (1 << 1) // write mode
#define FILE_FLAG_MODE_BINARY    (1 << 2) // binary mode
#define FILE_FLAG_MODE_TEXTUAL   (1 << 3) // textual mode
#define FILE_FLAG_MODE_TRUNCATE  (1 << 4) // truncate mode
#define FILE_FLAG_MODE_APPEND    (1 << 5) // append mode
#define FILE_FLAG_MODE_CREATE    (1 << 6) // create mode
#define FILE_FLAG_MODE_EXCLUSIVE (1 << 7) // exclusive mode
// currently, 16 bits are reserved for flags. adjust mode mask if necessary

/**
 * Seek modes
 */
typedef int32_t         FsSeekMode;
static const FsSeekMode FS_SEEK_SET = 0; /* set absolute offset */
static const FsSeekMode FS_SEEK_CUR = 1; /* set to current offset plus amount */
static const FsSeekMode FS_SEEK_END = 2; /* set offset to EOF plus offset */

/**
 * Types of filesystem nodes
 */
typedef int             FsNodeType;
static const FsNodeType FS_NODE_TYPE_NONE       = 0;
static const FsNodeType FS_NODE_TYPE_ROOT       = 1;
static const FsNodeType FS_NODE_TYPE_MOUNTPOINT = 2;
static const FsNodeType FS_NODE_TYPE_FOLDER     = 3;
static const FsNodeType FS_NODE_TYPE_FILE       = 4;
static const FsNodeType FS_NODE_TYPE_PIPE       = 5;

/**
 * Stat attributes
 */
typedef struct {
    uint32_t mode;
} __attribute__((packed)) FsStatAttributes;

/**
 * Create delegate status
 */
typedef int32_t                         FsRegisterAsDelegateStatus;
static const FsRegisterAsDelegateStatus FS_REGISTER_AS_DELEGATE_SUCCESSFUL               = 0;
static const FsRegisterAsDelegateStatus FS_REGISTER_AS_DELEGATE_FAILED_EXISTING          = 1;
static const FsRegisterAsDelegateStatus FS_REGISTER_AS_DELEGATE_FAILED_DELEGATE_CREATION = 2;

/**
 * Transaction IDs
 */
typedef uint64_t             FsTransactionID;
static const FsTransactionID FS_TRANSACTION_NO_REPEAT_ID = -1;

/**
 * Status codes for transactions
 */
typedef int                      FsTransactionStatus;
static const FsTransactionStatus FS_TRANSACTION_WAITING
    = 0; // transaction is waiting for the delegate
static const FsTransactionStatus FS_TRANSACTION_FINISHED = 1; // transaction is finished
static const FsTransactionStatus FS_TRANSACTION_REPEAT   = 2; // transaction must call handler again

/**
 * Status codes for the {fsCreateNode} system call
 */
typedef int                     FsCreateNodeStatus;
static const FsCreateNodeStatus FS_CREATE_NODE_STATUS_CREATED          = 0;
static const FsCreateNodeStatus FS_CREATE_NODE_STATUS_UPDATED          = 1;
static const FsCreateNodeStatus FS_CREATE_NODE_STATUS_FAILED_NO_PARENT = 2;

/**
 * Status codes for internal use during discovery
 */
typedef int                    FsDiscoveryStatus;
static const FsDiscoveryStatus FS_DISCOVERY_SUCCESSFUL = 0;
static const FsDiscoveryStatus FS_DISCOVERY_NOT_FOUND  = 1;
static const FsDiscoveryStatus FS_DISCOVERY_BUSY       = 2;
static const FsDiscoveryStatus FS_DISCOVERY_ERROR      = 3;

/**
 * Types of request messages that the kernel might send to a tasked fs delegate
 */
typedef int                              FsTaskedDelegateRequestType;
static const FsTaskedDelegateRequestType FS_TASKED_DELEGATE_REQUEST_TYPE_DISCOVER       = 0;
static const FsTaskedDelegateRequestType FS_TASKED_DELEGATE_REQUEST_TYPE_READ           = 1;
static const FsTaskedDelegateRequestType FS_TASKED_DELEGATE_REQUEST_TYPE_WRITE          = 2;
static const FsTaskedDelegateRequestType FS_TASKED_DELEGATE_REQUEST_TYPE_GET_LENGTH     = 3;
static const FsTaskedDelegateRequestType FS_TASKED_DELEGATE_REQUEST_TYPE_READ_DIRECTORY = 4;
static const FsTaskedDelegateRequestType FS_TASKED_DELEGATE_REQUEST_TYPE_OPEN           = 5;
static const FsTaskedDelegateRequestType FS_TASKED_DELEGATE_REQUEST_TYPE_CLOSE          = 6;

/**
 * Status codes for the {fsOpen} system call
 */
typedef int               FsOpenStatus;
static const FsOpenStatus FS_OPEN_SUCCESSFUL = 0;
static const FsOpenStatus FS_OPEN_NOT_FOUND  = 1;
static const FsOpenStatus FS_OPEN_ERROR      = 2;
static const FsOpenStatus FS_OPEN_BUSY       = 3;

/**
 * Status codes for the {fsRead} system call
 */
typedef int               FsReadStatus;
static const FsReadStatus FS_READ_SUCCESSFUL = 0;
static const FsReadStatus FS_READ_INVALID_FD = 1;
static const FsReadStatus FS_READ_BUSY       = 2;
static const FsReadStatus FS_READ_ERROR      = 3;

/**
 * Status codes for the {fsWrite} system call
 */
typedef int                FsWriteStatus;
static const FsWriteStatus FS_WRITE_SUCCESSFUL    = 0;
static const FsWriteStatus FS_WRITE_INVALID_FD    = 1;
static const FsWriteStatus FS_WRITE_NOT_SUPPORTED = 2;
static const FsWriteStatus FS_WRITE_BUSY          = 3;
static const FsWriteStatus FS_WRITE_ERROR         = 4;

/**
 * Status codes for the {fsClose} system call
 */
typedef int                FsCloseStatus;
static const FsCloseStatus FS_CLOSE_SUCCESSFUL = 0;
static const FsCloseStatus FS_CLOSE_INVALID_FD = 1;
static const FsCloseStatus FS_CLOSE_BUSY       = 2;
static const FsCloseStatus FS_CLOSE_ERROR      = 3;

/**
 * Status codes for the  {fsSeek} system call
 */
typedef int               FsSeekStatus;
static const FsSeekStatus FS_SEEK_SUCCESSFUL = 0;
static const FsSeekStatus FS_SEEK_INVALID_FD = 1;
static const FsSeekStatus FS_SEEK_ERROR      = 2;

/**
 * Status codes for the {fsTell} system call
 */
typedef int               FsTellStatus;
static const FsTellStatus FS_TELL_SUCCESSFUL = 0;
static const FsTellStatus FS_TELL_INVALID_FD = 1;

/**
 * Status codes for the {fsLength} system call
 */
typedef int                 FsLengthStatus;
static const FsLengthStatus FS_LENGTH_SUCCESSFUL = 0;
static const FsLengthStatus FS_LENGTH_INVALID_FD = 1;
static const FsLengthStatus FS_LENGTH_NOT_FOUND  = 2;
static const FsLengthStatus FS_LENGTH_BUSY       = 3;
static const FsLengthStatus FS_LENGTH_ERROR      = 4;

/**
 * Status codes for the {fsClonefd} system call
 */
typedef int                  FsClonefdStatus;
static const FsClonefdStatus FS_CLONEFD_SUCCESSFUL        = 0;
static const FsClonefdStatus FS_CLONEFD_INVALID_SOURCE_FD = 1;
static const FsClonefdStatus FS_CLONEFD_ERROR             = 2;

/**
 * Status codes for the {fsPipe} system call
 */
typedef int               FsPipeStatus;
static const FsPipeStatus FS_PIPE_SUCCESSFUL = 0;
static const FsPipeStatus FS_PIPE_ERROR      = 1;

/**
 * Status codes for the {setWorkingDirectory} system call
 */
typedef int                            SetWorkingDirectoryStatus;
static const SetWorkingDirectoryStatus SET_WORKING_DIRECTORY_SUCCESSFUL   = 0;
static const SetWorkingDirectoryStatus SET_WORKING_DIRECTORY_NOT_A_FOLDER = 1;
static const SetWorkingDirectoryStatus SET_WORKING_DIRECTORY_NOT_FOUND    = 2;
static const SetWorkingDirectoryStatus SET_WORKING_DIRECTORY_ERROR        = 3;

/**
 * Status codes for the {getWorkingDirectory} system call
 */
typedef int                            GetWorkingDirectoryStatus;
static const GetWorkingDirectoryStatus GET_WORKING_DIRECTORY_SUCCESSFUL    = 0;
static const GetWorkingDirectoryStatus GET_WORKING_DIRECTORY_SIZE_EXCEEDED = 1;
static const GetWorkingDirectoryStatus GET_WORKING_DIRECTORY_ERROR         = 2;

/**
 * Status codes & structures for directory reading
 */
typedef int                        FsOpenDirectoryStatus;
static const FsOpenDirectoryStatus FS_OPEN_DIRECTORY_SUCCESSFUL = 0;
static const FsOpenDirectoryStatus FS_OPEN_DIRECTORY_NOT_FOUND  = 1;
static const FsOpenDirectoryStatus FS_OPEN_DIRECTORY_ERROR      = 2;

typedef int                        FsReadDirectoryStatus;
static const FsReadDirectoryStatus FS_READ_DIRECTORY_SUCCESSFUL = 0;
static const FsReadDirectoryStatus FS_READ_DIRECTORY_EOD        = 1;
static const FsReadDirectoryStatus FS_READ_DIRECTORY_ERROR      = 2;

typedef int                           FsDirectoryRefreshStatus;
static const FsDirectoryRefreshStatus FS_DIRECTORY_REFRESH_SUCCESSFUL = 0;
static const FsDirectoryRefreshStatus FS_DIRECTORY_REFRESH_ERROR      = 1;
static const FsDirectoryRefreshStatus FS_DIRECTORY_REFRESH_BUSY       = 2;

typedef struct {
    FsVirtID   nodeID;
    FsNodeType type;
    char*      name;
} FsDirectoryEntry;

typedef struct {
    FsVirtID         nodeID;
    int              position;
    FsDirectoryEntry entryBuffer;
} FsDirectoryIterator;

/**
 * Transaction storage structures (NOTE limited to 1 page!)
 */
typedef struct {
    FsPhysID parentPhysFsID;
    char     name[FILENAME_MAX];

    FsDiscoveryStatus resultStatus;
} FsTaskedDelegateTransactionStorageDiscovery;

typedef struct {
    FsPhysID       physFsID;
    void*          mappedBuffer;
    int64_t        offset;
    int64_t        length;
    VirtualAddress mappingStart;
    int32_t        mappingPages;

    int64_t      resultRead;
    FsReadStatus resultStatus;
} FsTaskedDelegateTransactionStorageRead;

typedef struct {
    FsPhysID       physFsID;
    void*          mappedBuffer;
    int64_t        offset;
    int64_t        length;
    VirtualAddress mappingStart;
    int32_t        mappingPages;

    int64_t      resultWrite;
    FsReadStatus resultStatus;
} FsTaskedDelegateTransactionStorageWrite;

typedef struct {
    FsPhysID physFsID;

    int64_t      resultLength;
    FsReadStatus resultStatus;
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

__END_C

#endif
