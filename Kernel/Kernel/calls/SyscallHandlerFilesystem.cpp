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

#include "Api/utils/local.hpp"
#include "calls/SyscallHandler.hpp"
#include "filesystem/filesystem.hpp"
#include "filesystem/FsTransactionHandlerDiscoveryGetLength.hpp"
#include "filesystem/FsTransactionHandlerDiscoveryOpen.hpp"
#include "filesystem/FsTransactionHandlerDiscoveryOpenDirectory.hpp"
#include "filesystem/FsTransactionHandlerDiscoverySetCwd.hpp"
#include "filesystem/FsTransactionHandlerGetLengthDefault.hpp"
#include "filesystem/FsTransactionHandlerGetLengthSeek.hpp"
#include "logger/logger.hpp"
#include "memory/contextual.hpp"
#include "tasking/tasking.hpp"
#include "utils/string.hpp"

#include <debug/DebugInterfaceKernel.hpp>

/**
 * Return the current working directory of the current process
 */
SYSCALL_HANDLER(getWorkingDirectory) {
    SyscallFsGetWorkingDirectory* data
        = (SyscallFsGetWorkingDirectory*)SYSCALL_DATA(currentThread->cpuState);

    char* cwd = currentThread->process->workingDirectory;
    if ( cwd ) {
        size_t length = StringUtils::length(cwd);
        if ( length + 1 > data->m_out_buffer_len )
            data->m_working_directory_status = GET_WORKING_DIRECTORY_SIZE_EXCEEDED;
        else {
            StringUtils::copy(data->m_out_buffer, cwd);
            data->m_working_directory_status = GET_WORKING_DIRECTORY_SUCCESSFUL;
        }
    }

    else
        data->m_working_directory_status = GET_WORKING_DIRECTORY_ERROR;

    return currentThread;
}

/**
 * Return the executable path of the process if exists
 */
SYSCALL_HANDLER(getExecutablePath) {
    SyscallFsGetExecutablePath* data
        = (SyscallFsGetExecutablePath*)SYSCALL_DATA(currentThread->cpuState);

    if ( !currentThread->process->sourcePath ) {
        data->m_out_buffer[0] = 0;
        data->m_len           = 0;
    } else {
        StringUtils::copy(data->m_out_buffer, currentThread->process->sourcePath);
        data->m_len = StringUtils::length(currentThread->process->sourcePath);
    }

    return currentThread;
}

/**
 * Register the current thread as a FileSystem delagate driver
 */
SYSCALL_HANDLER(fsRegisterAsDelegate) {
    SyscallFsRegisterAsDelegate* data
        = (SyscallFsRegisterAsDelegate*)SYSCALL_DATA(currentThread->cpuState);
    data->m_register_as_delegate_status = FileSystem::createDelegate(currentThread,
                                                                     data->m_mountpoint_name,
                                                                     data->m_phys_mountpoint_id,
                                                                     &data->m_vfs_mountpoint_id,
                                                                     &data->m_transaction_storage);
    return currentThread;
}

/**
 * Set a new status for the filesystem transaction from a previously registered filesystem delagate
 */
SYSCALL_HANDLER(fsSetTransactionStatus) {
    SyscallFsSetTransactionStatus* data
        = (SyscallFsSetTransactionStatus*)SYSCALL_DATA(currentThread->cpuState);
    FsTransactionStore::setStatus(data->m_transaction_id, data->m_transaction_status);
    return currentThread;
}

/**
 * Creates a new vfs node
 */
