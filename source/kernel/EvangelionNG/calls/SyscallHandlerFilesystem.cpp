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

#include <debug/DebugInterfaceKernel.hpp>
#include "calls/SyscallHandler.hpp"

#include "filesystem/filesystem.hpp"
#include "filesystem/FsTransactionHandlerDiscoverySetCwd.hpp"
#include "filesystem/FsTransactionHandlerDiscoveryOpen.hpp"
#include "filesystem/FsTransactionHandlerDiscoveryOpenDirectory.hpp"
#include "filesystem/FsTransactionHandlerGetLengthSeek.hpp"
#include "filesystem/FsTransactionHandlerGetLengthDefault.hpp"
#include "filesystem/FsTransactionHandlerDiscoveryGetLength.hpp"

#include "eva/utils/local.hpp"
#include "tasking/tasking.hpp"
#include "utils/string.hpp"
#include "memory/contextual.hpp"
#include "logger/logger.hpp"

/**
 * Return the current working directory of the current process
 */
SYSCALL_HANDLER(getWorkingDirectory)
{
	SyscallFsGetWorkingDirectory *data = (SyscallFsGetWorkingDirectory*) SYSCALL_DATA(currentThread->cpuState);

	char *cwd = currentThread->process->workingDirectory;
	if (cwd)
	{
		size_t length = String::length(cwd);
		if (length + 1 > data->maxlen) data->result = GET_WORKING_DIRECTORY_SIZE_EXCEEDED;
		else
		{
			String::copy(data->buffer, cwd);
			data->result = GET_WORKING_DIRECTORY_SUCCESSFUL;
		}
	}

	else data->result = GET_WORKING_DIRECTORY_ERROR;

	return currentThread;
}

/**
 * Return the executable path of the process if exists
 */
SYSCALL_HANDLER(getExecutablePath)
{
	SyscallFsGetExecutablePath *data = (SyscallFsGetExecutablePath*) SYSCALL_DATA(currentThread->cpuState);

	if (!currentThread->process->sourcePath) data->buffer[0] = 0;
	else String::copy(data->buffer, currentThread->process->sourcePath);

	return currentThread;
}

/**
 * Register the current thread as a FileSystem delagate driver
 */
SYSCALL_HANDLER(fsRegisterAsDelegate)
{
	SyscallFsRegisterAsDelegate *data = (SyscallFsRegisterAsDelegate*) SYSCALL_DATA(currentThread->cpuState);
	data->result = FileSystem::createDelegate(currentThread, data->name, data->physMountpointID, &data->mountpointID, &data->transactionStorage);
	return currentThread;
}

/**
 * Set a new status for the filesystem transaction from a previously registered filesystem delagate
 */
SYSCALL_HANDLER(fsSetTransactionStatus)
{
	SyscallFsSetTransactionStatus *data = (SyscallFsSetTransactionStatus*) SYSCALL_DATA(currentThread->cpuState);
	FsTransactionStore::setStatus(data->transaction, data->status);
	return currentThread;
}

/**
 * Creates a new vfs node
 */
SYSCALL_HANDLER(fsCreateNode)
{
	SyscallFsCreateNode *data = (SyscallFsCreateNode*) SYSCALL_DATA(currentThread->cpuState);

	// check for parent
	FsNode *parent = FileSystem::getNodeById(data->parentID);
	if (!parent)
	{
		data->result = FS_CREATE_NODE_STATUS_FAILED_NO_PARENT;
		return currentThread;
	}

	// check for already existing child
	FsNode *node = parent->findChild(data->name);
	if (node) data->result = FS_CREATE_NODE_STATUS_UPDATED;
	else
	{
		// create the node
		node = FileSystem::createNode();
		node->name = new char[String::length(data->name) + 1];
		String::copy(node->name, data->name);
		parent->addChild(node);
		DEBUG_INTERFACE_FILESYSTEM_UPDATE_NODE(node);

		data->result = FS_CREATE_NODE_STATUS_CREATED;
	}

	// set node info
	node->type = data->type;
	node->physFsID = data->physFsID;

	// return node id
	data->createdID = node->id;
	return currentThread;
}

/**
 * Set a new working directory
 */
