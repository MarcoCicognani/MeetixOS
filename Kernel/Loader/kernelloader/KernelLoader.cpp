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

#include <kernelloader/KernelLoader.hpp>
#include <loader.hpp>
#include <logger/logger.hpp>
#include <memory/bitmap/BitmapPageAllocator.hpp>
#include <memory/memory.hpp>
#include <memory/paging.hpp>
#include <memory/PagingInitializer.hpp>
#include <video/PrettyBoot.hpp>

/**
 * Loads the given module as the kernel.
 *
 * @param kernelModule:		the multiboot module containing the kernel
 */
void KernelLoader::load(MultibootModule* kernelModule) {
    SetupInformation* setupInformation = EvaLoader::getSetupInformation();

    // ELF header
    Elf32Ehdr* elfHeader = (Elf32Ehdr*)kernelModule->moduleStart;
    checkHeader(elfHeader);

    // Load the binary
    loadBinary(elfHeader, setupInformation);

    /**
     * Create all page tables for kernel area to initial directory. This is done to ensure
     * that every copy of the kernel space contains all later kernel pages.
     */
    uint32_t* directory = (uint32_t*)0xFFFFF000;
    for ( uint32_t ti = TABLE_IN_DIRECTORY_INDEX(setupInformation->kernelImageStart); ti < 1024;
          ti++ ) {
        // Create the table only if it doesn't exist yet
        if ( !directory[ti] ) {
            uint32_t tableChunkAddress = (uint32_t)EvaLoader::getPhysicalAllocator()->allocate();
            directory[ti]              = tableChunkAddress | DEFAULT_KERNEL_TABLE_FLAGS;

            // Zero the new table
            uint32_t* table = ((uint32_t*)0xFFC00000) + (0x400 * ti);
            for ( uint32_t i = 0; i < 1024; i++ )
                table[i] = 0;
        }
    }

    // Create new stack
    setupInformation->stackStart = setupInformation->kernelImageEnd;
    setupInformation->stackEnd   = setupInformation->stackStart + PAGE_SIZE;
    uint32_t stackPointer        = setupInformation->stackEnd - 4;
    uint32_t stackPhys           = EvaLoader::getPhysicalAllocator()->allocate();
    if ( !stackPhys ) {
        PRETTY_BOOT_FAIL("Failed to allocate kernel stack");
        EvaLoader::panic("%! out of pages when trying to create kernel stack", "kernload");
    }
    PagingInitializer::mapPageToRecursiveDirectory(setupInformation->stackStart,
                                                   stackPhys,
                                                   DEFAULT_KERNEL_TABLE_FLAGS,
                                                   DEFAULT_KERNEL_PAGE_FLAGS);

    // Create kernel heap
    uint32_t heapStart = setupInformation->stackEnd;
    uint32_t heapEnd   = heapStart + KERNEL_HEAP_SIZE;
    for ( uint32_t virt = heapStart; virt < heapEnd; virt += PAGE_SIZE ) {
        uint32_t phys = EvaLoader::getPhysicalAllocator()->allocate();

        if ( !phys ) {
            PRETTY_BOOT_FAIL("Failed to allocate kernel heap");
            EvaLoader::panic("%! out of pages, allocated to %h", "kernload", virt);
        }

        PagingInitializer::mapPageToRecursiveDirectory(virt,
                                                       phys,
                                                       DEFAULT_KERNEL_TABLE_FLAGS,
                                                       DEFAULT_KERNEL_PAGE_FLAGS);
    }
    setupInformation->heapStart = heapStart;
    setupInformation->heapEnd   = heapEnd;

    // Push information and enter kernel
    uint32_t setupInformationAddress = (uint32_t)EvaLoader::getSetupInformation();
    uint32_t entryAddress            = elfHeader->e_entry;

    PRETTY_BOOT_STATUS("Starting kernel", 10, GREEN);
    logInfo("%! jumping to %h...", "kernload", entryAddress);
    asm("mov %0, %%esp\n"
        "mov %%esp, %%ebp\n"
        "push %1\n"
        "call *%2" ::"r"(stackPointer),
        "r"(setupInformationAddress),
        "r"(entryAddress));
    // Will never reach here, so no need to clean up the stack.
}

