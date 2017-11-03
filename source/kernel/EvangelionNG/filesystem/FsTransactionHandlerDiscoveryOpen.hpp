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

#ifndef EVA_FILESYSTEM_TRANSACTION_HANDLER_DISCOVERY_OPEN
#define EVA_FILESYSTEM_TRANSACTION_HANDLER_DISCOVERY_OPEN

#include "filesystem/FsTransactionHandlerDiscovery.hpp"
#include "filesystem/FsNode.hpp"
#include "filesystem/FsDescriptors.hpp"
#include "memory/contextual.hpp"
#include "logger/logger.hpp"
#include "filesystem/FsTransactionHandlerOpen.hpp"

/**
 *
 */
class FsTransactionHandlerDiscoveryOpen : public FsTransactionHandlerDiscovery
{
public:
	Contextual<SyscallFsOpen*> data;

	/**
	 *
	 */
	FsTransactionHandlerDiscoveryOpen(char *absolutePathIn, Contextual<SyscallFsOpen*> data) : FsTransactionHandlerDiscovery(absolutePathIn), data(data) {}

	/**
	 *
	 */
	virtual FsTransactionHandlerFinishStatus afterFinishTransaction(Thread *thread)
	{
		if (status == FS_DISCOVERY_SUCCESSFUL || status == FS_DISCOVERY_NOT_FOUND)
		{
			// create and start the new handler
			FsNode *discoveredNode = nullptr;
			if (status == FS_DISCOVERY_NOT_FOUND) discoveredNode = lastDiscoveredParent;
			else discoveredNode = node;

			FsTransactionHandlerOpen *handler = new FsTransactionHandlerOpen(data, status, discoveredNode);
			FsTransactionHandlerStartStatus startStatus = handler->startTransaction(thread);

			if (startStatus == FS_TRANSACTION_START_WITH_WAITER) return FS_TRANSACTION_HANDLING_KEEP_WAITING_WITH_NEW_HANDLER;
			else if (startStatus == FS_TRANSACTION_START_IMMEDIATE_FINISH) return FS_TRANSACTION_HANDLING_DONE;
			else
			{
				data()->status = FS_OPEN_ERROR;
				logWarn("%! failed to start actual open handler after node discovery", "filesystem");
				return FS_TRANSACTION_HANDLING_DONE;
			}

		}

		else if (status == FS_DISCOVERY_ERROR || status == FS_DISCOVERY_BUSY)
		{
			data()->fd = -1;
			data()->status = FS_OPEN_ERROR;
		}

		return FS_TRANSACTION_HANDLING_DONE;
	}

};

#endif