SYSCALL_HANDLER(fsCreateNode) {
    SyscallFsCreateNode* data = (SyscallFsCreateNode*)SYSCALL_DATA(currentThread->cpuState);

    // check for parent
    FsNode* parent = FileSystem::getNodeById(data->m_vfs_parent_id);
    if ( !parent ) {
        data->m_create_node_status = FS_CREATE_NODE_STATUS_FAILED_NO_PARENT;
        return currentThread;
    }

    // check for already existing child
    FsNode* node = parent->findChild(data->m_node_name);
    if ( node )
        data->m_create_node_status = FS_CREATE_NODE_STATUS_UPDATED;
    else {
        // create the node
        node       = FileSystem::createNode();
        node->name = new char[StringUtils::length(data->m_node_name) + 1];
        StringUtils::copy(node->name, data->m_node_name);
        parent->addChild(node);
        DEBUG_INTERFACE_FILESYSTEM_UPDATE_NODE(node);

        data->m_create_node_status = FS_CREATE_NODE_STATUS_CREATED;
    }

    // set node info
    node->type     = data->m_node_type;
    node->physFsID = data->m_phys_fs_id;

    // return node id
    data->m_vfs_node_id = node->id;
    return currentThread;
}

/**
 * Set a new working directory
 */
SYSCALL_HANDLER(setWorkingDirectory) {
    SyscallFsSetWorkingDirectory* data
        = (SyscallFsSetWorkingDirectory*)SYSCALL_DATA(currentThread->cpuState);

    // find the target thread
    Thread* target = nullptr;
    if ( !data->m_process_creation_identifier )
        target = currentThread; // any task can do this for himself
    else if ( currentThread->process->securityLevel <= SECURITY_LEVEL_KERNEL )
        target = (Thread*)data->m_process_creation_identifier; // only kernel-level tasks can do
                                                               // this for other tasks
    else {
        logWarn(
            "only kernel level tasks are allowed to set the working directory of another process");
        data->m_working_directory_status = SET_WORKING_DIRECTORY_ERROR;
        return currentThread;
    }

    // get the absolute path to the new working directory
    Local<char> absolutePath(new char[PATH_MAX]);
    FileSystem::concatAsAbsolutePath(target->process->workingDirectory,
                                     data->m_path,
                                     absolutePath());

    // if the executor sets the working directory for another thread (that is not yet attached),
    // we must supply this thread as the "unspawned target" to the transaction handler.
    Thread* unspawnedTarget = ((target == currentThread) ? 0 : target);

    // perform discovery, perform setting of working directory once finished
    Contextual<SyscallFsSetWorkingDirectory*> boundData(data,
                                                        currentThread->process->pageDirectory);
    FsTransactionHandlerDiscoverySetCwd*      handler
        = new FsTransactionHandlerDiscoverySetCwd(absolutePath(), boundData, unspawnedTarget);
    auto startStatus = handler->startTransaction(currentThread);

    if ( startStatus == FS_TRANSACTION_START_WITH_WAITER )
        return Tasking::schedule();
    else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH )
        return currentThread;
    else {
        logWarn("starting read transaction failed with status (%i)", startStatus);
        data->m_working_directory_status = SET_WORKING_DIRECTORY_ERROR;
        return currentThread;
    }
}

/**
 * Processes a file open request.
 */
SYSCALL_HANDLER(fsOpen) {
    SyscallFsOpen* data = (SyscallFsOpen*)SYSCALL_DATA(currentThread->cpuState);

    // create an absolute path from the given path
    Local<char> targetPath(new char[PATH_MAX]);
    FileSystem::concatAsAbsolutePath(currentThread->process->workingDirectory,
                                     data->m_path,
                                     targetPath());

    // create the handler that works after the node was discovered
    Contextual<SyscallFsOpen*> boundData(data, currentThread->process->pageDirectory);
    auto handler     = new FsTransactionHandlerDiscoveryOpen(targetPath(), boundData);
    auto startStatus = handler->startTransaction(currentThread);

    if ( startStatus == FS_TRANSACTION_START_WITH_WAITER ) {
        return Tasking::schedule();
    } else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH ) {
        return currentThread;
    } else {
        logWarn("starting open transaction failed with status (%i)", startStatus);
        data->m_open_status = FS_OPEN_ERROR;
        return currentThread;
    }
}

/**
 * Processes a file read request
 */