/**
 * Checks the ELF32 header for validity.
 *
 * @param header:		the header to check
 */
void KernelLoader::checkHeader(Elf32Ehdr* header) {
    if ( !(header->e_ident[EI_MAG0] == ELFMAG0 && header->e_ident[EI_MAG1] == ELFMAG1
           && header->e_ident[EI_MAG2] == ELFMAG2 && header->e_ident[EI_MAG3] == ELFMAG3) )
        EvaLoader::panic("%! binary is not ELF", "kernload");
    if ( header->e_type != ET_EXEC )
        EvaLoader::panic("%! binary is not executable", "kernload");
    if ( header->e_machine != EM_386 )
        EvaLoader::panic("%! binary target architecture not i386", "kernload");
    if ( header->e_ident[EI_CLASS] != ELFCLASS32 )
        EvaLoader::panic("%! binary is not 32bit", "kernload");
    if ( header->e_ident[EI_DATA] != ELFDATA2LSB )
        EvaLoader::panic("%! binary does not have little endian data encoding", "kernload");
    if ( header->e_version != EV_CURRENT )
        EvaLoader::panic("%! binary is not standard ELF", "kernload");

    logDebug("%! binary ELF validation successful", "kernload");
}

/**
 * Loads the kernel binary starting at the given ELF header, writing the binary location
 * in memory to the setupInformation structure.
 *
 * @param header:				the ELF header
 * @param setupInformation:		the setup information where the load information should be
 * stored
 */
void KernelLoader::loadBinary(Elf32Ehdr* header, SetupInformation* setupInformation) {
    logDebug("%! loading binary to higher memory", "kernload");
    uint32_t imageStart = -1;
    uint32_t imageEnd   = 0;

    // Calculate image start and end
    for ( uint32_t i = 0; i < header->e_phnum; i++ ) {
        Elf32Phdr* programHeader
            = (Elf32Phdr*)(((uint32_t)header) + header->e_phoff + (header->e_phentsize * i));
        if ( programHeader->p_type != PT_LOAD )
            continue;
        if ( programHeader->p_vaddr < imageStart )
            imageStart = programHeader->p_vaddr;
        if ( programHeader->p_vaddr + programHeader->p_memsz > imageEnd )
            imageEnd = programHeader->p_vaddr + programHeader->p_memsz;
    }

    imageStart = PAGE_ALIGN_DOWN(imageStart);
    imageEnd   = PAGE_ALIGN_UP(imageEnd);
    logDebug("%! image spans from %h to %h", "kernload", imageStart, imageEnd);

    // Map pages
    for ( uint32_t virt = imageStart; virt < imageEnd; virt = virt + PAGE_SIZE ) {
        uint32_t phys = EvaLoader::getPhysicalAllocator()->allocate();
        PagingInitializer::mapPageToRecursiveDirectory(virt,
                                                       phys,
                                                       DEFAULT_KERNEL_TABLE_FLAGS,
                                                       DEFAULT_KERNEL_PAGE_FLAGS);
    }

    // Copy image
    for ( uint32_t i = 0; i < header->e_phnum; i++ ) {
        // get the header of the elf
        Elf32Phdr* programHeader
            = (Elf32Phdr*)(((uint32_t)header) + header->e_phoff + (header->e_phentsize * i));

        // always check if is a loadable part
        if ( programHeader->p_type != PT_LOAD )
            continue;

        // Clear
        Memory::setBytes((void*)programHeader->p_vaddr, 0, programHeader->p_memsz);

        // Copy contents
        Memory::copy((void*)programHeader->p_vaddr,
                     (uint8_t*)(((uint32_t)header) + programHeader->p_offset),
                     programHeader->p_filesz);
    }

    logDebug("%! kernel image loaded to space %h - %h", "kernload", imageStart, imageEnd);
    setupInformation->kernelImageStart = imageStart;
    setupInformation->kernelImageEnd   = imageEnd;
}