SYSCALL_HANDLER(setWorkingDirectory)
{
	SyscallFsSetWorkingDirectory *data = (SyscallFsSetWorkingDirectory*) SYSCALL_DATA(currentThread->cpuState);

	// find the target thread
	Thread *target = nullptr;
	if (!data->process) target = currentThread; // any task can do this for himself
	else if (currentThread->process->securityLevel <= SECURITY_LEVEL_KERNEL) target = (Thread*) data->process; // only kernel-level tasks can do this for other tasks
	else
	{
		logWarn("only kernel level tasks are allowed to set the working directory of another process");
		data->result = SET_WORKING_DIRECTORY_ERROR;
		return currentThread;
	}

	// get the absolute path to the new working directory
	Local<char> absolutePath(new char[PATH_MAX]);
	FileSystem::concatAsAbsolutePath(target->process->workingDirectory, data->path, absolutePath());

	// if the executor sets the working directory for another thread (that is not yet attached),
	// we must supply this thread as the "unspawned target" to the transaction handler.
	Thread *unspawnedTarget = ((target == currentThread) ? 0 : target);

	// perform discovery, perform setting of working directory once finished
	Contextual<SyscallFsSetWorkingDirectory*> boundData(data, currentThread->process->pageDirectory);
	FsTransactionHandlerDiscoverySetCwd *handler = new FsTransactionHandlerDiscoverySetCwd(absolutePath(), boundData, unspawnedTarget);
	auto startStatus = handler->startTransaction(currentThread);

	if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
	else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
	else
	{
		logWarn("starting read transaction failed with status (%i)", startStatus);
		data->result = SET_WORKING_DIRECTORY_ERROR;
		return currentThread;
	}
}

/**
 * Processes a file open request.
 */
SYSCALL_HANDLER(fsOpen)
{
	SyscallFsOpen *data = (SyscallFsOpen*) SYSCALL_DATA(currentThread->cpuState);

	// create an absolute path from the given path
	Local<char> targetPath(new char[PATH_MAX]);
	FileSystem::concatAsAbsolutePath(currentThread->process->workingDirectory, data->path, targetPath());

	// create the handler that works after the node was discovered
	Contextual<SyscallFsOpen*> boundData(data, currentThread->process->pageDirectory);
	FsTransactionHandlerDiscoveryOpen *handler = new FsTransactionHandlerDiscoveryOpen(targetPath(), boundData);
	auto startStatus = handler->startTransaction(currentThread);

	if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
	else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
	else
	{
		logWarn("starting open transaction failed with status (%i)", startStatus);
		data->status = FS_OPEN_ERROR;
		return currentThread;
	}
}

/**
 * Processes a file read request
 */
SYSCALL_HANDLER(fsRead)
{
	SyscallFsRead *data = (SyscallFsRead*) SYSCALL_DATA(currentThread->cpuState);

	// find the filesystem node
	FsNode *node;
	FileDescriptorContent *fd;
	if (!FileSystem::nodeForDescriptor(currentThread->process->main->id, data->fd, &node, &fd))
	{
		data->status = FS_READ_INVALID_FD;
		return currentThread;
	}

	// create and start the handler
	Contextual<SyscallFsRead*> boundData(data, currentThread->process->pageDirectory);
	FsTransactionHandlerRead *handler = new FsTransactionHandlerRead(node, fd, boundData);
	auto startStatus = handler->startTransaction(currentThread);

	// check what happened when starting
	if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
	else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
	else
	{
		logWarn("starting read transaction failed with status (%i)", startStatus);
		data->status = FS_READ_ERROR;
		return currentThread;
	}
}

/**
 * Processes a file write request
 */
SYSCALL_HANDLER(fsWrite)
{
	SyscallFsWrite *data = (SyscallFsWrite*) SYSCALL_DATA(currentThread->cpuState);

	// find the filesystem node
	FsNode *node;
	FileDescriptorContent *fd;
	if (!FileSystem::nodeForDescriptor(currentThread->process->main->id, data->fd, &node, &fd))
	{
		data->status = FS_WRITE_INVALID_FD;
		return currentThread;
	}

	// create and start the handler
	Contextual<SyscallFsWrite*> boundData(data, currentThread->process->pageDirectory);
	FsTransactionHandlerWrite *handler = new FsTransactionHandlerWrite(node, fd, boundData);
	auto startStatus = handler->startTransaction(currentThread);

	// check what happened when starting
	if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
	else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
	else
	{
		logWarn("%! starting write transaction failed with status (%i)", "filesystem", startStatus);
		data->status = FS_WRITE_ERROR;
		return currentThread;
	}
}

