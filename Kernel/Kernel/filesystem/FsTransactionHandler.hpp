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

#ifndef EVA_FILESYSTEM_TRANSACTION_HANDLER
#define EVA_FILESYSTEM_TRANSACTION_HANDLER

#include "Api/StdInt.h"
#include "tasking/thread.hpp"

class FsDelegate;

/**
 * Status for waiter
 */
typedef int                            FsTransactionHandlerFinishStatus;
const FsTransactionHandlerFinishStatus FS_TRANSACTION_HANDLING_DONE                          = 0;
const FsTransactionHandlerFinishStatus FS_TRANSACTION_HANDLING_REPEAT_WITH_SAME_HANDLER      = 1;
const FsTransactionHandlerFinishStatus FS_TRANSACTION_HANDLING_KEEP_WAITING_WITH_NEW_HANDLER = 2;

/**
 *
 */
typedef int                           FsTransactionHandlerStartStatus;
const FsTransactionHandlerStartStatus FS_TRANSACTION_START_FAILED           = 0;
const FsTransactionHandlerStartStatus FS_TRANSACTION_START_IMMEDIATE_FINISH = 1;
const FsTransactionHandlerStartStatus FS_TRANSACTION_START_WITH_WAITER      = 2;

/**
 * A transaction handler is a temporary object that accompanies a transaction. It holds temporary
 * values that the (stateless) delegate needs for its operations. A handler is normally passed in
 * two cases:
 *
 * - When something is requested from a delegate, the handler is passed so the delegate can
 *   immediately set the output values and call the finishing function if required
 *
 * - When a { WaiterFsTransaction } is notified that a transaction has finished, the delegate calls
 *   the finish function of the handler; the handler then calls the respective finish function of
 * the delegate, so the delegate can fill the information that was the result of the transaction
 * into the handler. Then the handler does anything that is further required, like filling a data
 * structure or calling another function (see { FsTransactionHandlerDiscovery } etc.)
 */
class FsTransactionHandler {
private:
    FsTransactionID repeatTransaction = FS_TRANSACTION_NO_REPEAT_ID;

public:
    virtual ~FsTransactionHandler() {
    }

    /**
     *
     */
    virtual FsTransactionHandlerStartStatus startTransaction(Thread* thread) {
        return FS_TRANSACTION_START_IMMEDIATE_FINISH;
    }

    /**
     *
     */
    virtual FsTransactionHandlerFinishStatus finishTransaction(Thread* thread, FsDelegate* delegate)
        = 0;

    /**
     *
     */
    bool wantsRepeatTransaction() {
        return repeatTransaction != FS_TRANSACTION_NO_REPEAT_ID;
    }

    /**
     *
     */
    void prepareTransactionRepeat(FsTransactionID tr) {
        this->repeatTransaction = tr;
    }

    /**
     *
     */
    FsTransactionID getRepeatedTransaction() {
        return this->repeatTransaction;
    }
};

#endif