SYSCALL_HANDLER(fsRead) {
    SyscallFsRead* data = (SyscallFsRead*)SYSCALL_DATA(currentThread->cpuState);

    // find the filesystem node
    FsNode*                node;
    FileDescriptorContent* fd;
    if ( !FileSystem::nodeForDescriptor(currentThread->process->main->id,
                                        data->m_open_fd,
                                        &node,
                                        &fd) ) {
        data->m_read_status = FS_READ_INVALID_FD;
        return currentThread;
    }

    // create and start the handler
    Contextual<SyscallFsRead*> boundData(data, currentThread->process->pageDirectory);
    FsTransactionHandlerRead*  handler     = new FsTransactionHandlerRead(node, fd, boundData);
    auto                       startStatus = handler->startTransaction(currentThread);

    // check what happened when starting
    if ( startStatus == FS_TRANSACTION_START_WITH_WAITER )
        return Tasking::schedule();
    else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH )
        return currentThread;
    else {
        logWarn("starting read transaction failed with status (%i)", startStatus);
        data->m_read_status = FS_READ_ERROR;
        return currentThread;
    }
}

/**
 * Processes a file write request
 */
SYSCALL_HANDLER(fsWrite) {
    SyscallFsWrite* data = (SyscallFsWrite*)SYSCALL_DATA(currentThread->cpuState);

    // find the filesystem node
    FsNode*                node;
    FileDescriptorContent* fd;
    if ( !FileSystem::nodeForDescriptor(currentThread->process->main->id,
                                        data->m_open_fd,
                                        &node,
                                        &fd) ) {
        data->m_write_status = FS_WRITE_INVALID_FD;
        return currentThread;
    }

    // create and start the handler
    Contextual<SyscallFsWrite*> boundData(data, currentThread->process->pageDirectory);
    FsTransactionHandlerWrite*  handler     = new FsTransactionHandlerWrite(node, fd, boundData);
    auto                        startStatus = handler->startTransaction(currentThread);

    // check what happened when starting
    if ( startStatus == FS_TRANSACTION_START_WITH_WAITER )
        return Tasking::schedule();
    else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH )
        return currentThread;
    else {
        logWarn("%! starting write transaction failed with status (%i)", "filesystem", startStatus);
        data->m_write_status = FS_WRITE_ERROR;
        return currentThread;
    }
}

/**
 * s_close a file
 */
SYSCALL_HANDLER(fsClose) {
    SyscallFsClose* data = (SyscallFsClose*)SYSCALL_DATA(currentThread->cpuState);

    // find the filesystem node
    FsNode*                node;
    FileDescriptorContent* fd;
    if ( !FileSystem::nodeForDescriptor(currentThread->process->main->id,
                                        data->m_open_fd,
                                        &node,
                                        &fd) ) {
        data->m_close_status = FS_CLOSE_INVALID_FD;
        return currentThread;
    }

    // create and start handler
    Contextual<SyscallFsClose*> boundData(data, currentThread->process->pageDirectory);
    FsTransactionHandlerClose*  handler     = new FsTransactionHandlerClose(boundData, fd, node);
    auto                        startStatus = handler->startTransaction(currentThread);

    if ( startStatus == FS_TRANSACTION_START_WITH_WAITER )
        return Tasking::schedule();
    else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH )
        return currentThread;
    else {
        logWarn("starting close transaction failed with status (%i)", startStatus);
        data->m_close_status = FS_CLOSE_ERROR;
        return currentThread;
    }
}

/**
 * s_seek the cursor of a file
 */
