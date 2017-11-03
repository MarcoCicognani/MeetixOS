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

#include "filesystem/FsDelegateTasked.hpp"
#include "filesystem/filesystem.hpp"
#include "utils/string.hpp"
#include "logger/logger.hpp"
#include "EvangelionNG.hpp"
#include "tasking/communication/MessageController.hpp"
#include "memory/AddressSpace.hpp"
#include "memory/physical/PPallocator.hpp"
#include "eva/utils/local.hpp"

/**
 *
 */
FsDelegateTasked::FsDelegateTasked(Thread *delegateThread) : transactionStoragePhys(0), delegateThread(delegateThread)
{

}

/**
 *
 */
bool FsDelegateTasked::prepare(VirtualAddress *outTransactionStorage)
{
	VirtualAddress transactionStorageAddress = delegateThread->process->virtualRanges.allocate(1, PROC_VIRTUAL_RANGE_FLAG_PHYSICAL_OWNER);
	if (transactionStorageAddress == 0)
	{
		logWarn("%! failed to allocate virtual range for transaction storage when trying to create tasked delegate", "filesystem");
		return false;
	}

	transactionStoragePhys = PPallocator::allocate();
	if (transactionStoragePhys == 0)
	{
		logWarn("%! failed to allocate physical page for transaction storage when trying to create tasked delegate", "filesystem");
		return false;
	}

	/**
	 * For safety, were switching to the page directory of the process
	 * that is registered as a delegate and then map the transaction storage.
	 *
	 * This is to make sure that, if I get the amazing idea to one day trigger a
	 * file system delegate creation from within another process, the world will
	 * not fall into pieces.
	 */
	PageDirectory current = AddressSpace::getCurrentSpace();
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);
	AddressSpace::map(transactionStorageAddress, transactionStoragePhys, DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);
	AddressSpace::switchToSpace(current);
	logDebug("%! fs delegate transaction storage created at %h of process %i", "filesystem", transactionStorageAddress, delegateThread->id);

	*outTransactionStorage = transactionStorageAddress;
	transactionStorage.set((void*) transactionStorageAddress, delegateThread->process->pageDirectory);
	return true;
}

/**
 *
 */
FsTransactionID FsDelegateTasked::requestDiscovery(Thread *requester, FsNode *parent, char *child, FsTransactionHandlerDiscovery *handler)
{
	// begin the transaction
	FsTransactionID id = FsTransactionStore::nextTransaction();

	/**
	 * Switch to the delegates space and fill the transaction storage.
	 */
	bool configurationFine = true;
	PageDirectory current = AddressSpace::getCurrentSpace();
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageDiscovery *disc = (FsTaskedDelegateTransactionStorageDiscovery*) transactionStorage();
	int childlen = String::length(child);
	if (childlen > FILENAME_MAX)
	{
		logInfo("tried to discover a node that has a name with an illegal length");
		configurationFine = false;
	}

	else
	{
		Memory::copy(disc->name, child, childlen + 1);

		disc->parentPhysFsID = parent->physFsID;
	}

	AddressSpace::switchToSpace(current);

	// update the status / notify delegate thread
	if (configurationFine == false) handler->status = FS_DISCOVERY_ERROR;

	else
	{
		MessageSendStatus sendStatus = MESSAGE_SEND_STATUS_FAILED; // TODO

		// task was requested, wait for answer
		if (sendStatus == MESSAGE_SEND_STATUS_SUCCESSFUL) FsTransactionStore::setStatus(id, FS_TRANSACTION_WAITING);
		else if (sendStatus == MESSAGE_SEND_STATUS_QUEUE_FULL)
		{
			logWarn("%! message queue was full when trying to request discovery to fs delegate", "filesystem");
			handler->status = FS_DISCOVERY_BUSY;
			FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

		}

		else if (sendStatus == MESSAGE_SEND_STATUS_FAILED)
		{
			logWarn("%! message sending failed when trying to request discovery to fs delegate", "filesystem");
			handler->status = FS_DISCOVERY_ERROR;
			FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
		}
	}

	return id;
}

/**
 *
 */
