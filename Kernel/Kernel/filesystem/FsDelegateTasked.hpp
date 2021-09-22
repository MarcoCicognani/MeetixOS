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

#ifndef EVA_FILESYSTEM_FILESYSTEMRAMDISKTASKED
#define EVA_FILESYSTEM_FILESYSTEMRAMDISKTASKED

#include "Api/StdInt.h"
#include "filesystem/FsDelegate.hpp"
#include "memory/contextual.hpp"
#include "tasking/tasking.hpp"

/**
 *
 */
class FsDelegateTasked : public FsDelegate {
private:
    Contextual<void*> transactionStorage;
    PhysAddr          transactionStoragePhys;
    Thread*           delegateThread;

public:
    /**
     *
     */
    FsDelegateTasked(Thread* delegateThread);

    /**
     *
     */
    virtual ~FsDelegateTasked() {
    }

    /**
     * Prepares the task delegate by setting up a transaction storage
     * within the delegates address space.
     *
     * @param outTransactionStorage
     * 		is filled with the address of the transaction storage within
     * 		the delegates address space
     *
     * @return true if the preparation was successful
     */
    bool prepare(VirtAddr* outTransactionStorage);

    /**
     *
     */
    virtual FsTransactionID requestDiscovery(Thread*                        requester,
                                             FsNode*                        parent,
                                             char*                          child,
                                             FsTransactionHandlerDiscovery* handler);

    /**
     * Copies the status code from the transaction storage.
     */
    virtual void finishDiscovery(Thread* requester, FsTransactionHandlerDiscovery* handler);

    /**
     * This implementation first sets up a transaction storage in the delegates address
     * space containing the read request data and the maps the pages that contain the
     * requesters buffer into the delegates address space.
     *
     * Also, the pointer to the call data struct is put into the {g_fs_transaction_store}.
     * Once the transaction is finished, the {WaiterFsRead} calls this delegate to
     * finish the read operation, passing that call struct.
     */
    virtual FsTransactionID requestRead(Thread*                   requester,
                                        FsNode*                   node,
                                        int64_t                   length,
                                        Contextual<uint8_t*>      buffer,
                                        FileDescriptorContent*    fd,
                                        FsTransactionHandlerRead* handler);

    /**
     * Is called once a read transaction was successful. The call data struct (which was
     * inserted into the transaction store when the read was requested) is then filled
     * with the data from the transaction storage.
     */
    virtual void finishRead(Thread*                requester,
                            FsReadStatus*          outStatus,
                            int64_t*               outResult,
                            FileDescriptorContent* fd);

    /**
     * The write implementation is very similar to read.
     */
    virtual FsTransactionID requestWrite(Thread*                    requester,
                                         FsNode*                    node,
                                         int64_t                    length,
                                         Contextual<const uint8_t*> buffer,
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
                                        const char*               filename,
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
