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

#include "filesystem/FsDelegateRamdisk.hpp"

#include "eva/utils/local.hpp"
#include "EvangelionNG.hpp"
#include "filesystem/filesystem.hpp"
#include "logger/logger.hpp"
#include "utils/string.hpp"

/**
 *
 */
FsNode* FsDelegateRamdisk::createVfsNode(RamdiskEntry* ramdiskNode, FsNode* parent) {
    FsNode* node   = FileSystem::createNode();
    node->physFsID = ramdiskNode->id;

    if ( ramdiskNode->type == RAMDISK_ENTRY_TYPE_FILE )
        node->type = FS_NODE_TYPE_FILE;
    else
        node->type = FS_NODE_TYPE_FOLDER;

    int len    = String::length(ramdiskNode->name);
    node->name = new char[len + 1];
    Memory::copy(node->name, ramdiskNode->name, len);
    node->name[len] = 0;

    // add it to the parent
    parent->addChild(node);
    DEBUG_INTERFACE_FILESYSTEM_UPDATE_NODE(node);

    return node;
}

/**
 *
 */
FsTransactionID FsDelegateRamdisk::requestDiscovery(Thread*                        requester,
                                                    FsNode*                        parent,
                                                    char*                          child,
                                                    FsTransactionHandlerDiscovery* handler) {
    // the ramdisk handler is doing it's work immediately and doesn't request another process
    FsTransactionID id = FsTransactionStore::nextTransaction();

    // find on ramdisk
    RamdiskEntry* ramdiskParent;
    if ( parent->type == FS_NODE_TYPE_MOUNTPOINT )
        ramdiskParent = EvaKernel::ramdisk->getRoot();
    else
        ramdiskParent = EvaKernel::ramdisk->findById(parent->physFsID);

    if ( ramdiskParent ) {
        RamdiskEntry* ramdiskNode = EvaKernel::ramdisk->findChild(ramdiskParent, child);

        if ( ramdiskNode ) {
            // create the VFS node
            createVfsNode(ramdiskNode, parent);
            handler->status = FS_DISCOVERY_SUCCESSFUL;
        }

        else
            handler->status = FS_DISCOVERY_NOT_FOUND;
    }

    else
        handler->status = FS_DISCOVERY_NOT_FOUND;
    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

    return id;
}

/**
 *
 */
void FsDelegateRamdisk::finishDiscovery(Thread* requester, FsTransactionHandlerDiscovery* handler) {
    // nothing to do here
}

/**
 *
 */
FsTransactionID FsDelegateRamdisk::requestRead(Thread*                   requester,
                                               FsNode*                   node,
                                               int64_t                   length,
                                               Contextual<uint8_t*>      buffer,
                                               FileDescriptorContent*    fd,
                                               FsTransactionHandlerRead* handler) {
    // start/repeat transaction
    FsTransactionID id;
    if ( handler->wantsRepeatTransaction() )
        id = handler->getRepeatedTransaction();

    else
        id = FsTransactionStore::nextTransaction();

    RamdiskEntry* ramdiskNode = EvaKernel::ramdisk->findById(node->physFsID);
    if ( ramdiskNode == 0 ) {
        handler->status = FS_READ_INVALID_FD;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
        return id;
    }

    // check if node is valid
    if ( ramdiskNode->data == nullptr ) {
        logWarn("%! tried to read from a node %i that has no buffer", "ramdisk", node->physFsID);
        handler->status = FS_READ_ERROR;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

        return id;
    }

    // read data into buffer
    int64_t copyAmount = ((fd->offset + length) >= ramdiskNode->datalength)
                           ? (ramdiskNode->datalength - fd->offset)
                           : length;
    if ( copyAmount > 0 ) {
        Memory::copy(buffer(), &ramdiskNode->data[fd->offset], copyAmount);
        fd->offset += copyAmount;
    }
    handler->result = copyAmount;
    handler->status = FS_READ_SUCCESSFUL;
    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

    return id;
}

/**
 *
 */
void FsDelegateRamdisk::finishRead(Thread*                requester,
                                   FsReadStatus*          outStatus,
                                   int64_t*               outResult,
                                   FileDescriptorContent* fd) {
}

