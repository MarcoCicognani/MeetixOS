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

#ifndef EVA_FILESYSTEM_TRANSACTION_HANDLER_DISCOVERY
#define EVA_FILESYSTEM_TRANSACTION_HANDLER_DISCOVERY

#include "filesystem/FsTransactionHandler.hpp"
#include "filesystem/FsNode.hpp"
#include "filesystem/FsDescriptors.hpp"
#include "memory/contextual.hpp"
#include "utils/string.hpp"

/**
 * This handler discovers (and creates virtual nodes for) the given absolute path.
 *
 * This is done by looking up all path elements as virtual nodes top-down.
 * If all of the path elements already exist as virtual nodes, this function immediately
 * returns true. Otherwise, the delegate is asked to discover and a {g_waiter_fs_discovery}
 * is appended to the task. This waiter waits for the discovery transaction to finish.
 *
 * The discovery process is called by the waiter itself repeatedly until all path elements
 * are successfully discovered or discovery fails at some point.
 */
class FsTransactionHandlerDiscovery : public FsTransactionHandler
{
public:
	bool followSymlinks;
	FsDiscoveryStatus status = FS_DISCOVERY_ERROR;
	FsNode *node = 0;
	FsNode *lastDiscoveredParent = 0;

	char *absolutePath;
	bool allNodesDiscovered = false;

	/**
	 *
	 */
	FsTransactionHandlerDiscovery(char *absolutePathIn, bool followSymlinks = true) : followSymlinks(followSymlinks)
	{
		// clone incoming path
		int lenAbs = String::length(absolutePathIn);
		absolutePath = new char[lenAbs + 1];
		String::copy(absolutePath, absolutePathIn);
	}

	/**
	 *
	 */
	~FsTransactionHandlerDiscovery()
	{
		delete absolutePath;
	}

	/**
	 *
	 */
	virtual FsTransactionHandlerStartStatus startTransaction(Thread *thread);

	/**
	 *
	 */
	virtual FsTransactionHandlerFinishStatus finishTransaction(Thread *thread, FsDelegate *delegate);

	/**
	 *
	 */
	virtual FsTransactionHandlerFinishStatus afterFinishTransaction(Thread *thread) = 0;
};

#endif
