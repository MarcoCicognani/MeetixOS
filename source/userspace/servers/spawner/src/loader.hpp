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

#ifndef __LOADER__
#define __LOADER__

#include <eva.h>
#include <string>

// maximum number of pages to allocate & load at once
#define MAXIMUM_LOAD_PAGES_AT_ONCE		0x10

// loader status codes
enum LoaderStatus
{
	LS_SUCCESSFUL,
	LS_FORMAT_ERROR,
	LS_IO_ERROR,
	LS_MEMORY_ERROR,
	LS_UNKNOWN
};

/**
 *
 */
class Loader
{
protected:
	ProcessCreationIdentifier procIdent;
	File_t file;

public:
	Loader(ProcessCreationIdentifier target, File_t file) : procIdent(target), file(file) {}

	virtual ~Loader() {}

	virtual LoaderStatus load(uintptr_t *outEntryAddr) = 0;
};

#endif
