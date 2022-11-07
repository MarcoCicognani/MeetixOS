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

#include "filesystem/FsNode.hpp"

#include "utils/string.hpp"

/**
 *
 */
FsNode::FsNode()
    : delegate(0), type(FS_NODE_TYPE_NONE), id(0), physFsID(0), name(0), parent(0), children(0),
      isBlocking(true), contentsValid(false) {
}

/**
 *
 */
FsNode* FsNode::findChild(const char* name) {
    ListEntry<FsNode*>* n = children;
    while ( n ) {
        if ( n->value->name != 0 && StringUtils::equals(n->value->name, name) )
            return n->value;
        n = n->next;
    }

    return 0;
}

/**
 *
 */
void FsNode::setDelegate(FsDelegate* delegate) {
    this->delegate = delegate;
}

/**
 *
 */
FsDelegate* FsNode::getDelegate() {
    if ( delegate )
        return delegate;
    if ( parent )
        return parent->getDelegate();
    return 0;
}

/**
 *
 */
void FsNode::addChild(FsNode* child) {
    child->parent = this;

    ListEntry<FsNode*>* entry = new ListEntry<FsNode*>();
    entry->value              = child;

    entry->next = children;
    children    = entry;
}
