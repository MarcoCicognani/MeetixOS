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

#include "filesystem/FsTransactionStore.hpp"

#include "utils/HashMap.hpp"

static FsTransactionID                                nextTransactionID = 0;
static HashMap<FsTransactionID, FsTransactionStatus>* store;

/**
 *
 */
void FsTransactionStore::initialize() {
    store = new HashMap<FsTransactionID, FsTransactionStatus>();
}

/**
 *
 */
FsTransactionID FsTransactionStore::nextTransaction() {
    return nextTransactionID++;
}

/**
 *
 */
FsTransactionStatus FsTransactionStore::getStatus(FsTransactionID id) {
    auto entry = store->get(id);
    if ( entry )
        return entry->value;
    return FS_TRANSACTION_FINISHED;
}

/**
 *
 */
void FsTransactionStore::setStatus(FsTransactionID id, FsTransactionStatus result) {
    store->add(id, result);
}

/**
 *
 */
void FsTransactionStore::removeTransaction(FsTransactionID id) {
    store->erase(id);
}
