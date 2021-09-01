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

#include <EvangelionNG.hpp>

#include <ramdisk/ramdisk.hpp>
#include <utils/string.hpp>
#include <logger/logger.hpp>

/**
 * Initializes the empty ramdisk. A ramdisk is never deleted, therefore
 * there is no destructor.
 */
Ramdisk::Ramdisk()
{
	root = 0;
	firstHeader = 0;
}

/**
 * Loads the ramdisk by creating actual {RamdiskEntry} objects. Reads the "module"
 * and interprets it as a ramdisk filesystem
 *
 * @param module:		the ramdisk multiboot module
 * @return the first ramdisk entry
 */
RamdiskEntry *Ramdisk::load(MultibootModule *module)
{
	// Get the ramdisk location and its end from the multiboot info
	uint8_t *ramdisk = (uint8_t*) module->moduleStart;
	uint32_t ramdiskEnd = module->moduleEnd;

	// Create a root RamdiskEntry
	root = new RamdiskEntry();
	root->id = 0;

	// Initialize the header storage location
	firstHeader = 0;

	// Create the position
	uint32_t ramdiskPosition = 0;

	// Iterate through the ramdisk
	RamdiskEntry *currentHeader = 0;
	while ((uint32_t) (ramdisk + ramdiskPosition) < ramdiskEnd)
	{
		RamdiskEntry *header = new RamdiskEntry();
		header->next = 0;

		if (!currentHeader)
		{
			firstHeader = header;
			currentHeader = firstHeader;
		}

		else
		{
			currentHeader->next = header;
			currentHeader = header;
		}

		// Type (file/folder)
		uint8_t *typeptr = (uint8_t*) (ramdisk + ramdiskPosition);
		header->type = static_cast<RamdiskEntryType>(*typeptr);
		ramdiskPosition++;

		// ID
		uint32_t *idptr = (uint32_t*) (ramdisk + ramdiskPosition);
		header->id = *idptr;
		ramdiskPosition += 4;

		// parent ID
		uint32_t *parentidptr = (uint32_t*) (ramdisk + ramdiskPosition);
		header->parentid = *parentidptr;
		ramdiskPosition += 4;

		// Name
		uint32_t *namelengthptr = (uint32_t*) (ramdisk + ramdiskPosition);
		uint32_t namelength = *namelengthptr;
		ramdiskPosition += 4;

		uint8_t *nameptr = (uint8_t*) (ramdisk + ramdiskPosition);
		header->name = new char[namelength + 1];
		Memory::copy(header->name, nameptr, namelength);
		header->name[namelength] = 0;
		ramdiskPosition = ramdiskPosition + namelength;

		// If its a file, load rest
		header->dataOnRamdisk = true;
		if (header->type == RAMDISK_ENTRY_TYPE_FILE)
		{
			// Data length
			uint32_t *datalengthptr = (uint32_t*) (ramdisk + ramdiskPosition);
			header->datalength = *datalengthptr;
			ramdiskPosition = ramdiskPosition + 4;

			// Copy data
			header->data = (uint8_t*) (ramdisk + ramdiskPosition);
			ramdiskPosition = ramdiskPosition + header->datalength;
		}

		else
		{
			header->datalength = 0;
			header->data = 0;
		}

		// start with unused ids after the last one
		if (header->id > nextUnusedId)
		{
			nextUnusedId = header->id + 1;
		}
	}

	return this->root;
}

/**
 * Searches in the folder parent for a file/folder with the given name
 *
 * @param parent:			the parent folder to search through
 * @param childName:		the name of the child to find
 * @return the entry if found, else 0
 */
RamdiskEntry *Ramdisk::findChild(RamdiskEntry *parent, const char *childName)
{
	RamdiskEntry *current = firstHeader;
	do
	{
		if (current->parentid == parent->id && String::equals(current->name, childName)) return current;
	}
	while ((current = current->next) != 0);

	return 0;
}

/**
 * Searches for the entry at the given absolute path
 *
 * @param name name of the entry to find
 * @return the entry if found, else 0
 */