/**
 *
 */
FsTransactionID FsDelegateRamdisk::requestWrite(Thread*                    requester,
                                                FsNode*                    node,
                                                int64_t                    length,
                                                Contextual<uint8_t*>       buffer,
                                                FileDescriptorContent*     fd,
                                                FsTransactionHandlerWrite* handler) {
    // start/repeat transaction
    FsTransactionID id;
    if ( handler->wantsRepeatTransaction() )
        id = handler->getRepeatedTransaction();
    else
        id = FsTransactionStore::nextTransaction();

    RamdiskEntry* ramdiskNode = EvaKernel::ramdisk->findById(node->physFsID);
    if ( ramdiskNode == 0 ) {
        handler->status = FS_WRITE_INVALID_FD;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

        return id;
    }

    // copy data from ramdisk memory into variable memory
    if ( ramdiskNode->dataOnRamdisk ) {
        uint32_t buflen    = ramdiskNode->datalength * 1.2;
        uint8_t* newBuffer = new uint8_t[buflen];
        Memory::copy(newBuffer, ramdiskNode->data, ramdiskNode->datalength);
        ramdiskNode->data                = newBuffer;
        ramdiskNode->notOnRdBufferLength = buflen;
        ramdiskNode->dataOnRamdisk       = false;
    }

    else if ( ramdiskNode->data == nullptr ) {
        uint32_t initbuflen              = 32;
        ramdiskNode->data                = new uint8_t[initbuflen];
        ramdiskNode->notOnRdBufferLength = initbuflen;
    }

    // when file descriptor shall append, set it to the end
    if ( fd->openFlags & FILE_FLAG_MODE_APPEND )
        fd->offset = ramdiskNode->datalength;

    // expand buffer until enough space is available
    uint32_t space;
    while ( (space = ramdiskNode->notOnRdBufferLength - fd->offset) < length ) {
        uint32_t buflen    = ramdiskNode->notOnRdBufferLength * 1.2;
        uint8_t* newBuffer = new uint8_t[buflen];
        Memory::copy(newBuffer, ramdiskNode->data, ramdiskNode->datalength);

        delete ramdiskNode->data;

        ramdiskNode->data                = newBuffer;
        ramdiskNode->notOnRdBufferLength = buflen;
    }

    // copy data
    Memory::copy(&ramdiskNode->data[fd->offset], buffer(), length);
    ramdiskNode->datalength = fd->offset + length;
    fd->offset += length;

    handler->result = length;
    handler->status = FS_WRITE_SUCCESSFUL;
    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

    return id;
}

/**
 *
 */
void FsDelegateRamdisk::finishWrite(Thread*                requester,
                                    FsWriteStatus*         outStatus,
                                    int64_t*               outResult,
                                    FileDescriptorContent* fd) {
}

/**
 *
 */
FsTransactionID FsDelegateRamdisk::requestGetLength(Thread*                        requester,
                                                    FsNode*                        node,
                                                    FsTransactionHandlerGetLength* handler) {
    // the ramdisk handler is doing it's work immediately and doesn't request another process
    FsTransactionID id = FsTransactionStore::nextTransaction();

    RamdiskEntry* ramdiskNode = EvaKernel::ramdisk->findById(node->physFsID);
    if ( ramdiskNode == 0 ) {
        handler->status = FS_LENGTH_NOT_FOUND;
        handler->length = 0;
    }

    else {
        handler->status = FS_LENGTH_SUCCESSFUL;
        handler->length = ramdiskNode->datalength;
    }

    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    return id;
}

/**
 *
 */
void FsDelegateRamdisk::finishGetLength(Thread* requester, FsTransactionHandlerGetLength* handler) {
}

/**
 *
 */
