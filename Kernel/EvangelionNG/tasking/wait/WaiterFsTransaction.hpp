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

#ifndef EVA_MULTITASKING_WAIT_MANAGER_FS_TRANSACTION
#define EVA_MULTITASKING_WAIT_MANAGER_FS_TRANSACTION

#include "eva/utils/local.hpp"
#include "filesystem/FsTransactionHandler.hpp"
#include "logger/logger.hpp"
#include "tasking/wait/waiter.hpp"
#include "utils/string.hpp"

/**
 * Waiter implementation used for a specific transaction to be finished. Once the transaction is
 * finished, the given finish-handler is called (passing the task and the delegate) to do any
 * further action.
 */
class WaiterFsTransaction : public Waiter {
private:
    /**
     * internal data
     */
    FsTransactionHandler* handler;       // the transaction handler provided
    FsTransactionID       transactionID; // the id of the transaction
    FsDelegate*           delegate;      // the filesystem delegate

public:
    /**
     * filled constructor
     *
     * @param handler:			the handler to call when the transaction is finished.
     *							This handler will be deleted after the finish-transaction function was
     *called
     * @param transactionID:	the id of the transaction that we handle to
     * @param delegate:			the filesystem delegate
     */
    WaiterFsTransaction(FsTransactionHandler* handler,
                        FsTransactionID       transactionID,
                        FsDelegate*           delegate)
        : handler(handler), transactionID(transactionID), delegate(delegate) {
    }

    /**
     * implementation of check waiting method
     *
     * @param task:		the task that wait
     * @return true if task must keep waiting
     */
    virtual bool checkWaiting(Thread* task) {
        return isTransactionWaiting(task, handler, transactionID, delegate);
    }

    /**
     * Used to check what to do with the given transaction.
     *
     * @param task:				the task that wait
     * @param handler:			the transaction handler
     * @param transactionID:	the transaction id of the operation
     * @param delegate:			the delegate of the node where we operating
     * @return true if the task keep wait again
     */
    static bool isTransactionWaiting(Thread*               task,
                                     FsTransactionHandler* handler,
                                     FsTransactionID       transactionID,
                                     FsDelegate*           delegate) {
        FsTransactionStatus status = FsTransactionStore::getStatus(transactionID);

        // transaction waits for something, same transaction shall be repeated (could not be
        // fulfilled in the first attempt)
        if ( status == FS_TRANSACTION_WAITING )
            return true;
        else if ( status == FS_TRANSACTION_REPEAT ) {
            // tell the handler that we will repeat
            handler->prepareTransactionRepeat(transactionID);

            // repeat the transaction
            FsTransactionHandlerStartStatus restartStatus = handler->startTransaction(task);

            // transaction retry successful - let it wait again
            if ( restartStatus != FS_TRANSACTION_START_FAILED )
                return true;

            // could not repeat transaction start, set it finished so it repeats once more and exits
            FsTransactionStore::setStatus(status, FS_TRANSACTION_FINISHED);
            logInfo("%! problem: failed to repeat a transaction");
            return true;

            // the transaction has finished
        }

        else if ( status == FS_TRANSACTION_FINISHED ) {
            // let the delegate do finish operations
            FsTransactionHandlerFinishStatus stat = handler->finishTransaction(task, delegate);

            // remove the transaction information
            FsTransactionStore::removeTransaction(transactionID);

            /* the handler has requested another action from the delegate & a new waiter was created
               and set the transaction continues with a different handler, delete the old one */
            if ( stat == FS_TRANSACTION_HANDLING_REPEAT_WITH_SAME_HANDLER )
                return true;
            else if ( stat == FS_TRANSACTION_HANDLING_KEEP_WAITING_WITH_NEW_HANDLER ) {
                delete handler;
                return true;

                // the transaction is entirely finished and no more work must be done
            }

            else if ( stat == FS_TRANSACTION_HANDLING_DONE ) {
                delete handler;
                return false;
            }

            // the finish handler returned an unknown status. this is an error
            FsTransactionStore::removeTransaction(transactionID);
            logInfo("%! unknown finish handler status (%i) while waiting for transaction",
                    "filesystem",
                    stat);
            delete handler;
            return false;
        }

        // the transaction finished with an unknown status. this should never happen!
        FsTransactionStore::removeTransaction(transactionID);
        logInfo("%! unknown transaction status (%i) while waiting for transaction",
                "filesystem",
                status);
        delete handler;
        return false;
    }

    /**
     * @return the name of the waiter
     */
    virtual const char* debugName() {
        return "WaiterFsTransaction";
    }
};

#endif
