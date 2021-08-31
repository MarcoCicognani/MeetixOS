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

#ifndef EVA_LOADER_KERNEL_LOADER_
#define EVA_LOADER_KERNEL_LOADER_

#include "eva/elf32.h"
#include <multiboot/multiboot.hpp>
#include <kernelloader/SetupInformation.hpp>

/**
 * The initial kernel heap is sized as 16MB. For example, the stacked
 * physical allocator needs this amount to convert its bitmap to stack pages.
 */
#define KERNEL_HEAP_SIZE	0x1000000

/**
 * The kernel loader class loads the given module as an ELF executable.
 */
class KernelLoader
{
public:

	/**
	 * Loads the given module as the kernel.
	 *
	 * @param kernelModule:		the multiboot module containing the kernel
	 */
	static void load(MultibootModule *kernelModule);

private:

	/**
	 * Checks the ELF32 header for validity.
	 *
	 * @param header:		the header to check
	 */
	static void checkHeader(Elf32Ehdr *header);

	/**
	 * Loads the kernel binary starting at the given ELF header, writing the binary location
	 * in memory to the setupInformation structure.
	 *
	 * @param header:				the ELF header
	 * @param setupInformation:		the setup information where the load information should be stored
	 */
	static void loadBinary(Elf32Ehdr *header, SetupInformation *setupInformation);
};

#endif
