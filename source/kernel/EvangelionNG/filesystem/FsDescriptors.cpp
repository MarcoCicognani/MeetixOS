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

#include <filesystem/FsDescriptors.hpp>
#include <logger/logger.hpp>

static HashMap<Pid, FileDescriptorTable*> *processDescriptorTableMap;

/**
 *
 */
void FileDescriptors::initialize()
{
	processDescriptorTableMap = new HashMap<Pid, FileDescriptorTable*>();
}

/**
 *
 */
FileDescriptorContent *FileDescriptors::createDescriptor(FileDescriptorTable *table, File_t overrideFile_t)
{
	File_t descriptor;
	if (overrideFile_t == -1) descriptor = table->nextFileDescriptor++;

	else descriptor = overrideFile_t;

	FileDescriptorContent *desc = new FileDescriptorContent();
	desc->id = descriptor;
	desc->offset = 0;
	table->descriptors.add(descriptor, desc);

	return desc;
}

/**
 *
 */
File_t FileDescriptors::map(Pid pid, FsVirtID nodeID, File_t fd, int32_t openFlags)
{
	FileDescriptorTable *processTable = getProcessTable(pid);

	FileDescriptorContent *desc = createDescriptor(processTable, fd);
	desc->nodeID = nodeID;
	desc->openFlags = openFlags;

	return desc->id;
}

/**
 *
 */
void FileDescriptors::unmap(Pid pid, File_t fd)
{
	FileDescriptorTable *processTable = getProcessTable(pid);
	auto entry = processTable->descriptors.get(fd);
	if (entry)
	{
		auto content = entry->value;
		processTable->descriptors.erase(content->id);
		delete content;
	}
}

/**
 *
 */
void FileDescriptors::unmapAll(Pid pid)
{
	FileDescriptorTable *processTable = getProcessTable(pid);

	for (auto iter = processTable->descriptors.begin(); iter != processTable->descriptors.end(); ++iter)
	{
		FileDescriptorContent *content = iter->value;
		delete content;
	}

	processDescriptorTableMap->erase(pid);

	delete processTable;
}

/**
 *
 */
FileDescriptorTable *FileDescriptors::getProcessTable(Pid pid)
{
	auto entry = processDescriptorTableMap->get(pid);

	FileDescriptorTable *table = 0;
	if (entry == 0)
	{
		table = new FileDescriptorTable;
		processDescriptorTableMap->add(pid, table);
	}

	else table = entry->value;

	return table;
}

/**
 *
 */
FileDescriptorContent *FileDescriptors::get(Pid pid, File_t fd)
{
	FileDescriptorTable* processTable = getProcessTable(pid);
	auto entry = processTable->descriptors.get(fd);

	if (entry) return entry->value;

	return 0;
}
