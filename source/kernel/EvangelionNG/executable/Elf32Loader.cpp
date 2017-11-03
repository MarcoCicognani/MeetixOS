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

#include <executable/Elf32Loader.hpp>

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <ramdisk/ramdisk.hpp>
#include <tasking/tasking.hpp>
#include <tasking/ThreadManager.hpp>
#include <memory/physical/PPallocator.hpp>
#include <memory/physical/PPreferenceTracker.hpp>
#include <memory/paging.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/memory.hpp>
#include <memory/TemporaryPagingUtil.hpp>
#include <memory/constants.hpp>
#include <utils/string.hpp>

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
Elf32SpawnStatus Elf32Loader::spawnFromRamdisk(const char *path, SecurityLevel securityLevel, Thread **target, bool enforceCurrentCore, ThreadPriority priority)
{
	// get file entry
	RamdiskEntry *entry = EvaKernel::ramdisk->findAbsolute(path);

	// Check file
	if (!entry || entry->type != RAMDISK_ENTRY_TYPE_FILE) return Elf32SpawnStatus::FILE_NOT_FOUND;

	// Get and validate ELF header
	Elf32Ehdr *header = (Elf32Ehdr*) entry->data;
	Elf32ValidationStatus status = validate(header);

	if (status == Elf32ValidationStatus::SUCCESSFUL)
	{
		// Create the process
		Thread *mainThread = ThreadManager::createProcess(securityLevel, nullptr);
		if (!mainThread)
		{
			logWarn("%! failed to create main thread to spawn ELF binary from ramdisk", "elf32");
			return Elf32SpawnStatus::PROCESS_CREATION_FAILED;
		}

		// get process object
		Process *process = mainThread->process;

		// set path
		process->setPath(path);

		// Temporarily switch to the new processes page directory to load the binary to it
		PageDirectory thisPageDirectory = AddressSpace::getCurrentSpace();
		AddressSpace::switchToSpace(process->pageDirectory);
		loadBinaryToCurrentAddressSpace(header, process);
		ThreadManager::prepareThreadLocalStorage(mainThread);
		AddressSpace::switchToSpace(thisPageDirectory);

		// Set the tasks entry point
		mainThread->cpuState->eip = header->e_entry;

		// Set priority
		mainThread->priority = priority;

		// Add to scheduling list
		Tasking::addTask(mainThread, enforceCurrentCore);

		// Set out parameter
		*target = mainThread;
		return Elf32SpawnStatus::SUCCESSFUL;
	}

	return Elf32SpawnStatus::VALIDATION_ERROR;
}

/**
 * validate the elf32 header format
 *
 * @param header:		the elf header to validate
 * @return one of the Elf32ValidationStatus code
 */
Elf32ValidationStatus Elf32Loader::validate(Elf32Ehdr *header)
{
	// Valid ELF header
	if (/**/(header->e_ident[EI_MAG0] != ELFMAG0) || // 0x7F
			(header->e_ident[EI_MAG1] != ELFMAG1) || // E
			(header->e_ident[EI_MAG2] != ELFMAG2) || // L
			(header->e_ident[EI_MAG3] != ELFMAG3)){  // F
		return Elf32ValidationStatus::NOT_ELF;
	}

	// Must be executable
	if (header->e_type != ET_EXEC) return Elf32ValidationStatus::NOT_EXECUTABLE;

	// Must be i386 architecture compatible
	if (header->e_machine != EM_386) return Elf32ValidationStatus::NOT_I386;

	// Must be 32 bit
	if (header->e_ident[EI_CLASS] != ELFCLASS32) return Elf32ValidationStatus::NOT_32BIT;

	// Must be little endian
	if (header->e_ident[EI_DATA] != ELFDATA2LSB) return Elf32ValidationStatus::NOT_LITTLE_ENDIAN;

	// Must comply to current ELF standard
	if (header->e_version != EV_CURRENT) return Elf32ValidationStatus::NOT_STANDARD_ELF;

	// All fine
	return Elf32ValidationStatus::SUCCESSFUL;
}

