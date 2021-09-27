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

#include "filesystem/FsDelegatePipe.hpp"

#include "EvangelionNG.hpp"
#include "filesystem/filesystem.hpp"
#include "logger/logger.hpp"
#include "utils/string.hpp"

/**
 * Pipes do not support discovery. We put a warning out though.
 */
FsTransactionID FsDelegatePipe::requestDiscovery(Thread*                        requester,
                                                 FsNode*                        parent,
                                                 char*                          child,
                                                 FsTransactionHandlerDiscovery* handler) {
    FsTransactionID id = FsTransactionStore::nextTransaction();
    handler->status    = FS_DISCOVERY_ERROR;

    logWarn("%! no discovery support", "pipes");
    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

    return id;
}

/**
 *
 */
void FsDelegatePipe::finishDiscovery(Thread* requester, FsTransactionHandlerDiscovery* handler) {
    // nothing to do here
}

/**
 *
 */
FsTransactionID FsDelegatePipe::requestRead(Thread*                   requester,
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

    // find the right pipe
    auto pipe = Pipes::get(node->physFsID);
    if ( pipe == nullptr ) {
        // cancel with error if pipe not found
        handler->result = -1;
        handler->status = FS_READ_ERROR;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
        return id;
    }

    // find out what size can be read at maximum
    length = (pipe->size >= length) ? length : pipe->size;

    // calculate how much space remains until the end of the pipe
    uint32_t spaceToEnd = ((uint32_t)pipe->buffer + pipe->capacity) - (uint32_t)pipe->read;

    // if we want to read more than remaining
    if ( length > spaceToEnd ) {
        // copy bytes from the location of the read pointer
        Memory::copy(buffer(), pipe->read, spaceToEnd);

        // copy remaining data
        uint32_t remaining = length - spaceToEnd;
        Memory::copy(&buffer()[spaceToEnd], pipe->buffer, remaining);

        // set the read pointer to it's new location
        pipe->read = (uint8_t*)((uint32_t)pipe->buffer + remaining);
    }

    else {
        // there are enough bytes left from read pointer, copy it to buffer
        Memory::copy(buffer(), pipe->read, length);

        // set the read pointer to it's new location
        pipe->read = (uint8_t*)((uint32_t)pipe->read + length);
    }

    // reset read pointer if end reached
    if ( pipe->read == pipe->buffer + pipe->capacity )
        pipe->read = pipe->buffer;

    // if any bytes could be copied
    if ( length > 0 ) {
        // decrease pipes remaining bytes
        pipe->size -= length;

        // finish with success
        handler->result = length;
        handler->status = FS_READ_SUCCESSFUL;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    }

    else if ( node->isBlocking ) {
        // avoid block if no one else has access to pipe
        if ( !Pipes::hasReferenceFromOtherProcess(pipe, requester->process->main->id) ) {
            handler->result = 0;
            handler->status = FS_READ_ERROR;
            FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
        }

        else
            FsTransactionStore::setStatus(id, FS_TRANSACTION_REPEAT);
    }

    else {
        // otherwise just finished with successful read status
        handler->result = 0;
        handler->status = FS_READ_SUCCESSFUL;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    }

    return id;
}

/**
 *
 */
void FsDelegatePipe::finishRead(Thread*                requester,
                                FsReadStatus*          outStatus,
                                int64_t*               outResult,
                                FileDescriptorContent* fd) {
    // nothing to do
}

/**
 *
 */