void FsDelegateTasked::finishDiscovery(Thread *requester, FsTransactionHandlerDiscovery *handler)
{
	// save current directory
	PageDirectory current = AddressSpace::getCurrentSpace();

	// First get status from transaction storage
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);
	FsTaskedDelegateTransactionStorageDiscovery *dspace = (FsTaskedDelegateTransactionStorageDiscovery*) transactionStorage();
	FsDiscoveryStatus status = dspace->resultStatus;

	// Now switch to the requesters space and copy status there
	AddressSpace::switchToSpace(requester->process->pageDirectory);
	handler->status = status;
	AddressSpace::switchToSpace(current);
}

/**
 *
 */
FsTransactionID FsDelegateTasked::requestRead(Thread *requester, FsNode *node, int64_t length, Contextual<uint8_t*> buffer, FileDescriptorContent *fd, FsTransactionHandlerRead *handler)
{
	// start/repeat transaction
	FsTransactionID id;
	if (handler->wantsRepeatTransaction()) id = handler->getRepeatedTransaction();
	else id = FsTransactionStore::nextTransaction();

	// save current directory
	PageDirectory current = AddressSpace::getCurrentSpace();

	/**
	 * First we switch to the requesters space and copy everything that we need below to the stack,
	 * then get the physical addresses of the area where the requesters buffer is:
	 */
	AddressSpace::switchToSpace(requester->process->pageDirectory);

	int requiredPages = PAGE_ALIGN_UP(length) / PAGE_SIZE + 1;
	Local<PhysicalAddress> physPages(new PhysicalAddress[requiredPages]);
	VirtualAddress virtStart = PAGE_ALIGN_DOWN((VirtualAddress ) buffer());
	uint32_t offsetInFirstPage = ((VirtualAddress) buffer()) & PAGE_ALIGN_MASK;

	for (int i = 0; i < requiredPages; i++)
		physPages()[i] = AddressSpace::virtualToPhysical(virtStart + i * PAGE_SIZE);

	/**
	 * Now we switch into the delegates space, copy the required data to the
	 * transaction storage and map all physical pages from the requesters space
	 * to the delegates space.
	 */
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageRead *disc = (FsTaskedDelegateTransactionStorageRead*) transactionStorage();
	disc->offset = fd->offset;
	disc->length = length;
	disc->physFsID = node->physFsID;

	VirtualAddress mappedVirt = delegateThread->process->virtualRanges.allocate(requiredPages);
	for (int i = 0; i < requiredPages; i++)
		AddressSpace::map(mappedVirt + i * PAGE_SIZE, physPages()[i], DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);

	disc->mappingStart = mappedVirt;
	disc->mappingPages = requiredPages;
	disc->mappedBuffer = (void*) (mappedVirt + offsetInFirstPage);

	AddressSpace::switchToSpace(current);

	// send message
	MessageSendStatus sendStatus = MESSAGE_SEND_STATUS_FAILED; // TODO

	if (sendStatus == MESSAGE_SEND_STATUS_SUCCESSFUL)
	{
		// set transaction status
		FsTransactionStore::setStatus(id, FS_TRANSACTION_WAITING);

	}

	else if (sendStatus == MESSAGE_SEND_STATUS_QUEUE_FULL)
	{
		logWarn("%! message queue was full when trying to request read to fs delegate", "filesystem");
		handler->status = FS_READ_BUSY;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

	}

	else if (sendStatus == MESSAGE_SEND_STATUS_FAILED)
	{
		logWarn("%! message sending failed when trying to request read to fs delegate", "filesystem");
		handler->status = FS_READ_ERROR;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	return id;

}

/**
 *
 */
void FsDelegateTasked::finishRead(Thread *requester, FsReadStatus *outStatus, int64_t *outResult, FileDescriptorContent *fd)
{
	// save current directory
	PageDirectory current = AddressSpace::getCurrentSpace();

	// Get values from transaction storage and unmap the mapping
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageRead *rspace = (FsTaskedDelegateTransactionStorageRead*) transactionStorage();
	int64_t lengthRead = rspace->resultRead;
	FsReadStatus status = rspace->resultStatus;

	for (int i = 0; i < rspace->mappingPages; i++)
		AddressSpace::unmap(rspace->mappingStart + i * PAGE_SIZE);

	delegateThread->process->virtualRanges.free(rspace->mappingStart);

	// Now switch to the requesters space and copy data there
	AddressSpace::switchToSpace(requester->process->pageDirectory);

	*outResult = lengthRead;
	*outStatus = status;
	if (lengthRead >= 0)
	{
		fd->offset += lengthRead;
	}

	AddressSpace::switchToSpace(current);
}

/**
 *
 */
FsTransactionID FsDelegateTasked::requestWrite(Thread *requester, FsNode *node, int64_t length, Contextual<uint8_t*> buffer, FileDescriptorContent *fd, FsTransactionHandlerWrite *handler)
{
	// start/repeat transaction
	FsTransactionID id;
	if (handler->wantsRepeatTransaction()) id = handler->getRepeatedTransaction();
	else id = FsTransactionStore::nextTransaction();

	// save current directory
	PageDirectory current = AddressSpace::getCurrentSpace();

	/**
	 * First we switch to the requesters space and copy everything that we need below to the stack,
	 * then get the physical addresses of the area where the requesters buffer is:
	 */
	AddressSpace::switchToSpace(requester->process->pageDirectory);

	int requiredPages = PAGE_ALIGN_UP(length) / PAGE_SIZE + 1;
	Local<PhysicalAddress> physPages(new PhysicalAddress[requiredPages]);
	VirtualAddress virtStart = PAGE_ALIGN_DOWN((VirtualAddress ) buffer());
	uint32_t offsetInFirstPage = ((VirtualAddress) buffer()) & PAGE_ALIGN_MASK;

	for (int i = 0; i < requiredPages; i++)
		physPages()[i] = AddressSpace::virtualToPhysical(virtStart + i * PAGE_SIZE);

	/**
	 * Now we switch into the delegates space, copy the required data to the
	 * transaction storage and map all physical pages from the requesters space
	 * to the delegates space.
	 */
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageWrite *disc = (FsTaskedDelegateTransactionStorageWrite*) transactionStorage();
	disc->offset = fd->offset;
	disc->length = length;
	disc->physFsID = node->physFsID;

	VirtualAddress mappedVirt = delegateThread->process->virtualRanges.allocate(requiredPages);
	for (int i = 0; i < requiredPages; i++)
		AddressSpace::map(mappedVirt + i * PAGE_SIZE, physPages()[i], DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);

	disc->mappingStart = mappedVirt;
	disc->mappingPages = requiredPages;
	disc->mappedBuffer = (void*) (mappedVirt + offsetInFirstPage);

	AddressSpace::switchToSpace(current);

	// send message
	MessageSendStatus sendStatus = MESSAGE_SEND_STATUS_FAILED; // TODO

	// set transaction status
	if (sendStatus == MESSAGE_SEND_STATUS_SUCCESSFUL) FsTransactionStore::setStatus(id, FS_TRANSACTION_WAITING);

	else if (sendStatus == MESSAGE_SEND_STATUS_QUEUE_FULL)
	{
		logWarn("%! message queue was full when trying to request write to fs delegate", "filesystem");
		handler->status = FS_WRITE_BUSY;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	else if (sendStatus == MESSAGE_SEND_STATUS_FAILED)
	{
		logWarn("%! message sending failed when trying to request write to fs delegate", "filesystem");
		handler->status = FS_WRITE_ERROR;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	return id;
}

/**
 *
 */
void FsDelegateTasked::finishWrite(Thread *requester, FsWriteStatus *outStatus, int64_t *outResult, FileDescriptorContent *fd)
{
	// save current directory
	PageDirectory current = AddressSpace::getCurrentSpace();

	// Get values from transaction storage and unmap the mapping
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageWrite *storage = (FsTaskedDelegateTransactionStorageWrite*) transactionStorage();
	int64_t lengthWrite = storage->resultWrite;
	FsReadStatus status = storage->resultStatus;

	for (int i = 0; i < storage->mappingPages; i++)
		AddressSpace::unmap(storage->mappingStart + i * PAGE_SIZE);

	delegateThread->process->virtualRanges.free(storage->mappingStart);

	// Now switch to the requesters space and copy data there
	AddressSpace::switchToSpace(requester->process->pageDirectory);

	*outResult = lengthWrite;
	*outStatus = status;
	if (lengthWrite >= 0) fd->offset += lengthWrite;

	AddressSpace::switchToSpace(current);
}

/**
 *
 */
FsTransactionID FsDelegateTasked::requestGetLength(Thread *requester, FsNode *node, FsTransactionHandlerGetLength *handler)
{
	// the ramdisk handler is doing it's work immediately and doesn't request another process
	FsTransactionID id = FsTransactionStore::nextTransaction();

	/**
	 * Switch to the delegates space and fill the transaction storage.
	 */
	PageDirectory current = AddressSpace::getCurrentSpace();
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageGetLength *disc = (FsTaskedDelegateTransactionStorageGetLength*) transactionStorage();
	disc->physFsID = node->physFsID;

	AddressSpace::switchToSpace(current);

	// update the status / notify delegate thread
	MessageSendStatus sendStatus = MESSAGE_SEND_STATUS_FAILED; // TODO

	// task was requested, wait for answer
	if (sendStatus == MESSAGE_SEND_STATUS_SUCCESSFUL) FsTransactionStore::setStatus(id, FS_TRANSACTION_WAITING);
	else if (sendStatus == MESSAGE_SEND_STATUS_QUEUE_FULL)
	{
		logWarn("%! message queue was full when trying to request get-length to fs delegate", "filesystem");
		handler->status = FS_LENGTH_BUSY;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	else if (sendStatus == MESSAGE_SEND_STATUS_FAILED)
	{
		logWarn("%! message sending failed when trying to request get-length to fs delegate", "filesystem");
		handler->status = FS_LENGTH_ERROR;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	return id;
}

/**
 *
 */
void FsDelegateTasked::finishGetLength(Thread *requester, FsTransactionHandlerGetLength *handler)
{
	// save current directory
	PageDirectory current = AddressSpace::getCurrentSpace();

	// Get values from transaction storage and unmap the mapping
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageGetLength *storage = (FsTaskedDelegateTransactionStorageGetLength*) transactionStorage();
	int64_t length = storage->resultLength;
	FsReadStatus status = storage->resultStatus;

	// Now switch to the requesters space and copy data there
	AddressSpace::switchToSpace(requester->process->pageDirectory);

	handler->length = length;
	handler->status = status;

	AddressSpace::switchToSpace(current);
}

/**
 *
 */
FsTransactionID FsDelegateTasked::requestDirectoryRefresh(Thread *requester, FsNode *node, FsTransactionHandlerDirectoryRefresh *handler)
{
	FsTransactionID id = FsTransactionStore::nextTransaction();

	/**
	 * Switch to the delegates space and fill the transaction storage.
	 */
	PageDirectory current = AddressSpace::getCurrentSpace();
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageDirectoryRefresh *disc = (FsTaskedDelegateTransactionStorageDirectoryRefresh*) transactionStorage();
	disc->parentPhysFsID = node->physFsID;
	disc->parentVirtFsID = node->id;

	AddressSpace::switchToSpace(current);

	// update the status / notify delegate thread
	MessageSendStatus sendStatus = MESSAGE_SEND_STATUS_FAILED; // TODO

	// task was requested, wait for answer
	if (sendStatus == MESSAGE_SEND_STATUS_SUCCESSFUL) FsTransactionStore::setStatus(id, FS_TRANSACTION_WAITING);
	else if (sendStatus == MESSAGE_SEND_STATUS_QUEUE_FULL)
	{
		logWarn("%! message queue was full when trying to request read-directory on fs delegate", "filesystem");
		handler->status = FS_DIRECTORY_REFRESH_BUSY;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	else if (sendStatus == MESSAGE_SEND_STATUS_FAILED)
	{
		logWarn("%! message sending failed when trying to request read-directory on fs delegate", "filesystem");
		handler->status = FS_DIRECTORY_REFRESH_ERROR;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	return id;
}

/**
 *
 */
void FsDelegateTasked::finishDirectoryRefresh(Thread *requester, FsTransactionHandlerDirectoryRefresh *handler)
{
	// save current directory
	PageDirectory current = AddressSpace::getCurrentSpace();

	// Get values from transaction storage and unmap the mapping
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageDirectoryRefresh *storage = (FsTaskedDelegateTransactionStorageDirectoryRefresh*) transactionStorage();
	FsDirectoryRefreshStatus status = storage->resultStatus;
	FsVirtID parentID = storage->parentVirtFsID;

	// Now switch to the requesters space and copy data there
	AddressSpace::switchToSpace(requester->process->pageDirectory);

	// update the folder
	FsNode *folder = FileSystem::getNodeById(parentID);
	if (folder) folder->contentsValid = true;

	handler->status = status;

	AddressSpace::switchToSpace(current);
}

/**
 *
 */
FsTransactionID FsDelegateTasked::requestOpen(Thread *requester, FsNode *node, char *filename, int32_t flags, int32_t mode, FsTransactionHandlerOpen *handler)
{
	// the ramdisk handler is doing it's work immediately and doesn't request another process
	FsTransactionID id = FsTransactionStore::nextTransaction();
	bool configurationFine = true;

	/**
	 * Switch to the delegates space and fill the transaction storage.
	 */
	PageDirectory current = AddressSpace::getCurrentSpace();
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageOpen *disc = (FsTaskedDelegateTransactionStorageOpen*) transactionStorage();
	disc->physFsID = node->physFsID;

	int childlen = String::length(filename);
	if (childlen > FILENAME_MAX)
	{
		logInfo("tried to open a node that has a name with an illegal length");
		configurationFine = false;
	}

	else Memory::copy(disc->name, filename, childlen + 1);

	AddressSpace::switchToSpace(current);

	if (!configurationFine)
	{
		handler->status = FS_OPEN_ERROR;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
		return id;
	}

	// update the status / notify delegate thread
	MessageSendStatus sendStatus = MESSAGE_SEND_STATUS_FAILED; // TODO

	// task was requested, wait for answer
	if (sendStatus == MESSAGE_SEND_STATUS_SUCCESSFUL) FsTransactionStore::setStatus(id, FS_TRANSACTION_WAITING);
	else if (sendStatus == MESSAGE_SEND_STATUS_QUEUE_FULL)
	{
		logWarn("%! message queue was full when trying to request open to fs delegate", "filesystem");
		handler->status = FS_OPEN_BUSY;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	else if (sendStatus == MESSAGE_SEND_STATUS_FAILED)
	{
		logWarn("%! message sending failed when trying to request open to fs delegate", "filesystem");
		handler->status = FS_OPEN_ERROR;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	return id;
}

/**
 *
 */
void FsDelegateTasked::finishOpen(Thread *requester, FsTransactionHandlerOpen *handler)
{
	// save current directory
	PageDirectory current = AddressSpace::getCurrentSpace();

	// Get values from transaction storage and unmap the mapping
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageOpen *storage = (FsTaskedDelegateTransactionStorageOpen*) transactionStorage();
	FsOpenStatus status = storage->resultStatus;

	// Now switch to the requesters space and copy data there
	AddressSpace::switchToSpace(requester->process->pageDirectory);

	handler->status = status;

	AddressSpace::switchToSpace(current);
}

/**
 *
 */
FsTransactionID FsDelegateTasked::requestClose(Thread *requester, FsTransactionHandlerClose *handler, FileDescriptorContent *fd, FsNode *node)
{
	FsTransactionID id = FsTransactionStore::nextTransaction();

	// fill transaction storage
	PageDirectory current = AddressSpace::getCurrentSpace();
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageClose *disc = (FsTaskedDelegateTransactionStorageClose*) transactionStorage();
	disc->physFsID = node->physFsID;

	AddressSpace::switchToSpace(current);

	// update the status / notify delegate thread
	MessageSendStatus sendStatus = MESSAGE_SEND_STATUS_FAILED; // TODO

	// task was requested, wait for answer
	if (sendStatus == MESSAGE_SEND_STATUS_SUCCESSFUL) FsTransactionStore::setStatus(id, FS_TRANSACTION_WAITING);
	else if (sendStatus == MESSAGE_SEND_STATUS_QUEUE_FULL)
	{
		logWarn("%! message queue was full when trying to request close on fs delegate", "filesystem");
		handler->status = FS_CLOSE_BUSY;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	else if (sendStatus == MESSAGE_SEND_STATUS_FAILED)
	{
		logWarn("%! message sending failed when trying to request close on fs delegate", "filesystem");
		handler->status = FS_CLOSE_ERROR;
		FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
	}

	return id;
}

/**
 *
 */
void FsDelegateTasked::finishClose(Thread *requester, FsTransactionHandlerClose *handler)
{
	PageDirectory current = AddressSpace::getCurrentSpace();

	// copy values from transaction storage to local stack
	AddressSpace::switchToSpace(delegateThread->process->pageDirectory);

	FsTaskedDelegateTransactionStorageClose *storage = (FsTaskedDelegateTransactionStorageClose*) transactionStorage();
	FsDirectoryRefreshStatus status = storage->resultStatus;

	// copy values to the handler
	AddressSpace::switchToSpace(requester->process->pageDirectory);
	handler->status = status;

	// switch back
	AddressSpace::switchToSpace(current);
}