/**
 * This method loads the binary with the given header to the current address space. Once finished,
 * the start and end address of the executable image in memory are written to the respective out parameters.
 */
void Elf32Loader::loadBinaryToCurrentAddressSpace(Elf32Ehdr *header, Process *process)
{
	loadLoadSegment(header, process);
	loadTlsMasterCopy(header, process);
}

/**
 * load the thread local storage copy
 *
 * @param header:		the elf header
 * @param process:		the target process
 */
void Elf32Loader::loadTlsMasterCopy(Elf32Ehdr *header, Process *process)
{
	uint32_t tlsSize = 0;

	// Map pages for TLS master copy
	for (uint32_t i = 0; i < header->e_phnum; i++)
	{
		Elf32Phdr *programHeader = (Elf32Phdr*) (((uint32_t) header) + header->e_phoff + (header->e_phentsize * i));
		if (programHeader->p_type == PT_TLS)
		{
			tlsSize = PAGE_ALIGN_UP(programHeader->p_memsz);

			uint32_t tlsPages = tlsSize / PAGE_SIZE;
			uint32_t tlsStart = process->virtualRanges.allocate(tlsPages);
			uint32_t tlsEnd = tlsStart + tlsSize;

			for (uint32_t virt = tlsStart; virt < tlsEnd; virt = virt + PAGE_SIZE)
			{
				uint32_t phys = PPallocator::allocate();
				AddressSpace::map(virt, phys, DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);
				PPreferenceTracker::increment(phys);
			}

			Memory::setBytes((void*) tlsStart, 0x0, programHeader->p_memsz);
			Memory::copy((void*) tlsStart, (uint8_t*) (((uint32_t) header) + programHeader->p_offset), programHeader->p_filesz);
			break;
		}
	}
}

/**
 * load the loadable elf segment
 *
 * @param header:		the elf header
 * @param process:		the target process
 */
void Elf32Loader::loadLoadSegment(Elf32Ehdr *header, Process *process)
{
	// Initial values
	uint32_t imageStart = 0xFFFFFFFF;
	uint32_t imageEnd = 0;

	// First find out how much place the image needs in memory
	for (uint32_t i = 0; i < header->e_phnum; i++)
	{
		Elf32Phdr *programHeader = (Elf32Phdr*) (((uint32_t) header) + header->e_phoff + (header->e_phentsize * i));

		if (programHeader->p_type != PT_LOAD) continue;
		if (programHeader->p_vaddr < imageStart) imageStart = programHeader->p_vaddr;
		if (programHeader->p_vaddr + programHeader->p_memsz > imageEnd) imageEnd = programHeader->p_vaddr + programHeader->p_memsz;
	}

	// Align the addresses
	imageStart = PAGE_ALIGN_DOWN(imageStart);
	imageEnd = PAGE_ALIGN_UP(imageEnd);

	// Map pages for the executable
	for (uint32_t virt = imageStart; virt < imageEnd; virt = virt + PAGE_SIZE)
	{
		uint32_t phys = PPallocator::allocate();
		AddressSpace::map(virt, phys, DEFAULT_USER_TABLE_FLAGS, DEFAULT_USER_PAGE_FLAGS);
		PPreferenceTracker::increment(phys);
	}

	// Write the image to memory
	for (uint32_t i = 0; i < header->e_phnum; i++)
	{
		Elf32Phdr *programHeader = (Elf32Phdr*) (((uint32_t) header) + header->e_phoff + (header->e_phentsize * i));
		if (programHeader->p_type != PT_LOAD)
			continue;
		Memory::setBytes((void*) programHeader->p_vaddr, 0x0, programHeader->p_memsz);
		Memory::copy((void*) programHeader->p_vaddr, (uint8_t*) (((uint32_t) header) + programHeader->p_offset), programHeader->p_filesz);
	}

	// Set out parameters
	process->imageStart = imageStart;
	process->imageEnd = imageEnd;
}