FsTransactionID FsDelegatePipe::requestWrite(Thread*                    requester,
                                             FsNode*                    node,
                                             int64_t                    length,
                                             Contextual<const uint8_t*> buffer,
                                             FileDescriptorContent*     fd,
                                             FsTransactionHandlerWrite* handler) {
    // start/repeat transaction
    FsTransactionID id;
    if ( handler->wantsRepeatTransaction() )
        id = handler->getRepeatedTransaction();

    else
        id = FsTransactionStore::nextTransaction();

    // find the pipe to write to
    auto pipe = Pipes::get(node->physFsID);
    if ( pipe == nullptr ) {
        handler->result = -1;
        handler->status = FS_WRITE_ERROR;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

        return id;
    }

    uint32_t space = (pipe->capacity - pipe->size);

    if ( space > 0 ) {
        // check how many bytes can be written
        length = (space >= length) ? length : space;

        // check how many bytes can be written at the write pointer
        uint32_t spaceToEnd = ((uint32_t)pipe->buffer + pipe->capacity) - (uint32_t)pipe->write;

        if ( length > spaceToEnd ) {
            // write bytes at the write pointer
            Memory::copy(pipe->write, buffer(), spaceToEnd);

            // write remaining bytes to the start of the pipe
            uint32_t remain = length - spaceToEnd;
            Memory::copy(pipe->buffer, &buffer()[spaceToEnd], remain);

            // set the write pointer to the new location
            pipe->write = (uint8_t*)((uint32_t)pipe->buffer + remain);

        }

        else {
            // just write bytes at write pointer
            Memory::copy(pipe->write, buffer(), length);

            // set the write pointer to the new location
            pipe->write = (uint8_t*)((uint32_t)pipe->write + length);
        }

        // reset write pointer if end reached
        if ( pipe->write == pipe->buffer + pipe->capacity )
            pipe->write = pipe->buffer;

        pipe->size += length;
        handler->result = length;
        handler->status = FS_WRITE_SUCCESSFUL;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    }

    else if ( node->isBlocking ) {
        // try again later
        if ( Pipes::hasReferenceFromOtherProcess(pipe, requester->process->main->id) )
            FsTransactionStore::setStatus(id, FS_TRANSACTION_REPEAT);

        else {
            handler->result = -1;
            handler->status = FS_WRITE_ERROR;
            FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
        }
    }

    else {
        handler->result = -1;
        handler->status = FS_WRITE_SUCCESSFUL;
        FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    }

    return id;
}

/**
 *
 */
void FsDelegatePipe::finishWrite(Thread*                requester,
                                 FsWriteStatus*         outStatus,
                                 int64_t*               outResult,
                                 FileDescriptorContent* fd) {
}

/**
 *
 */
FsTransactionID FsDelegatePipe::requestGetLength(Thread*                        requester,
                                                 FsNode*                        node,
                                                 FsTransactionHandlerGetLength* handler) {
    FsTransactionID id = FsTransactionStore::nextTransaction();

    auto pipe = Pipes::get(node->physFsID);
    if ( pipe ) {
        handler->length = pipe->capacity;
        handler->status = FS_LENGTH_SUCCESSFUL;
    }

    else {
        handler->length = -1;
        handler->status = FS_LENGTH_ERROR;
    }
    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);

    return id;
}

/**
 *
 */
void FsDelegatePipe::finishGetLength(Thread* requester, FsTransactionHandlerGetLength* handler) {
}

/**
 *
 */
FsTransactionID
FsDelegatePipe::requestDirectoryRefresh(Thread*                               requester,
                                        FsNode*                               folder,
                                        FsTransactionHandlerDirectoryRefresh* handler) {
    FsTransactionID id = FsTransactionStore::nextTransaction();

    // pipe has no children
    folder->contentsValid = true;
    handler->status       = FS_DIRECTORY_REFRESH_ERROR;

    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    return id;
}

/**
 *
 */
void FsDelegatePipe::finishDirectoryRefresh(Thread*                               requester,
                                            FsTransactionHandlerDirectoryRefresh* handler) {
}

/**
 *
 */
FsTransactionID FsDelegatePipe::requestOpen(Thread*                   requester,
                                            FsNode*                   node,
                                            const char*               filename,
                                            int32_t                   flags,
                                            int32_t                   mode,
                                            FsTransactionHandlerOpen* handler) {
    FsTransactionID id = FsTransactionStore::nextTransaction();

    // pipes can never be opened
    logWarn("%! pipes can not be opened explicitly", "filesystem");
    handler->status = FS_OPEN_ERROR;

    FsTransactionStore::setStatus(id, FS_TRANSACTION_FINISHED);
    return id;
}

/**
 *
 */
void FsDelegatePipe::finishOpen(Thread* requester, FsTransactionHandlerOpen* handler) {
}

/**
 *
 */
FsTransactionID FsDelegatePipe::requestClose(Thread*                    requester,
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
void FsDelegatePipe::finishClose(Thread* requester, FsTransactionHandlerClose* handler) {
}
