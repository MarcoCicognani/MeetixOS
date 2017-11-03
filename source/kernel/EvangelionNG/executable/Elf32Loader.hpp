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

#ifndef EVA_EXECUTABLE_ELF32_LOADER
#define EVA_EXECUTABLE_ELF32_LOADER

#include "eva/elf32.h"
#include <ramdisk/ramdisk.hpp>
#include <memory/paging.hpp>
#include <tasking/thread.hpp>

/**
 * Executable spawn status
 */
enum class Elf32SpawnStatus : uint8_t
{
	SUCCESSFUL,
	FILE_NOT_FOUND,
	VALIDATION_ERROR,
	PROCESS_CREATION_FAILED
};

/**
 * ELF validation status
 */
enum class Elf32ValidationStatus : uint8_t
{
	SUCCESSFUL,
	NOT_ELF,
	NOT_EXECUTABLE,
	NOT_I386,
	NOT_32BIT,
	NOT_LITTLE_ENDIAN,
	NOT_STANDARD_ELF
};

/**
 * Executable loader for 32bit ELF binaries
 */
class Elf32Loader
{
public:
	/**
	 * perform a spawning of executable stored on mounted ramdisk
	 *
	 * @param path:						the path of the executable on ramdisk
	 * @param securityLevel:			the security level to be assigned to the created process
	 * @param target:					main thread of the process
	 * @param enforceCurrentCore:		obbligate the kernel to use the current cpu core
	 * @param priority:					process priority
	 * @return one of the Elf32SpawnStatus code
	 */
	static Elf32SpawnStatus spawnFromRamdisk(const char *path, SecurityLevel securityLevel, Thread **target, bool enforceCurrentCore, ThreadPriority priority);

private:
	/**
	 * validate the elf32 header format
	 *
	 * @param header:		the elf header to validate
	 * @return one of the Elf32ValidationStatus code
	 */
	static Elf32ValidationStatus validate(Elf32Ehdr *header);

	/**
	 * load the binary to the current address space
	 *
	 * @param binaryHeader:		the header of the elf
	 * @param process:			target process
	 */
	static void loadBinaryToCurrentAddressSpace(Elf32Ehdr *binaryHeader, Process *process);

	/**
	 * load the thread local storage copy
	 *
	 * @param header:		the elf header
	 * @param process:		the target process
	 */
	static void loadTlsMasterCopy(Elf32Ehdr *header, Process *process);

	/**
	 * load the loadable elf segment
	 *
	 * @param header:		the elf header
	 * @param process:		the target process
	 */
	static void loadLoadSegment(Elf32Ehdr *header, Process *process);
};

#endif
