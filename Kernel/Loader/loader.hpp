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

#ifndef EVA_LOADER_LOADER
#define EVA_LOADER_LOADER

#include "eva/stdint.h"

#include <kernelloader/SetupInformation.hpp>
#include <memory/bitmap/BitmapPageAllocator.hpp>
#include <memory/memory.hpp>
#include <multiboot/multiboot.hpp>

/**
 * The loader class is responsible for preparing the virtual memory management
 * and then loading the kernel to higher memory.
 */
class EvaLoader {
public:
    /**
     * Initializes the system by preparing the necessities to enable virtual memory and
     * then loads the kernel module to the higher half and jumps to it.
     *
     * @param multibootInformation:		the GRUB multiboot information
     */
    static void initialize(MultibootInformation* multibootInformation);

    /**
     * This function is used to find a chunk of free memory even before any memory
     * management was initialized. It is used to find a place to put the GDT and the
     * bitmap, to avoid accidentally overwriting multiboot modules.
     *
     * @param info:		the multiboot infos
     * @param start:	the start index
     * @param count:	number of indexes
     */
    static uint32_t findFreeMemory(MultibootInformation* info, uint32_t start, int count);

    /**
     * Triggers a loader panic, means halting the entire system and displaying the given
     * message/diagnostic information.
     *
     * @param message:		the panic message, written in a format that the g_logger class
     * understands
     * @param ...:			variable arguments for the message
     */
    static void panic(const char* msg, ...);

    /**
     * @return the loaders setup information struct
     */
    static SetupInformation* getSetupInformation();

    /**
     * @return the physical allocator pointer
     */
    static BitMapPageAllocator* getPhysicalAllocator();
};

#endif
