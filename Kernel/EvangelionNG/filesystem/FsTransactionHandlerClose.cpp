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

#include "filesystem/filesystem.hpp"
#include "filesystem/FsDelegate.hpp"
#include "filesystem/FsTransactionHandlerOpen.hpp"
#include "logger/logger.hpp"
#include "tasking/wait/WaiterFsTransaction.hpp"

/**
 *
 */
FsTransactionHandlerStartStatus FsTransactionHandlerClose::startTransaction(Thread* thread) {
    // check for the driver delegate
    FsDelegate* delegate = node->getDelegate();
    if ( delegate == 0 ) {
        logWarn("%! failed to close descriptor %i due to missing delegate", "filesystem", fd);
        return FS_TRANSACTION_START_FAILED;
    }

    // start transaction by requesting the delegate
    FsTransactionID transaction = delegate->requestClose(thread, this, fd, node);

    // check status for possible immediate finish
    if ( WaiterFsTransaction::isTransactionWaiting(thread, this, transaction, delegate) ) {
        thread->wait(new WaiterFsTransaction(this, transaction, delegate));
        return FS_TRANSACTION_START_WITH_WAITER;
    }

    return FS_TRANSACTION_START_IMMEDIATE_FINISH;
}

/**
 *
 */
FsTransactionHandlerFinishStatus
FsTransactionHandlerClose::finishTransaction(Thread* thread, FsDelegate* delegate) {
    delegate->finishClose(thread, this);

    data()->status = status;

    // once delegate says closing was successful, unmap it
    if ( status == FS_CLOSE_SUCCESSFUL ) {
        if ( !FileSystem::unmapFile(thread->process->main->id, node, fd) ) {
            logWarn("%! delegate failed to close file descriptor %i in process %i",
                    "filesystem",
                    fd->id,
                    thread->process->main->id);
            status = FS_CLOSE_ERROR;
        }
    }

    return FS_TRANSACTION_HANDLING_DONE;
}
