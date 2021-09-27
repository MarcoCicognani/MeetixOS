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

#ifndef EVA_LOADER_MEMORY_PAGINGINITIALIZER
#define EVA_LOADER_MEMORY_PAGINGINITIALIZER

#include "Api/StdInt.h"

#include <kernelloader/SetupInformation.hpp>
#include <memory/paging.hpp>

/**
 * Setup class for paging initialization
 */
class PagingInitializer {
public:
    /**
     * Initially creates the paging directory and identity maps the lower 1MB plus the area of the
     * kernel (end of this is delimited by the parameter given to initialize). Identity-maps the
     * memory from 0x00000000 to reservedAreaEnd.
     *
     * @param reservedAreaEnd:		end of the reserved kernel area
     * @param setupInformation:		setup information struct use to store physical page
     * directory location
     */
    static void initialize(uint32_t reservedAreaEnd, SetupInformation* setupInformation);

    /**
     * Maps the page at physicalAddress to the virtualAddress, using the given tableFlags and
     * pageFlags. Works only once paging is enabled.
     *
     * @param virtualAddress:		the targeted virtual address in the adress space
     * @param physicalAddress:		the physical address of the page
     * @param tableFlags:			the table flags to apply
     * @param pageFlags:			the page flags to apply
     * @return whether the mapping was successful
     */
    static bool mapPageToRecursiveDirectory(uint32_t virtualAddress,
                                            uint32_t physicalAddress,
                                            uint32_t tableFlags,
                                            uint32_t pageFlags);

    /**
     * Sets the "page global enabled" in the CR4 to enable the use of global pages for the kernel
     * space.
     */
    static void enableGlobalPageFlag();

private:
    /**
     * Enables paging.
     */
    static void enable();

    /**
     * Relocates the multiboot modules. They can be loaded to a high address, but we want
     * them to appear right behind 1MB, so we remap them here and write their new addresses
     * into the multiboot structures.
     *
     * @param pageDirectory:		the new page directory where map modules
     * @param reservedAreaEnd:		the end address of the reserved area
     */
    static void relocateMultibootModules(PageDirectory pageDirectory, uint32_t reservedAreaEnd);

    /**
     * Switches to the given directory.
     *
     * @param directory		the directory to switch to
     */
    static void switchToSpace(PageDirectory directory);

    /**
     * Identity-maps the area from start to end to the page directory directory, using the given
     * tableFlags and pageFlags. This only works while paging is disable, thus it is private for the
     * paging initializer.
     *
     * @param directory:		the targeted directory
     * @param start:			start of the area to map
     * @param end:				end of the area to map
     * @param tableFlags:		the table flags to apply
     * @param pageFlags:		the page flags to apply
     */
    static void identityMap(uint32_t* directory,
                            uint32_t  start,
                            uint32_t  end,
                            uint32_t  tableFlags,
                            uint32_t  pageFlags);

    /**
     * Maps the page at physicalAddress to the virtualAddress, using the given tableFlags and
     * pageFlags. This only works while paging is disable, thus it is private for the paging
     * initializer.
     *
     * @param directory:		the targeted directory
     * @param virtualAddress:	the targeted virtual address in the adress space
     * @param physicalAddress:	the physical address of the page
     * @param tableFlags:		the table flags to apply
     * @param pageFlags:		the page flags to apply
     */
    static void mapPage(uint32_t* directory,
                        uint32_t  virtualAddress,
                        uint32_t  physicalAddress,
                        uint32_t  tableFlags,
                        uint32_t  pageFlags);
};

#endif
