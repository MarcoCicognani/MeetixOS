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

#ifndef EVA_FILESYSTEM_FILESYSTEMTRANSACTIONSTORE
#define EVA_FILESYSTEM_FILESYSTEMTRANSACTIONSTORE

#include "filesystem/FsNode.hpp"
#include "memory/paging.hpp"

/**
 * Address-space bound meta object passed during transactions.
 */
struct FsTransactionStoreMeta
{
	void *value;
	PageDirectory space;
};

/**
 * The discovery store is used to store information about an ongoing discovery.
 * A discovery happens when a child for a specific node should be resolved by a driver
 * delegate which must happen asynchronously.
 */
class FsTransactionStore
{
public:
	static void initialize();
	static FsTransactionID nextTransaction();
	static void setStatus(FsTransactionID id, FsTransactionStatus result);
	static FsTransactionStatus getStatus(FsTransactionID id);
	static void removeTransaction(FsTransactionID id);
};

#endif
