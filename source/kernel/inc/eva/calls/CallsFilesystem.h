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

#ifndef EVA_API_CALLS_FILESYSTEMCALLS
#define EVA_API_CALLS_FILESYSTEMCALLS

#include "eva/fs.h"

__BEGIN_C

/**
 * @field path
 * 		buffer containing the file path
 *
 * @field flags
 * 		open flags
 *
 * @field status
 * 		one of the
 {FsOpenStatus} codes
 *
 * @field fd
 * 		resulting file descriptor
 *
 * @security-level APPLICATION
 */
typedef struct
{
	char *path;
	int32_t flags;
	int32_t mode;

	FsOpenStatus status;
	File_t fd;
}__attribute__((packed)) SyscallFsOpen;

/**
 * @field fd
 * 		file descriptor
 *
 * @field buffer
 * 		output buffer
 *
 * @field length
 * 		number of bytes to read
 *
 * @field status
 * 		one of the
 {FsReadStatus} codes
 *
 * @field result
 * 		number of bytes read
 *
 * @security-level APPLICATION
 */
typedef struct
{
	File_t fd;
	uint8_t *buffer;
	int64_t length;

	FsReadStatus status;
	int64_t result;
}__attribute__((packed)) SyscallFsRead;

/**
 * @field fd
 * 		file descriptor
 *
 * @field buffer
 * 		output buffer
 *
 * @field length
 * 		number of bytes to write
 *
 * @field status
 * 		one of the
 {FsWriteStatus} codes
 *
 * @field result
 * 		number of bytes write
 *
 * @security-level APPLICATION
 */
typedef struct
{
	File_t fd;
	uint8_t *buffer;
	int64_t length;

	FsWriteStatus status;
	int64_t result;
}__attribute__((packed)) SyscallFsWrite;

/**
 * @field fd
 * 		file descriptor
 *
 * @field status
 * 		one of the
 {FsCloseStatus} codes
 *
 * @security-level APPLICATION
 */
typedef struct
{
	File_t fd;

	FsCloseStatus status;
}__attribute__((packed)) SyscallFsClose;

/**
 * @field path
 * 		file path
 *
 * @field followSymlinks
 * 		whether to follow symbolic links
 *
 * @field result
 * 		the call result
 *
 * @security-level APPLICATION
 */
typedef struct
{
	char *path;
	uint8_t followSymlinks;

	FsStatAttributes stats;
	int32_t result;
}__attribute__((packed)) SyscallFsStat;

/**
 * @field fd
 * 		file descriptor
 *
 * @field followSymlinks
 * 		whether to follow symbolic links
 *
 * @field result
 * 		the call result
 *
 * @security-level APPLICATION
 */
typedef struct
{
	File_t fd;

	FsStatAttributes stats;
	int32_t result;
}__attribute__((packed)) SyscallFsFstat;

/**
 * @field sourceFd
 * 		source file descriptor
 *
 * @field sourcePid
 * 		source process id
 *
 * @field targetFd
 * 		target file descriptor
 *
 * @field targetPid
 * 		target process id
 *
 * @field status
 * 		status of the operation
 *
 * @field result
 * 		the resulting file descriptor
 *
 * @security-level APPLICATION
 */
typedef struct
{
	File_t sourceFd;
	Pid sourcePid;
	File_t targetFd;
	Pid targetPid;

	FsClonefdStatus status;
	File_t result;
}__attribute__((packed)) SyscallFsClonefd;

/**
 * @field writeFd
 * 		write end file descriptor of created pipe
 *
 * @field readFd
 * 		read end file descriptor of created pipe
 *
 * @field status
 * 		the call status
 *
 * @security-level APPLICATION
 */
typedef struct
{
	File_t writeFd;
	File_t readFd;
	FsPipeStatus status;
}__attribute__((packed)) SyscallFsPipe;

/**
 * @field mode
 * 		the mode flags
 *
 * @field path
 * 		buffer containing the file path
 *
 * @field fd
 * 		contains the fd
 *
 * @field status
 * 		the status code
 *
 * @field length
 * 		length in bytes
 *
 * @security-level APPLICATION
 */
typedef int SyscallFsLengthMode;
#define	SYSCALL_FS_LENGTH_MODE_BY_MASK				1
#define	SYSCALL_FS_LENGTH_BY_PATH					((SyscallFsLengthMode) 0x0)
#define SYSCALL_FS_LENGTH_BY_FD						((SyscallFsLengthMode) 0x1)
#define SYSCALL_FS_LENGTH_MODE_SYMLINK_MASK			2
#define SYSCALL_FS_LENGTH_FOLLOW_SYMLINKS			((SyscallFsLengthMode) 0x0)
#define SYSCALL_FS_LENGTH_NOT_FOLLOW_SYMLINKS		((SyscallFsLengthMode) 0x2)