/**
 * Close a file
 */
SYSCALL_HANDLER(fsClose)
{
	SyscallFsClose *data = (SyscallFsClose*) SYSCALL_DATA(currentThread->cpuState);

	// find the filesystem node
	FsNode *node;
	FileDescriptorContent *fd;
	if (!FileSystem::nodeForDescriptor(currentThread->process->main->id, data->fd, &node, &fd))
	{
		data->status = FS_CLOSE_INVALID_FD;
		return currentThread;
	}

	// create and start handler
	Contextual<SyscallFsClose*> boundData(data, currentThread->process->pageDirectory);
	FsTransactionHandlerClose *handler = new FsTransactionHandlerClose(boundData, fd, node);
	auto startStatus = handler->startTransaction(currentThread);

	if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
	else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
	else
	{
		logWarn("starting close transaction failed with status (%i)", startStatus);
		data->status = FS_CLOSE_ERROR;
		return currentThread;
	}
}

/**
 * Seek the cursor of a file
 */
SYSCALL_HANDLER(fsSeek)
{
	SyscallFsSeek *data = (SyscallFsSeek*) SYSCALL_DATA(currentThread->cpuState);

	// find the node
	FsNode *node;
	FileDescriptorContent *fd;
	if (!FileSystem::nodeForDescriptor(currentThread->process->main->id, data->fd, &node, &fd))
	{
		data->status = FS_SEEK_INVALID_FD;
		return currentThread;
	}

	// create and start handler
	Contextual<SyscallFsSeek*> boundData(data, currentThread->process->pageDirectory);
	FsTransactionHandlerGetLengthSeek *handler = new FsTransactionHandlerGetLengthSeek(fd, node, boundData);
	auto startStatus = handler->startTransaction(currentThread);

	if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
	else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
	else
	{
		logWarn("starting get-length transaction for seek failed with status (%i)", startStatus);
		data->status = FS_SEEK_ERROR;
		return currentThread;
	}
}

/**
 * Returns the length of the file in bytes
 */
SYSCALL_HANDLER(fsLength)
{
	SyscallFsLength *data = (SyscallFsLength*) SYSCALL_DATA(currentThread->cpuState);

	bool byFd = (data->mode & SYSCALL_FS_LENGTH_MODE_BY_MASK) == SYSCALL_FS_LENGTH_BY_FD;
	bool followSymlinks = (data->mode & SYSCALL_FS_LENGTH_MODE_SYMLINK_MASK) == SYSCALL_FS_LENGTH_FOLLOW_SYMLINKS;

	if (byFd)
	{
		// find the node
		FsNode *node;
		FileDescriptorContent *fd;
		if (!FileSystem::nodeForDescriptor(currentThread->process->main->id, data->fd, &node, &fd))
		{
			data->status = FS_LENGTH_INVALID_FD;
			return currentThread;
		}

		// create and start the handler
		Contextual<SyscallFsLength*> boundData(data, currentThread->process->pageDirectory);
		FsTransactionHandlerGetLengthDefault *handler = new FsTransactionHandlerGetLengthDefault(boundData, node);
		auto startStatus = handler->startTransaction(currentThread);

		if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
		else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
		else
		{
			logWarn("starting get-length transaction failed with status (%i)", startStatus);
			data->status = FS_LENGTH_ERROR;
			return currentThread;
		}
	}

	else
	{
		// get absolute path for the requested path, relative to process working directory
		Local<char> absolutePath(new char[PATH_MAX]);
		FileSystem::concatAsAbsolutePath(currentThread->process->workingDirectory, data->path, absolutePath());

		// create and start handler
		Contextual<SyscallFsLength*> boundData(data, currentThread->process->pageDirectory);
		FsTransactionHandlerDiscoveryGetLength *handler = new FsTransactionHandlerDiscoveryGetLength(absolutePath(), followSymlinks, boundData);
		auto startStatus = handler->startTransaction(currentThread);

		if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
		else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
		else
		{
			logWarn("starting get-length transaction failed with status (%i)", startStatus);
			data->status = FS_LENGTH_ERROR;
			return currentThread;
		}
	}

	return currentThread;
}

