/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM * This software is derived
 * from the Ghost operating system project,           * written by Max Schlüssel
 * <lokoxe@gmail.com>. Copyright 2012-2017            * https://ghostkernel.org/
 * * https://github.com/maxdev1/ghost *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#include "loader.hpp"

#include "kernelloader/KernelLoader.hpp"
#include "memory/gdt/GdtManager.hpp"
#include "memory/MultibootMmapInterpreter.hpp"
#include "memory/PagingInitializer.hpp"
#include "multiboot/MultibootModuleAnalyzer.hpp"

#include <logger/logger.hpp>
#include <memory/bitmap/BitmapPageAllocator.hpp>
#include <memory/memory.hpp>
#include <multiboot/MultibootUtil.hpp>
#include <stdarg.h>
#include <video/ConsoleVideo.hpp>
#include <video/PrettyBoot.hpp>

/**
 * Linker symbols defined in the linker script
 */
extern "C" {
    void* endAddress;
}

/**
 * Setup information struct containing important information that the
 * kernel needs for further setup.
 */
static SetupInformation setupInformation;

/**
 * physical allocator instance,
 * used to collect indexes of free memory chucks
 */
static BitMapPageAllocator physicalAllocator;

/**
 * Initializes the system by preparing the necessities to enable virtual memory
 * and then loads the kernel module to the higher half and jumps to it.
 *
 * @param multibootInformation:		the GRUB multiboot information
 */
void EvaLoader::initialize(MultibootInformation* multibootInformation) {
    // Store multiboot structure
    setupInformation.multibootInformation = multibootInformation;

    // Begin initialization
    logInfo("%! loader initializing", "Eva Loader");

    // End of the loader binary in memory
    uint32_t loaderEndAddress = PAGE_ALIGN_UP((uint32_t)&endAddress);

    // Find free spaces to place the GDT and the bitmap
    PRETTY_BOOT_STATUS("Initiliazing memory", 3, RED);
    uint32_t gdtAreaStart = findFreeMemory(multibootInformation, loaderEndAddress, 1);
    uint32_t gdtAreaEnd   = gdtAreaStart + PAGE_SIZE;

    uint32_t bitmapStart
        = findFreeMemory(multibootInformation, gdtAreaEnd, PAGE_ALIGN_UP(BITMAP_SIZE) / PAGE_SIZE);
    uint32_t bitmapEnd = PAGE_ALIGN_UP(bitmapStart + BITMAP_SIZE);

    // The "reservedAreaEnd" is the end of the memory (somewhere above 1MiB)
    // that is not occupied by the loader binary or the pages that we split
    // of for use as bitmap and GDT.
    uint32_t reservedAreaEnd = bitmapEnd;

#if LOGGING_DEBUG
    // Information output
    logDebug("%! available modules:", "mmodule");
    for ( uint32_t i = 0; i < multibootInformation->modulesCount; i++ ) {
        MultibootModule* module = (MultibootModule*)(multibootInformation->modulesAddress
                                                     + sizeof(MultibootModule) * i);
        logDebug("%#   '%s' at %h - %h", module->path, module->moduleStart, module->moduleEnd);
    }

    logDebug("%! calculated addresses:", "Eva Loader");
    logDebug("%#   gdt area:            %h", gdtAreaStart);
    logDebug("%#   bitmap:              %h", bitmapStart);
    logDebug("%#   reserved area end:   %h", reservedAreaEnd);
#endif

    // Store setup information
    setupInformation.bitmapStart = bitmapStart;
    setupInformation.bitmapEnd   = bitmapEnd;

    // Set up the GDT. Here we pass the address of the gdt area, which contains
    // enough space to create the descriptor table and its pointer.
    GdtManager::initialize(gdtAreaStart);

    // Read GRUB map to add free pages to the allocator
    physicalAllocator.initialize((BitMapEntry*)bitmapStart);
    MultibootMmapInterpreter::load(&physicalAllocator, reservedAreaEnd);

    // Set up paging, this relocates the multiboot modules
    PagingInitializer::initialize(reservedAreaEnd, &setupInformation);
    // IMPORTANT: Now the multiboot module location has changed!

    // Load kernel binary
    PRETTY_BOOT_STATUS("locating Evangelion kernel", 3, GREEN);
    logInfo("%! locating Evangelion Kernel...", "Eva Loader");
    MultibootModule* kernelModule
        = MultibootUtils::findModule(setupInformation.multibootInformation, "/boot/Kernel");
    if ( kernelModule ) {
        PRETTY_BOOT_STATUS("Loading EvangelionNG", 5, GREEN);
        logInfo("%! found EvangelionNG binary at %h, loading...",
                "Eva Loader",
                kernelModule->moduleStart);

        KernelLoader::load(kernelModule);
        EvaLoader::panic("%! something went wrong during boot process, halting", "Eva Loader");
    }

    else {
        PRETTY_BOOT_FAIL("EvangelionNG not found");
        EvaLoader::panic("%! EvangelionNG not found", "Eva Loader");
    }
}

/**
 * This function is used to find a chunk of free memory even before any memory
 * management was initialized. It is used to find a place to put the GDT and the
 * bitmap, to avoid accidentally overwriting multiboot modules.
 *
 * @param info:		the multiboot infos
 * @param start:	the start index
 * @param count:	number of indexes
 */
uint32_t EvaLoader::findFreeMemory(MultibootInformation* info, uint32_t start, int count) {
    logInfo("%! searching for %i free pages (starting at %h)", "Eva Loader", count, start);

    // parsing each physical addres from start to 32bit max range
    PhysicalAddress location = start;
    while ( location < 0xFFFFFFFF ) {
        bool notWithinModule = true;

        // For each of the required pages, check if it is within a module
        for ( int i = 0; i < count; i++ ) {
            uint32_t pos = location + i * PAGE_SIZE;

            // Check one of the modules contains this position
            for ( uint32_t i = 0; i < info->modulesCount; i++ ) {
                // check if module exist in this position
                MultibootModule* module
                    = (MultibootModule*)(info->modulesAddress + sizeof(MultibootModule) * i);
                uint32_t moduleStart = PAGE_ALIGN_DOWN(module->moduleStart);
                uint32_t moduleEnd   = PAGE_ALIGN_UP(module->moduleEnd);

                // no modules in this address
                if ( pos >= moduleStart && pos < moduleEnd ) {
                    notWithinModule = false;
                    location        = moduleEnd;
                    break;
                }
            }
        }

        // a module is found
        if ( notWithinModule ) {
            logInfo("%# found: %h", location);
            return location;
        }

        // next address
        location = location + PAGE_SIZE;
    }

    panic("%! could not find free memory chunk", "Eva Loader");
    return 0;
}

/**
 * Triggers a loader panic, means halting the entire system and displaying the
 * given message/diagnostic information.
 *
 * @param message:		the panic message, written in a format that the
 * g_logger class understands
 * @param ...:			variable arguments for the message
 */
void EvaLoader::panic(const char* msg, ...) {
    // print panic header
    logInfo("%! an unrecoverable error has occured. reason:", "EvaLoader Panic");

    // print other function parameters
    va_list valist;
    va_start(valist, msg);
    Logger::printFormatted(msg, valist);
    va_end(valist);
    Logger::printCharacter('\n');

    // halt the system
    asm("cli");
    for ( ;; )
        asm("hlt");
}

/**
 * @return the loaders setup information struct
 */
SetupInformation* EvaLoader::getSetupInformation() {
    return &setupInformation;
}

/**
 * @return the physical allocator pointer
 */
BitMapPageAllocator* EvaLoader::getPhysicalAllocator() {
    return &physicalAllocator;
}
