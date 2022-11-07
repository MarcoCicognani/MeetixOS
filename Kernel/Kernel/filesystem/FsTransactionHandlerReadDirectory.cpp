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

#include "filesystem/FsTransactionHandlerReadDirectory.hpp"

#include "filesystem/FsDelegate.hpp"
#include "logger/logger.hpp"

/**
 *
 */
FsTransactionHandlerStartStatus
FsTransactionHandlerReadDirectory::startTransaction(Thread* thread) {
    return FS_TRANSACTION_START_IMMEDIATE_FINISH;
}

/**
 *
 */
FsTransactionHandlerFinishStatus
FsTransactionHandlerReadDirectory::finishTransaction(Thread* thread, FsDelegate* delegate) {
    // check if it was called after a refresh, and the status of the refresh was good
    if ( causingHandler != nullptr && causingHandler->status != FS_DIRECTORY_REFRESH_SUCCESSFUL ) {
        data()->m_read_directory_status = FS_READ_DIRECTORY_ERROR;
        return FS_TRANSACTION_HANDLING_DONE;
    }

    // find node at position
    FsNode* item = nullptr;

    int position = data()->m_directory_iterator->m_position;

    int  iterpos = 0;
    auto iter    = folder->children;
    while ( iter ) {
        if ( iterpos == position ) {
            item = iter->value;
            break;
        }
        iter = iter->next;
        ++iterpos;
    }

    // EOD if none found
    if ( item == nullptr ) {
        data()->m_read_directory_status = FS_READ_DIRECTORY_EOD;
        return FS_TRANSACTION_HANDLING_DONE;
    }

    // copy data to output
    Memory::copy(data()->m_directory_iterator->m_entry_buffer.m_name,
                 item->name,
                 StringUtils::length(item->name) + 1);
    ++data()->m_directory_iterator->m_position;
    data()->m_directory_iterator->m_entry_buffer.m_node_id   = item->id;
    data()->m_directory_iterator->m_entry_buffer.m_node_type = item->type;
    data()->m_read_directory_status                          = FS_READ_DIRECTORY_SUCCESSFUL;
    return FS_TRANSACTION_HANDLING_DONE;
}
