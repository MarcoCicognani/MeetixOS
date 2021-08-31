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

#include "filesystem/FsDelegate.hpp"
#include "filesystem/FsTransactionHandlerWrite.hpp"
#include "filesystem/filesystem.hpp"
#include "logger/logger.hpp"
#include "tasking/wait/WaiterFsTransaction.hpp"

/**
 *
 */
FsTransactionHandlerStartStatus FsTransactionHandlerWrite::startTransaction(Thread *thread)
{
	// create a context-bound wrapper for the data buffer
	Contextual<uint8_t*> boundBuffer(data()->buffer, thread->process->pageDirectory);

	// check for the driver delegate
	FsDelegate *delegate = node->getDelegate();
	if (delegate == 0)
	{
		logWarn("%! writing of '%i' failed due to missing delegate on underlying node %i", "filesystem", fd->id, node->id);
		return FS_TRANSACTION_START_FAILED;
	}

	// check if the transaction is repeated only
	if (wantsRepeatTransaction())
	{

		// when a transaction is repeated, the waiter is still on the requesters task
		delegate->requestWrite(thread, node, data()->length, boundBuffer, fd, this);
		return FS_TRANSACTION_START_WITH_WAITER;
	}

	// start transaction by requesting the delegate
	FsTransactionID transaction = delegate->requestWrite(thread, node, data()->length, boundBuffer, fd, this);

	// initially check status
	if (WaiterFsTransaction::isTransactionWaiting(thread, this, transaction, delegate))
	{
		thread->wait(new WaiterFsTransaction(this, transaction, delegate));
		return FS_TRANSACTION_START_WITH_WAITER;
	}

	return FS_TRANSACTION_START_IMMEDIATE_FINISH;
}

/**
 *
 */
FsTransactionHandlerFinishStatus FsTransactionHandlerWrite::finishTransaction(Thread *thread, FsDelegate *delegate)
{
	delegate->finishWrite(thread, &status, &result, fd);
	data()->result = result;
	data()->status = status;
	return FS_TRANSACTION_HANDLING_DONE;
}