SYSCALL_HANDLER(fsSeek) {
    SyscallFsSeek* data = (SyscallFsSeek*)SYSCALL_DATA(currentThread->cpuState);

    // find the node
    FsNode*                node;
    FileDescriptorContent* fd;
    if ( !FileSystem::nodeForDescriptor(currentThread->process->main->id,
                                        data->m_open_fd,
                                        &node,
                                        &fd) ) {
        data->m_seek_status = FS_SEEK_INVALID_FD;
        return currentThread;
    }

    // create and start handler
    Contextual<SyscallFsSeek*>         boundData(data, currentThread->process->pageDirectory);
    FsTransactionHandlerGetLengthSeek* handler
        = new FsTransactionHandlerGetLengthSeek(fd, node, boundData);
    auto startStatus = handler->startTransaction(currentThread);

    if ( startStatus == FS_TRANSACTION_START_WITH_WAITER )
        return Tasking::schedule();
    else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH )
        return currentThread;
    else {
        logWarn("starting get-length transaction for seek failed with status (%i)", startStatus);
        data->m_seek_status = FS_SEEK_ERROR;
        return currentThread;
    }
}

/**
 * Returns the length of the file in bytes
 */
SYSCALL_HANDLER(fsLength) {
    SyscallFsLength* data = (SyscallFsLength*)SYSCALL_DATA(currentThread->cpuState);

    bool byFd = (data->m_length_mode & SYSCALL_FS_LENGTH_MODE_BY_MASK) == SYSCALL_FS_LENGTH_BY_FD;
    bool followSymlinks = (data->m_length_mode & SYSCALL_FS_LENGTH_MODE_SYMLINK_MASK)
                       == SYSCALL_FS_LENGTH_FOLLOW_SYMLINKS;

    if ( byFd ) {
        // find the node
        FsNode*                node;
        FileDescriptorContent* fd;
        if ( !FileSystem::nodeForDescriptor(currentThread->process->main->id,
                                            data->m_open_fd,
                                            &node,
                                            &fd) ) {
            data->m_length_status = FS_LENGTH_INVALID_FD;
            return currentThread;
        }

        // create and start the handler
        Contextual<SyscallFsLength*> boundData(data, currentThread->process->pageDirectory);
        FsTransactionHandlerGetLengthDefault* handler
            = new FsTransactionHandlerGetLengthDefault(boundData, node);
        auto startStatus = handler->startTransaction(currentThread);

        if ( startStatus == FS_TRANSACTION_START_WITH_WAITER )
            return Tasking::schedule();
        else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH )
            return currentThread;
        else {
            logWarn("starting get-length transaction failed with status (%i)", startStatus);
            data->m_length_status = FS_LENGTH_ERROR;
            return currentThread;
        }
    }

    else {
        // get absolute path for the requested path, relative to process working directory
        Local<char> absolutePath(new char[PATH_MAX]);
        FileSystem::concatAsAbsolutePath(currentThread->process->workingDirectory,
                                         data->m_path,
                                         absolutePath());

        // create and start handler
        Contextual<SyscallFsLength*> boundData(data, currentThread->process->pageDirectory);
        FsTransactionHandlerDiscoveryGetLength* handler
            = new FsTransactionHandlerDiscoveryGetLength(absolutePath(), followSymlinks, boundData);
        auto startStatus = handler->startTransaction(currentThread);

        if ( startStatus == FS_TRANSACTION_START_WITH_WAITER )
            return Tasking::schedule();
        else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH )
            return currentThread;
        else {
            logWarn("starting get-length transaction failed with status (%i)", startStatus);
            data->m_length_status = FS_LENGTH_ERROR;
            return currentThread;
        }
    }

    return currentThread;
}

/**
 * Returns the cursor position of the provided file
 */
SYSCALL_HANDLER(fsTell) {
    SyscallFsTell* data = (SyscallFsTell*)SYSCALL_DATA(currentThread->cpuState);

    FsNode*                node;
    FileDescriptorContent* fd;
    if ( FileSystem::nodeForDescriptor(currentThread->process->main->id,
                                       data->m_open_fd,
                                       &node,
                                       &fd) ) {
        data->m_tell_status = FS_TELL_SUCCESSFUL;
        data->m_result      = fd->offset;
    }

    else {
        data->m_tell_status = FS_TELL_INVALID_FD;
        data->m_result      = -1;
    }
    return currentThread;
}