FsTransactionID
FsDelegateRamdisk::requestDirectoryRefresh(Thread*                               requester,
                                           FsNode*                               folder,
                                           FsTransactionHandlerDirectoryRefresh* handler) {
    FsTransactionID id = FsTransactionStore::nextTransaction();

    RamdiskEntry* rdFolder = EvaKernel::ramdisk->findById(folder->physFsID);
    if ( rdFolder == 0 )
        handler->status = FS_DIRECTORY_REFRESH_ERROR;

    else {
        // create all nodes that not yet exist
        int           position = 0;
        RamdiskEntry* rdChild;

        while ( (rdChild = EvaKernel::ramdisk->getChildAt(folder->physFsID, position++)) != 0 ) {
            // get real path to parent
            Local<char> absolute(new char[PATH_MAX]);
            FileSystem::getRealPathToNode(folder, absolute());

            // append child name
            int absCurLen = String::length((const char*)absolute());
            int childlen  = String::length(rdChild->name);
            Memory::copy(&absolute()[absCurLen], "/", 1);
            Memory::copy(&absolute()[absCurLen + 1], rdChild->name, childlen);
            absolute()[absCurLen + 1 + childlen] = 0;

            // check if file exists as vfs node
            FsNode*     fsChildsParent = 0;
            FsNode*     fsChild        = 0;
            Local<char> current(new char[PATH_MAX]);
            FileSystem::findExisting(absolute(), &fsChildsParent, &fsChild, current(), true);

            // if not, create it
            if ( fsChild == 0 )
                fsChild = createVfsNode(rdChild, folder);
        }

        // finish the transaction
        folder->contentsValid = true;
        handler->status       = FS_DIRECTORY_REFRESH_SUCCESSFUL;
    }

    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    return id;
}

/**
 *
 */
void FsDelegateRamdisk::finishDirectoryRefresh(Thread*                               requester,
                                               FsTransactionHandlerDirectoryRefresh* handler) {
}

/**
 *
 */
FsTransactionID FsDelegateRamdisk::requestOpen(Thread*                   requester,
                                               FsNode*                   node,
                                               char*                     filename,
                                               int32_t                   flags,
                                               int32_t                   mode,
                                               FsTransactionHandlerOpen* handler) {
    FsTransactionID id = FsTransactionStore::nextTransaction();

    RamdiskEntry* ramdiskNode = EvaKernel::ramdisk->findById(node->physFsID);

    if ( handler->discoveryStatus == FS_DISCOVERY_SUCCESSFUL ) {
        if ( ramdiskNode->type != RAMDISK_ENTRY_TYPE_FILE ) {
            logWarn("%! only files can be opened, given node ('%s') was a %i",
                    "filesystem",
                    ramdiskNode->name,
                    ramdiskNode->type);
            handler->status = FS_OPEN_ERROR;
        }

        else {
            // truncate file if requested
            if ( flags & FILE_FLAG_MODE_TRUNCATE ) {
                // only applies when data no more used from ramdisk memory
                if ( !ramdiskNode->dataOnRamdisk ) {
                    // completely remove the buffer
                    ramdiskNode->datalength          = 0;
                    ramdiskNode->notOnRdBufferLength = 0;

                    delete ramdiskNode->data;

                    ramdiskNode->data = 0;
                }
            }

            handler->status = FS_OPEN_SUCCESSFUL;
        }
    }

    else if ( handler->discoveryStatus == FS_DISCOVERY_NOT_FOUND ) {
        if ( flags & FILE_FLAG_MODE_CREATE ) {
            // create the filesystem file
            RamdiskEntry* newRamdiskEntry = EvaKernel::ramdisk->createChild(ramdiskNode, filename);

            handler->node   = createVfsNode(newRamdiskEntry, node);
            handler->status = FS_OPEN_SUCCESSFUL;
        }

        // return with failure
        else
            handler->status = FS_OPEN_NOT_FOUND;
    }

    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    return id;
}

/**
 *
 */
void FsDelegateRamdisk::finishOpen(Thread* requester, FsTransactionHandlerOpen* handler) {
}

/**
 *
 */
FsTransactionID FsDelegateRamdisk::requestClose(Thread*                    requester,
                                                FsTransactionHandlerClose* handler,
                                                FileDescriptorContent*     fd,
                                                FsNode*                    node) {
    FsTransactionID id = FsTransactionStore::nextTransaction();

    // nothing to do here
    handler->status = FS_CLOSE_SUCCESSFUL;
    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

    return id;
}

/**
 *
 */
void FsDelegateRamdisk::finishClose(Thread* requester, FsTransactionHandlerClose* handler) {
}
