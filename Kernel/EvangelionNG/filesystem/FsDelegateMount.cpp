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

#include "filesystem/FsDelegateMount.hpp"
#include "filesystem/filesystem.hpp"
#include "utils/string.hpp"
#include "logger/logger.hpp"
#include "EvangelionNG.hpp"
#include "eva/utils/local.hpp"

/**
 *
 */
FsTransactionID FsDelegateMount::requestDiscovery(Thread *requester, FsNode *parent, char *child, FsTransactionHandlerDiscovery *handler)
{
	// the ramdisk handler is doing it's work immediately and doesn't request another process
	FsTransactionID id = FsTransactionStore::nextTransaction();

	// root elements must exist and cannot be discovered
	handler->status = FS_DISCOVERY_NOT_FOUND;
	FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

	return id;
}

/**
 *
 */
void FsDelegateMount::finishDiscovery(Thread *requester, FsTransactionHandlerDiscovery *handler)
{
	// nothing to do here
}

/**
 *
 */
FsTransactionID FsDelegateMount::requestRead(Thread *requester, FsNode *node, int64_t length, Contextual<uint8_t*> buffer, FileDescriptorContent *fd, FsTransactionHandlerRead *handler)
{
	// start/repeat transaction
	FsTransactionID id;
	if (handler->wantsRepeatTransaction()) id = handler->getRepeatedTransaction();
	else id = FsTransactionStore::nextTransaction();

	handler->status = FS_READ_ERROR;
	FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

	return id;
}

/**
 *
 */
void FsDelegateMount::finishRead(Thread *requester, FsReadStatus *outStatus, int64_t *outResult, FileDescriptorContent *fd)
{
}

/**
 *
 */
FsTransactionID FsDelegateMount::requestWrite(Thread *requester, FsNode *node, int64_t length, Contextual<uint8_t*> buffer, FileDescriptorContent *fd, FsTransactionHandlerWrite *handler)
{
	// start/repeat transaction
	FsTransactionID id;
	if (handler->wantsRepeatTransaction()) id = handler->getRepeatedTransaction();
	else id = FsTransactionStore::nextTransaction();

	handler->status = FS_WRITE_ERROR;
	FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

	return id;
}

/**
 *
 */
void FsDelegateMount::finishWrite(Thread *requester, FsWriteStatus *outStatus, int64_t *outResult, FileDescriptorContent *fd)
{
}

/**
 *
 */
FsTransactionID FsDelegateMount::requestGetLength(Thread *requester, FsNode *node, FsTransactionHandlerGetLength *handler)
{
	// the ramdisk handler is doing it's work immediately and doesn't request another process
	FsTransactionID id = FsTransactionStore::nextTransaction();

	handler->status = FS_LENGTH_ERROR;
	handler->length = -1;

	FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	return id;
}

/**
 *
 */
void FsDelegateMount::finishGetLength(Thread *requester, FsTransactionHandlerGetLength *handler)
{
}

/**
 *
 */
FsTransactionID FsDelegateMount::requestDirectoryRefresh(Thread *requester, FsNode *folder, FsTransactionHandlerDirectoryRefresh *handler)
{
	FsTransactionID id = FsTransactionStore::nextTransaction();

	// mount must not be refreshed, contents are always consistent
	folder->contentsValid = true;
	handler->status = FS_DIRECTORY_REFRESH_SUCCESSFUL;

	FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	return id;
}

/**
 *
 */
void FsDelegateMount::finishDirectoryRefresh(Thread *requester, FsTransactionHandlerDirectoryRefresh *handler)
{
}

/**
 *
 */
FsTransactionID FsDelegateMount::requestOpen(Thread *requester, FsNode *node, char *filename, int32_t flags, int32_t mode, FsTransactionHandlerOpen *handler)
{
	FsTransactionID id = FsTransactionStore::nextTransaction();

	// mount can't be opened
	logWarn("%! mountpoints can not be opened", "filesystem");
	handler->status = FS_OPEN_ERROR;

	FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	return id;
}

/**
 *
 */
void FsDelegateMount::finishOpen(Thread *requester, FsTransactionHandlerOpen *handler)
{
}

/**
 *
 */
FsTransactionID FsDelegateMount::requestClose(Thread *requester, FsTransactionHandlerClose *handler, FileDescriptorContent *fd, FsNode *node)
{
	FsTransactionID id = FsTransactionStore::nextTransaction();

	// mount can't be opened
	logWarn("%! mountpoints can not be closed", "filesystem");
	handler->status = FS_CLOSE_ERROR;

	FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	return id;
}

/**
 *
 */
void FsDelegateMount::finishClose(Thread *requester, FsTransactionHandlerClose *handler)
{
}