/**
 * Returns the cursor position of the provided file
 */
SYSCALL_HANDLER(fsTell)
{
	SyscallFsTell *data = (SyscallFsTell*) SYSCALL_DATA(currentThread->cpuState);

	FsNode *node;
	FileDescriptorContent *fd;
	if (FileSystem::nodeForDescriptor(currentThread->process->main->id, data->fd, &node, &fd))
	{
		data->status = FS_TELL_SUCCESSFUL;
		data->result = fd->offset;
	}

	else
	{
		data->status = FS_TELL_INVALID_FD;
		data->result = -1;
	}
	return currentThread;
}

/**
 * Clone the file descriptors for a new process
 */
SYSCALL_HANDLER(fsClonefd)
{
	SyscallFsClonefd *data = (SyscallFsClonefd*) SYSCALL_DATA(currentThread->cpuState);
	data->result = FileSystem::clonefd(data->sourceFd, data->sourcePid, data->targetFd, data->targetPid, &data->status);
	return currentThread;
}

/**
 * Creates a pipe for the currentThread
 */
SYSCALL_HANDLER(fsPipe)
{
	SyscallFsPipe *data = (SyscallFsPipe*) SYSCALL_DATA(currentThread->cpuState);
	data->status = FileSystem::pipe(currentThread, &data->writeFd, &data->readFd);
	return currentThread;
}

/**
 * Process a directory open
 */
SYSCALL_HANDLER(fsOpenDirectory)
{
	SyscallFsOpenDirectory *data = (SyscallFsOpenDirectory*) SYSCALL_DATA(currentThread->cpuState);

	// get absolute path for the requested path, relative to process working directory
	Local<char> absolutePath(new char[PATH_MAX]);
	FileSystem::concatAsAbsolutePath(currentThread->process->workingDirectory, data->path, absolutePath());

	// create handler
	Contextual<SyscallFsOpenDirectory*> boundData(data, currentThread->process->pageDirectory);
	FsTransactionHandlerDiscoveryOpenDirectory *handler = new FsTransactionHandlerDiscoveryOpenDirectory(absolutePath(), boundData);
	handler->startTransaction(currentThread);

	return Tasking::schedule();
}

/**
 * Performs a directory reading
 */
SYSCALL_HANDLER(fsReadDirectory)
{
	SyscallFsReadDirectory *data = (SyscallFsReadDirectory*) SYSCALL_DATA(currentThread->cpuState);

	// create handler
	Contextual<SyscallFsReadDirectory*> boundData(data, currentThread->process->pageDirectory);

	// find the folder to operate on
	FsNode *folder = FileSystem::getNodeById(data->iterator->nodeID);
	if (!folder)
	{
		data->status = FS_READ_DIRECTORY_ERROR;
		return currentThread;
	}

	// handler that actually puts the next node into the iterator
	FsTransactionHandlerReadDirectory *readHandler = new FsTransactionHandlerReadDirectory(folder, boundData);

	// check if the directory contents are complete and valid, if they are finish immediately
	if (folder->contentsValid)
	{
		readHandler->finishTransaction(currentThread, 0);
		delete readHandler;
		return currentThread;
	}

	// schedule a refresh
	FsTransactionHandlerDirectoryRefresh *refreshHandler = new FsTransactionHandlerDirectoryRefresh(folder, boundData, readHandler);
	readHandler->causingHandler = refreshHandler;
	auto startStatus = refreshHandler->startTransaction(currentThread);

	if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return Tasking::schedule();
	else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return currentThread;
	else
	{
		logWarn("starting read-directory transaction failed with status (%i)", startStatus);
		data->status = FS_READ_DIRECTORY_ERROR;
		return currentThread;
	}
}

/**
 * Uninplemented
 */
SYSCALL_HANDLER(fsStat)
{
	SyscallFsStat *data = (SyscallFsStat*) SYSCALL_DATA(currentThread->cpuState);
	data->result = -1;
	return currentThread;
}

/**
 * Uninplemented
 */
SYSCALL_HANDLER(fsFstat)
{
	SyscallFsFstat *data = (SyscallFsFstat*) SYSCALL_DATA(currentThread->cpuState);
	data->result = -1;
	return currentThread;
}
