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

#ifndef EVA_FILESYSTEM_TRANSACTION_HANDLER_GET_LENGTH_SEEK
#define EVA_FILESYSTEM_TRANSACTION_HANDLER_GET_LENGTH_SEEK

#include "filesystem/FsDescriptors.hpp"
#include "filesystem/FsNode.hpp"
#include "filesystem/FsTransactionHandlerGetLength.hpp"
#include "logger/logger.hpp"
#include "memory/contextual.hpp"

/**
 *
 */
class FsTransactionHandlerGetLengthSeek : public FsTransactionHandlerGetLength {
public:
    FileDescriptorContent*     fd;
    Contextual<SyscallFsSeek*> data;

    /**
     *
     */
    FsTransactionHandlerGetLengthSeek(FileDescriptorContent*     fd,
                                      FsNode*                    node,
                                      Contextual<SyscallFsSeek*> data)
        : FsTransactionHandlerGetLength(node), fd(fd), data(data) {
    }

    /**
     *
     */
    virtual void performAfterwork(Thread* thread) {
        if ( status == FS_LENGTH_SUCCESSFUL ) {
            // add amount to offset
            if ( data()->mode == FS_SEEK_CUR )
                fd->offset += data()->amount;
            else if ( data()->mode == FS_SEEK_SET )
                fd->offset = data()->amount;
            else if ( data()->mode == FS_SEEK_END )
                fd->offset = length - data()->amount;

            // validate offset
            if ( fd->offset > length )
                fd->offset = length;
            if ( fd->offset < 0 )
                fd->offset = 0;

            data()->result = fd->offset;
            data()->status = FS_SEEK_SUCCESSFUL;
        }

        else {
            data()->result = -1;
            data()->status = FS_SEEK_ERROR;
        }
    }
};

#endif
