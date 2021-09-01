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

#ifndef EVA_FILESYSTEM_FILESYSTEM_NODE
#define EVA_FILESYSTEM_FILESYSTEM_NODE

#include "eva/stdint.h"
#include "eva/fs.h"
#include "utils/ListEntry.hpp"

class FsDelegate;

/**
 *
 */
class FsNode
{
private:
	FsDelegate *delegate;

public:
	FsNode();

	void setDelegate(FsDelegate *delegate);
	FsDelegate *getDelegate();

	FsNodeType type;
	FsVirtID id;
	FsPhysID physFsID;

	char *name;
	FsNode *parent;
	ListEntry<FsNode*> *children;
	void addChild(FsNode *child);

	bool isBlocking;

	/**
	 * True if the node is a directory and all child
	 * nodes are up-to-date.
	 */
	bool contentsValid;

	FsNode *findChild(char *name);
};

#endif
