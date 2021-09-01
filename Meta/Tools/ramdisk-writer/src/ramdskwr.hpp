/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *  Ghost, a micro-kernel based operating system for the x86 architecture    *
 *  Copyright (C) 2015, Max Schlüssel <lokoxe@gmail.com>                     *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _RAMDISK_WRITER_
#define _RAMDISK_WRITER_

#include <fstream>
#include <stdint.h>
#include <string>
#include <list>

#define VERSION_MAJOR	1
#define	VERSION_MINOR	1

/**
 * class to create formatted ramdisk readable by ramdisk driver
 */
class RamdiskWriter
{
private:
	/**
	 * internal infos
	 */
	int idCounter;                  // the current file id
	std::ofstream out;              // the output file
	std::list<std::string> ignores; // the list of ignorable files

	/**
	 * write each file on a output file
	 *
	 * @param basePath:		 the base path of a file
	 * @param path:			 the absolute path of the file
	 * @param name:			 the filename
	 * @param contentLength: the content length in bytes of the file
	 * @param parentId:		 the id of the parent
	 * @param isFile:		 flag to check if current entry is a file or a directory
	 */
	void writeRecursive(const std::string &basePath, const std::string &path, const std::string &name, uint32_t contentLength, uint32_t parentId, bool isFile);

public:
	/**
	 * standard constructor
	 */
	RamdiskWriter() : idCounter(0) {}

	/**
	 * create a ramdisk file from a source path
	 *
	 * @param sourcePath:		the path where the tool take entryes to create the output file
	 * @param targetPath:		the path where the tool write the output file
	 */
	void create(const std::string &sourcePath, const std::string &targetPath);
};

#endif