RamdiskEntry *Ramdisk::findAbsolute(const char *path)
{
	return findRelative(root, path);
}

/**
 * Searches for a child with of "node" with the given relative path
 *
 * @param node:		the parent node
 * @param path:		the relative child path
 * @return the entry if found, else 0
 */
RamdiskEntry *Ramdisk::findRelative(RamdiskEntry *node, const char *path)
{
	char buf[RAMDISK_MAXIMUM_PATH_LENGTH];
	uint32_t pathLen = String::length(path);
	Memory::copy(buf, path, pathLen);
	buf[pathLen] = 0;

	RamdiskEntry *currentNode = node;
	while (String::length(buf) > 0)
	{
		int slashIndex = String::indexOf(buf, '/');
		if (slashIndex >= 0)
		{
			// Set current node to next layer
			if (slashIndex > 0)
			{
				char childpath[RAMDISK_MAXIMUM_PATH_LENGTH];
				Memory::copy(childpath, buf, slashIndex);
				childpath[slashIndex] = 0;

				currentNode = findChild(currentNode, childpath);
			}

			// Cut off layer
			uint32_t len = String::length(buf) - (slashIndex + 1);
			Memory::copy(buf, &buf[slashIndex + 1], len);
			buf[len] = 0;
		}

		else
		{
			// Reached the last node, find the child
			currentNode = findChild(currentNode, buf);
			break;
		}
	}
	return currentNode;
}

/**
 * Returns the entry with the given "id"
 *
 * @param name:		the name
 * @return the entry if it exists, else 0
 */
RamdiskEntry *Ramdisk::findById(uint32_t id)
{
	// return the root node
	if (!id) return root;

	RamdiskEntry *foundNode = 0;
	RamdiskEntry *currentNode = firstHeader;
	while (currentNode != 0)
	{
		if (currentNode->id == id)
		{
			foundNode = currentNode;
			break;
		}

		currentNode = currentNode->next;
	}

	return foundNode;
}

/**
 * Returns the number of children that the entry with "id" has
 *
 * @param id:		the entries id
 * @return the number of children
 */
uint32_t Ramdisk::getChildCount(uint32_t id)
{
	uint32_t count = 0;
	RamdiskEntry *currentNode = firstHeader;
	while (currentNode != 0)
	{
		if (currentNode->parentid == id) ++count;
		currentNode = currentNode->next;
	}

	return count;
}

/**
 * Returns the child at "index" of the node "id"
 *
 * @param id:			the parent entries id
 * @param index:		the index of the child
 * @return the entry if found, else 0
 */
RamdiskEntry *Ramdisk::getChildAt(uint32_t id, uint32_t index)
{
	uint32_t pos = 0;

	RamdiskEntry *currentNode = firstHeader;
	while (currentNode != 0)
	{
		if (currentNode->parentid == id)
		{
			if (pos == index) return currentNode;

			++pos;
		}
		currentNode = currentNode->next;
	}

	return 0;
}

/**
 * @return the root.
 */
RamdiskEntry *Ramdisk::getRoot() const
{
	return root;
}

/**
 * create a new ramdisk entry with name and linked with the father
 *
 * @param parent:		the parent node
 * @param filename:		name of the node
 * @return the new RamdiskEntry
 */
RamdiskEntry *Ramdisk::createChild(RamdiskEntry *parent, char *filename)
{
	RamdiskEntry *newNode = new RamdiskEntry();
	newNode->next = firstHeader;
	firstHeader = newNode;

	// copy name
	int namelen = String::length(filename);
	newNode->name = new char[namelen + 1];
	String::copy(newNode->name, filename);

	newNode->type = RAMDISK_ENTRY_TYPE_FILE;
	newNode->id = nextUnusedId++;
	newNode->parentid = parent->id;

	// set empty buffer
	newNode->data = nullptr;
	newNode->datalength = 0;
	newNode->notOnRdBufferLength = 0;
	newNode->dataOnRamdisk = false;

	return newNode;
}
