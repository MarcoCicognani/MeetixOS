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

#ifndef EVA_FILESYSTEM_FILESYSTEMRAMDISKDELEGATE
#define EVA_FILESYSTEM_FILESYSTEMRAMDISKDELEGATE

#include "eva/stdint.h"
#include "filesystem/FsDelegate.hpp"
#include "memory/contextual.hpp"
#include "utils/HashMap.hpp"

/**
 *
 */
class FsDelegateRamdisk : public FsDelegate {
private:
    /**
     *
     */
    FsNode* createVfsNode(RamdiskEntry* ramdiskNode, FsNode* parent);

public:
    /**
     *
     */
    virtual ~FsDelegateRamdisk() {
    }

    /**
     *
     */
    virtual FsTransactionID requestDiscovery(Thread*                        requester,
                                             FsNode*                        parent,
                                             char*                          child,
                                             FsTransactionHandlerDiscovery* handler);

    /**
     *
     */
    virtual void finishDiscovery(Thread* requester, FsTransactionHandlerDiscovery* handler);

    /**
     *
     */
    virtual FsTransactionID requestRead(Thread*                   requester,
                                        FsNode*                   node,
                                        int64_t                   length,
                                        Contextual<uint8_t*>      buffer,
                                        FileDescriptorContent*    fd,
                                        FsTransactionHandlerRead* handler);

    /**
     *
     */
    virtual void finishRead(Thread*                requester,
                            FsReadStatus*          outStatus,
                            int64_t*               outResult,
                            FileDescriptorContent* fd);

    /**
     *
     */
    virtual FsTransactionID requestWrite(Thread*                    requester,
                                         FsNode*                    node,
                                         int64_t                    length,
                                         Contextual<uint8_t*>       buffer,
                                         FileDescriptorContent*     fd,
                                         FsTransactionHandlerWrite* handler);

    /**
     *
     */
    virtual void finishWrite(Thread*                requester,
                             FsWriteStatus*         outStatus,
                             int64_t*               outResult,
                             FileDescriptorContent* fd);

    /**
     *
     */
    virtual FsTransactionID
    requestGetLength(Thread* requester, FsNode* node, FsTransactionHandlerGetLength* handler);

    /**
     *
     */
    virtual void finishGetLength(Thread* requester, FsTransactionHandlerGetLength* handler);

    /**
     *
     */
    virtual FsTransactionID requestDirectoryRefresh(Thread*                               requester,
                                                    FsNode*                               node,
                                                    FsTransactionHandlerDirectoryRefresh* handler);

    /**
     *
     */
    virtual void finishDirectoryRefresh(Thread*                               requester,
                                        FsTransactionHandlerDirectoryRefresh* handler);

    /**
     *
     */
    virtual FsTransactionID requestOpen(Thread*                   requester,
                                        FsNode*                   node,
                                        char*                     filename,
                                        int32_t                   flags,
                                        int32_t                   mode,
                                        FsTransactionHandlerOpen* handler);

    /**
     *
     */
    virtual void finishOpen(Thread* requester, FsTransactionHandlerOpen* handler);

    /**
     *
     */
    virtual FsTransactionID requestClose(Thread*                    requester,
                                         FsTransactionHandlerClose* handler,
                                         FileDescriptorContent*     fd,
                                         FsNode*                    node);

    /**
     *
     */
    virtual void finishClose(Thread* requester, FsTransactionHandlerClose* handler);
};

#endif