/**
 * Clone the file descriptors for a new process
 */
SYSCALL_HANDLER(fsClonefd) {
    SyscallFsCloneFd* data = (SyscallFsCloneFd*)SYSCALL_DATA(currentThread->cpuState);
    data->m_cloned_fd      = FileSystem::clonefd(data->m_source_fd,
                                                 data->m_source_proc_id,
                                                 data->m_target_fd,
                                                 data->m_target_proc_id,
                                                 &data->m_clone_fd_status);
    return currentThread;
}

/**
 * Creates a pipe for the currentThread
 */
SYSCALL_HANDLER(fsPipe) {
    SyscallFsPipe* data = (SyscallFsPipe*)SYSCALL_DATA(currentThread->cpuState);
    data->m_pipe_status
        = FileSystem::pipe(currentThread, &data->m_write_end_fd, &data->m_read_end_fd);
    return currentThread;
}

/**
 * Process a directory open
 */
SYSCALL_HANDLER(fsOpenDirectory) {
    SyscallFsOpenDirectory* data = (SyscallFsOpenDirectory*)SYSCALL_DATA(currentThread->cpuState);

    // get absolute path for the requested path, relative to process working directory
    Local<char> absolutePath(new char[PATH_MAX]);
    FileSystem::concatAsAbsolutePath(currentThread->process->workingDirectory,
                                     data->m_path,
                                     absolutePath());

    // create handler
    Contextual<SyscallFsOpenDirectory*> boundData(data, currentThread->process->pageDirectory);
    FsTransactionHandlerDiscoveryOpenDirectory* handler
        = new FsTransactionHandlerDiscoveryOpenDirectory(absolutePath(), boundData);
    handler->startTransaction(currentThread);

    return Tasking::schedule();
}

/**
 * Performs a directory reading
 */
SYSCALL_HANDLER(fsReadDirectory) {
    SyscallFsReadDirectory* data = (SyscallFsReadDirectory*)SYSCALL_DATA(currentThread->cpuState);

    // create handler
    Contextual<SyscallFsReadDirectory*> boundData(data, currentThread->process->pageDirectory);

    // find the folder to operate on
    FsNode* folder = FileSystem::getNodeById(data->m_directory_iterator->m_node_id);
    if ( !folder ) {
        data->m_read_directory_status = FS_READ_DIRECTORY_ERROR;
        return currentThread;
    }

    // handler that actually puts the next node into the iterator
    FsTransactionHandlerReadDirectory* readHandler
        = new FsTransactionHandlerReadDirectory(folder, boundData);

    // check if the directory contents are complete and valid, if they are finish immediately
    if ( folder->contentsValid ) {
        readHandler->finishTransaction(currentThread, 0);
        delete readHandler;
        return currentThread;
    }

    // schedule a refresh
    FsTransactionHandlerDirectoryRefresh* refreshHandler
        = new FsTransactionHandlerDirectoryRefresh(folder, boundData, readHandler);
    readHandler->causingHandler = refreshHandler;
    auto startStatus            = refreshHandler->startTransaction(currentThread);

    if ( startStatus == FS_TRANSACTION_START_WITH_WAITER )
        return Tasking::schedule();
    else if ( startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH )
        return currentThread;
    else {
        logWarn("starting read-directory transaction failed with status (%i)", startStatus);
        data->m_read_directory_status = FS_READ_DIRECTORY_ERROR;
        return currentThread;
    }
}

/**
 * Uninplemented
 */
SYSCALL_HANDLER(fsStat) {
    SyscallFsStat* data = (SyscallFsStat*)SYSCALL_DATA(currentThread->cpuState);
    data->m_result      = -1;
    return currentThread;
}

/**
 * Uninplemented
 */
SYSCALL_HANDLER(fsFstat) {
    SyscallFsFstat* data = (SyscallFsFstat*)SYSCALL_DATA(currentThread->cpuState);
    data->m_result       = -1;
    return currentThread;
}
