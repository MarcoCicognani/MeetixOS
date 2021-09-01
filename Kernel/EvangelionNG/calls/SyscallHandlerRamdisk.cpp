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

#include <calls/SyscallHandler.hpp>

#include <EvangelionNG.hpp>
#include <ramdisk/ramdisk.hpp>
#include <logger/logger.hpp>

/**
 * Searches for a node on the ramdisk, using the absolute path of the node.
 */
SYSCALL_HANDLER(ramdiskFind)
{
	SyscallRamdiskFind *data = (SyscallRamdiskFind*) SYSCALL_DATA(currentThread->cpuState);

	RamdiskEntry *entry = EvaKernel::ramdisk->findAbsolute(data->path);
	if (entry) data->nodeID = entry->id;
	else data->nodeID = -1;

	return currentThread;
}

/**
 * Searches for a child with a specific name in the parent node with the given id.
 */
SYSCALL_HANDLER(ramdiskFindChild)
{
	SyscallRamdiskFindChild *data = (SyscallRamdiskFindChild*) SYSCALL_DATA(currentThread->cpuState);
	data->nodeID = -1;

	RamdiskEntry *parent = EvaKernel::ramdisk->findById(data->parentID);
	if (parent)
	{
		RamdiskEntry *entry = EvaKernel::ramdisk->findRelative(parent, data->childName);
		if (entry) data->nodeID = entry->id;
	}

	return currentThread;
}

/**
 * Returns information to a specific node on the ramdisk file system.
 */
SYSCALL_HANDLER(ramdiskInfo)
{
	SyscallRamdiskInfo *data = (SyscallRamdiskInfo*) SYSCALL_DATA(currentThread->cpuState);

	RamdiskEntry *entry = EvaKernel::ramdisk->findById(data->nodeID);
	if (entry)
	{
		data->length = entry->datalength;

		uint32_t nameLength = String::length(entry->name);
		Memory::copy(data->name, entry->name, nameLength);
		data->name[nameLength] = 0;

		data->type = entry->type;
	}

	else data->type = RAMDISK_ENTRY_TYPE_UNKNOWN;

	return currentThread;
}

/**
 * Reads from a ramdisk node.
 */
SYSCALL_HANDLER(ramdiskRead)
{
	SyscallRamdiskRead* data = (SyscallRamdiskRead*) SYSCALL_DATA(currentThread->cpuState);

	RamdiskEntry *entry = EvaKernel::ramdisk->findById(data->nodeID);
	if (entry && entry->type == RAMDISK_ENTRY_TYPE_FILE)
	{
		int bytesFromOffset = entry->datalength - ((int32_t) data->offset);
		if (bytesFromOffset < 0) bytesFromOffset = 0;

		int byteCount = (((long) bytesFromOffset) > (long) data->length) ? data->length : bytesFromOffset;
		if (byteCount > 0)
		{
			for (uint32_t i = 0; i < data->length; i++)
				data->buffer[i] = 0;

			Memory::copy(data->buffer, &entry->data[data->offset], byteCount);
		}

		data->readBytes = byteCount;
	}

	else data->readBytes = -1;

	return currentThread;
}

/**
 * Returns the number of children a ramdisk node has.
 */
SYSCALL_HANDLER(ramdiskChildCount)
{
	SyscallRamdiskChildCount *data = (SyscallRamdiskChildCount*) SYSCALL_DATA(currentThread->cpuState);

	data->count = EvaKernel::ramdisk->getChildCount(data->nodeID);

	return currentThread;
}

/**
 * Returns the id of the child of the parent node (with the given id) at the given index on the ramdisk.
 */
SYSCALL_HANDLER(ramdiskChildAt)
{
	SyscallRamdiskChildAt *data = (SyscallRamdiskChildAt*) SYSCALL_DATA(currentThread->cpuState);

	RamdiskEntry *entry = EvaKernel::ramdisk->getChildAt(data->nodeID, data->index);
	if (entry) data->childID = entry->id;
	else data->childID = -1;

	return currentThread;
}