typedef struct
{
	SyscallFsLengthMode mode;
	char *path;
	File_t fd;

	FsLengthStatus status;
	int64_t length;
}__attribute__((packed)) SyscallFsLength;

/**
 * @field fd
 * 		file descriptor
 *
 * @field amount
 * 		amount to seek
 *
 * @field mode
 * 		mode to seek with
 *
 * @field status
 * 		one of the {FsSeekStatus} codes
 *
 * @field result
 * 		resulting offset
 *
 * @security-level APPLICATION
 */
typedef struct
{
	File_t fd;
	int64_t amount;
	FsSeekMode mode;

	FsSeekStatus status;
	int64_t result;
}__attribute__((packed)) SyscallFsSeek;

/**
 * @field fd
 * 		file descriptor
 *
 * @field status
 * 		status code
 *
 * @field result
 * 		current offset
 *
 * @security-level APPLICATION
 */
typedef struct
{
	File_t fd;

	FsTellStatus status;
	int64_t result;
}__attribute__((packed)) SyscallFsTell;

/**
 * @field path
 * 		buffer containing the path
 *
 * @field process
 * 		process to set working directory for, -1 to set for current
 *
 * @field result
 * 		one of the {SetWorkingDirectoryStatus} codes
 *
 * @security-level APPLICATION
 */
typedef struct
{
	char *path;
	ProcessCreationIdentifier process;

	SetWorkingDirectoryStatus result;
}__attribute__((packed)) SyscallFsSetWorkingDirectory;

/**
 * @field buffer
 * 		buffer with the given size
 *
 * @field maxlen
 * 		maximum number of bytes to write to the buffer
 *
 * @security-level APPLICATION
 */
typedef struct
{
	char *buffer;
	size_t maxlen;
	GetWorkingDirectoryStatus result;
}__attribute__((packed)) SyscallFsGetWorkingDirectory;

/**
 * @field buffer
 * 		buffer with a size of at least {PATH_MAX} bytes
 *
 * @security-level APPLICATION
 */
typedef struct
{
	char *buffer;
}__attribute__((packed)) SyscallFsGetExecutablePath;

/**
 * @field name
 * 		the name to use for delegate registration
 *
 * @field result
 * 		one of the
 {G_FS_REGISTER_AS_DELEGATE_*} status codes
 *
 * @field phys_mountpoint_id
 * 		the physical id to set for the mountpoint
 *
 * @field mountpoint_id
 * 		contains the mountpoint id on success
 *
 * @field transaction_storage
 * 		contains the transaction storage address on success
 *
 * @security-level DRIVER
 */
typedef struct
{
	char *name;
	FsPhysID physMountpointID;

	FsVirtID mountpointID;
	Address transactionStorage;
	FsRegisterAsDelegateStatus result;
}__attribute__((packed)) SyscallFsRegisterAsDelegate;

/**
 * @field transaction
 * 		the transaction to set the status for
 *
 * @field status
 * 		the status to set
 *
 * @security-level DRIVER
 */
typedef struct
{
	FsTransactionID transaction;
	FsTransactionStatus status;
}__attribute__((packed)) SyscallFsSetTransactionStatus;

/**
 * @field parentID
 * 		id of the parent node
 *
 * @field name
 *		the name to set
 *
 * @field type
 * 		the type to set
 *
 * @field phys_fs_id
 * 		physical fs unique id for the node
 *
 * @field create_id
 * 		is filled with the id of the created node
 *
 * @security-level DRIVER
 */
typedef struct
{
	FsVirtID parentID;
	char *name;
	FsNodeType type;
	FsPhysID physFsID;

	FsCreateNodeStatus result;
	FsVirtID createdID;
}__attribute__((packed)) SyscallFsCreateNode;

/**
 * @field path
 * 		buffer containing the folders path
 *
 * @field iterator
 * 		pointer to an allocated iterator
 *
 * @field status
 * 		one of the {FsOpenDirectoryStatus} codes
 *
 * @security-level APPLICATION
 */
typedef struct
{
	char *path;
	FsDirectoryIterator *iterator;

	FsOpenDirectoryStatus status;
}__attribute__((packed)) SyscallFsOpenDirectory;

/**
 * @field iterator
 * 		pointer to the iterator
 *
 * @field status
 * 		one of the {FsDirectoryRefreshStatus} codes
 *
 * @security-level APPLICATION
 */
typedef struct
{
	FsDirectoryIterator *iterator;

	FsDirectoryRefreshStatus status;
}__attribute__((packed)) SyscallFsReadDirectory;

/**
 * @field iterator
 * 		pointer to the iterator
 *
 * @security-level APPLICATION
 */
typedef struct
{
	FsDirectoryIterator *iterator;
}__attribute__((packed)) SyscallFsCloseDirectory;

__END_C

#endif
