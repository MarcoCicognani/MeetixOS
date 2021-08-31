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

#ifndef EVA_SHARED_MULTIBOOT_MULTIBOOTUTIL
#define EVA_SHARED_MULTIBOOT_MULTIBOOTUTIL

#include <utils/HashMap.hpp>
#include <multiboot/multiboot.hpp>

/**
 * Util for GRUB multiboot related stuff
 */
class MultibootUtils
{
private:
	/**
	 * map to contains the parsed arguments
	 */
	static HashMap<const char*, const char*> *arguments;

public:
	/**
	 * Parse the multiboots arguments to fill the <arguments> map
	 *
	 * @param info:		the multiboot object provided by the bootloader
	 */
	static void parseArguments(MultibootInformation *info);

	/**
	 * Looks for the module with the given path.
	 *
	 * @param info:		the multiboot object provided by the bootloader
	 * @param path:		the path of the module
	 * @return the module with the provided path or nullptr
	 */
	static MultibootModule *findModule(MultibootInformation *info, const char *path);

};

#endif
