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

#ifndef EVA_FILESYSTEM_FILESYSTEMDELEGATE
#define EVA_FILESYSTEM_FILESYSTEMDELEGATE

#include "eva/stdint.h"
#include "utils/HashMap.hpp"
#include "filesystem/FsTransactionStore.hpp"
#include "filesystem/FsDescriptors.hpp"
#include "filesystem/FsTransactionHandler.hpp"
#include "filesystem/FsTransactionHandlerRead.hpp"
#include "filesystem/FsTransactionHandlerReadDirectory.hpp"
#include "filesystem/FsTransactionHandlerDirectoryRefresh.hpp"
#include "filesystem/FsTransactionHandlerWrite.hpp"
#include "filesystem/FsTransactionHandlerDiscovery.hpp"
#include "filesystem/FsTransactionHandlerGetLength.hpp"
#include "filesystem/FsTransactionHandlerOpen.hpp"
#include "filesystem/FsTransactionHandlerClose.hpp"
#include "memory/contextual.hpp"

/**
 *
 */
class FsDelegate
{
public:
	/**
	 *
	 */
	virtual ~FsDelegate()
	{
	}

	/**
	 * Performs a discovery transaction (this means that a lookup is made to find an
	 * actual file with the given name) for a name, searching with a specific parent node.
	 *
	 * @param parent
	 * 		the parent node
	 * @param child
	 * 		the child to discover
	 * @param handler
	 * 		the finish handler
	 *
	 * @return the transaction id
	 */
	virtual FsTransactionID requestDiscovery(Thread *requester, FsNode *parent, char *child, FsTransactionHandlerDiscovery *handler) = 0;

	/**
	 * Finishs a read transaction, allowing the delegate implementation to fill the
	 * output status.
	 *
	 * @param requester
	 * 		the thread requesting read
	 * @param handler
	 * 		the finish handler
	 */
	virtual void finishDiscovery(Thread *requester, FsTransactionHandlerDiscovery *handler) = 0;

	/**
	 * Performs a read transaction.
	 *
	 * @param requester
	 * 		the thread requesting read
	 * @param node
	 * 		the node to read from
	 * @param fd
	 * 		the file descriptor
	 * @param handler
	 * 		the finish handler
	 *
	 * @return the transaction id
	 */
	virtual FsTransactionID requestRead(Thread *requester, FsNode *node, int64_t length, Contextual<uint8_t*> buffer, FileDescriptorContent *fd, FsTransactionHandlerRead *handler) = 0;

	/**
	 * Finishs a read transaction, allowing the delegate implementation to fill
	 * results into the call data struct.
	 *
	 * @param requester
	 * 		the thread requesting read
	 * @param data
	 * 		the call data to fill
	 * @param fd
	 * 		the file descriptor that was read from
	 */
	virtual void finishRead(Thread *requester, FsReadStatus *outStatus, int64_t *outResult, FileDescriptorContent *fd) = 0;

	/**
	 * Performs a write transaction.
	 *
	 * @param requester
	 * 		the thread requesting write
	 * @param node
	 * 		the node to write from
	 * @param fd
	 * 		the file descriptor
	 * @param handler
	 * 		the finish handler
	 *
	 * @return the transaction id
	 */
	virtual FsTransactionID requestWrite(Thread *requester, FsNode *node, int64_t length, Contextual<uint8_t*> buffer, FileDescriptorContent *fd, FsTransactionHandlerWrite *handler) = 0;

	/**
	 * Finishs a write transaction, allowing the delegate implementation to fill
	 * results into the call data struct.
	 *
	 * @param requester
	 * 		the thread requesting read
	 * @param data
	 * 		the call data to fill
	 * @param fd
	 * 		the file descriptor that was read from
	 */
	virtual void finishWrite(Thread *requester, FsWriteStatus *outStatus, int64_t *outResult, FileDescriptorContent *fd) = 0;

	/**
	 * Performs a get-length transaction
	 *
	 * @param requester
	 * 		the thread requesting get-length
	 * @param node
	 * 		the node to get length of
	 * @param handler
	 * 		the finish handler
	 *
	 * @return the transaction id
	 */
	virtual FsTransactionID requestGetLength(Thread *requester, FsNode *node, FsTransactionHandlerGetLength *handler) = 0;

	/**
	 * Finishs a get-length transaction, allowing the delegate implementation to fill
	 * results into the requested variable.
	 *
	 * @param requester
	 * 		the thread requesting read
	 * @param handler
	 * 		the finish handler
	 */
	virtual void finishGetLength(Thread *requester, FsTransactionHandlerGetLength *handler) = 0;

	/**
	 * Refreshing the directory means that the executing delegate shall create all VFS nodes for
	 * all children of the given node.
	 */
	virtual FsTransactionID requestDirectoryRefresh(Thread *requester, FsNode *node, FsTransactionHandlerDirectoryRefresh *handler) = 0;

	/**
	 * Finishes the directory refresh transaction, allowing the delegate to fill the results into
	 * the output
	 */
	virtual void finishDirectoryRefresh(Thread *requester, FsTransactionHandlerDirectoryRefresh *handler) = 0;

	/**
	 * Used to open a file. The given node can be two things here: If the node was found (meaning the file
	 * already exists) it is the node itself. Otherwise it is the last discovered parent node and the filename
	 * is the name of the file that shall be created.
	 */
	virtual FsTransactionID requestOpen(Thread *requester, FsNode *node, char *filename, int32_t flags, int32_t mode, FsTransactionHandlerOpen *handler) = 0;

	/**
	 *
	 */
	virtual void finishOpen(Thread *requester, FsTransactionHandlerOpen *handler) = 0;

	/**
	 * Used to close a file.
	 */
	virtual FsTransactionID requestClose(Thread *requester, FsTransactionHandlerClose *handler, FileDescriptorContent *fd, FsNode *node) = 0;

	/**
	 *
	 */
	virtual void finishClose(Thread *requester, FsTransactionHandlerClose *handler) = 0;

};

#endif
