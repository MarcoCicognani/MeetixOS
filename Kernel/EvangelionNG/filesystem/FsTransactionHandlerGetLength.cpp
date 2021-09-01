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
#include "filesystem/FsTransactionHandlerGetLength.hpp"
#include "tasking/wait/WaiterFsTransaction.hpp"

/**
 *
 */
FsTransactionHandlerStartStatus FsTransactionHandlerGetLength::startTransaction(Thread *thread)
{
	FsDelegate *delegate = node->getDelegate();
	if (delegate)
	{
		FsTransactionID transaction = delegate->requestGetLength(thread, node, this);
		thread->wait(new WaiterFsTransaction(this, transaction, delegate));
		return FS_TRANSACTION_START_WITH_WAITER;
	}

	return FS_TRANSACTION_START_FAILED;
}

/**
 *
 */
FsTransactionHandlerFinishStatus FsTransactionHandlerGetLength::finishTransaction(Thread *thread, FsDelegate *delegate)
{
	delegate->finishGetLength(thread, this);

	performAfterwork(thread);
	return FS_TRANSACTION_HANDLING_DONE;
}
