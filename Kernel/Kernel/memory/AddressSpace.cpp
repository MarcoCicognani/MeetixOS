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

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <memory/constants.hpp>
#include <memory/memory.hpp>
#include <memory/paging.hpp>
#include <memory/physical/PPallocator.hpp>
#include <memory/TemporaryPagingUtil.hpp>
#include <tasking/tasking.hpp>

/**
 * Creates a mapping from the virtualAddress to the physicalAddress. Writes the entries
 * to the recursively mapped directory in the last 4MB of the memory.
 *
 * @param virt:					the virtual address to map to
 * @param phys:					the address of the physical page to map
 * @param tableFlags:			the flags to add on the table entry
 * @param pageFlags:			the flags to add on the page entry
 * @param allowOverride:		whether an existing entry may be overriden
 * @return true if success, false otherwise
 */
bool AddressSpace::map(VirtAddr virtualAddr,
                       PhysAddr physicalAddr,
                       uint32_t tableFlags,
                       uint32_t pageFlags,
                       bool     allowOverride) {
    // check if addresses are aligned
    if ( (virtualAddr & PAGE_ALIGN_MASK) || (physicalAddr & PAGE_ALIGN_MASK) )
        EvaKernel::panic("%! tried to map unaligned addresses: virt %h to phys %h",
                         "addrspace",
                         virtualAddr,
                         physicalAddr);

    // calculate table & page indices
    uint32_t ti = TABLE_IN_DIRECTORY_INDEX(virtualAddr);
    uint32_t pi = PAGE_IN_TABLE_INDEX(virtualAddr);

    // get pointers to directory and table
    PageDirectory directory = (PageDirectory)CONST_RECURSIVE_PAGE_DIRECTORY_ADDRESS;
    PageTable     table     = ((PageTable)CONST_RECURSIVE_PAGE_DIRECTORY_AREA) + (0x400 * ti);

    // create table if it does not exist
    if ( !directory[ti] ) {
        PhysAddr newTablePhys = PPallocator::allocate();
        if ( !newTablePhys )
            EvaKernel::panic("%! no pages left for mapping", "addrspace");

        // insert table
        directory[ti] = newTablePhys | tableFlags;

        // empty the created (and mapped) table
        for ( uint32_t i = 0; i < 1024; i++ )
            table[i] = 0;

        logDebug("%! created table %i", "addrspace", ti);
    }

    else {
        // this is illegal and an unrecoverable error
        if ( tableFlags & PAGE_TABLE_USERSPACE )
            if ( ((directory[ti] & PAGE_ALIGN_MASK) & PAGE_TABLE_USERSPACE) == 0 )
                EvaKernel::panic("%! tried to map user page in kernel space table, virt %h",
                                 "addrspace",
                                 virtualAddr);
    }

    // put address into table
    if ( !table[pi] || allowOverride ) {
        table[pi] = physicalAddr | pageFlags;

        // flush address
        INVLPG(virtualAddr);
        return true;
    }

    else {
        Thread* failor = Tasking::lastThread();
        if ( failor != 0 ) {
            const char* ident = failor->getIdentifier();
            if ( ident ) {
                logInfo(
                    "%! '%s' (%i) tried duplicate mapping, virt %h -> phys %h, table contains %h",
                    "addrspace",
                    ident,
                    failor->id,
                    virtualAddr,
                    physicalAddr,
                    table[pi]);
            }

            else {
                logInfo("%! %i tried duplicate mapping, virt %h -> phys %h, table contains %h",
                        "addrspace",
                        failor->id,
                        virtualAddr,
                        physicalAddr,
                        table[pi]);
            }
        }

        else {
            logInfo("%! unknown tried duplicate mapping, virt %h -> phys %h, table contains %h",
                    "addrspace",
                    virtualAddr,
                    physicalAddr,
                    table[pi]);
        }
    }
    return false;
}

/**
 * Maps a page to a page directory that is temporarily mapped into the current address space.
 *
 * @param directory:				the directory to map into
 * @param virtualAddress:			the virtual address to map to
 * @param physicalAddress:			the address of the physical page to map
 * @param tableFlags:				the flags to add on the table entry
 * @param pageFlags:				the flags to add on the page entry
 * @param allowOverride:			whether an existing entry may be overriden
 */
void AddressSpace::mapToTemporaryMappedDirectory(PageDirectory directory,
                                                 VirtAddr      virtualAddr,
                                                 PhysAddr      physicalAddr,
                                                 uint32_t      tableFlags,
                                                 uint32_t      pageFlags,
                                                 bool          allowOverride) {
    // check if addresses are aligned
    if ( (virtualAddr & PAGE_ALIGN_MASK) || (physicalAddr & PAGE_ALIGN_MASK) )
        EvaKernel::panic("%! tried to map unaligned addresses: virt %h to phys %h",
                         "addrspace",
                         virtualAddr,
                         physicalAddr);

    // calculate table & page indices
    uint32_t ti = TABLE_IN_DIRECTORY_INDEX(virtualAddr);
    uint32_t pi = PAGE_IN_TABLE_INDEX(virtualAddr);

    // create table if it does not exist
    if ( !directory[ti] ) {
        PhysAddr newTablePhys = PPallocator::allocate();
        if ( !newTablePhys )
            EvaKernel::panic("%! no pages left for mapping", "addrspace");

        // temporary map the table and insert it
        VirtAddr  tempTableAddr = TemporaryPagingUtil::map(newTablePhys);
        PageTable table         = (PageTable)tempTableAddr;
        for ( uint32_t i = 0; i < 1024; i++ )
            table[i] = 0;

        TemporaryPagingUtil::unmap(tempTableAddr);

        // insert table
        directory[ti] = newTablePhys | tableFlags;
        logDebug("%! created table %i while mapping %h - %h (temp)",
                 "addrspace",
                 ti,
                 virtualAddr,
                 physicalAddr);
    }

    // Insert address into table
    PhysAddr tablePhys = (directory[ti] & ~PAGE_ALIGN_MASK);

    VirtAddr  tempTableAddr = TemporaryPagingUtil::map(tablePhys);
    PageTable table         = (PageTable)tempTableAddr;
    if ( !table[pi] || allowOverride )
        table[pi] = physicalAddr | pageFlags;

    else {
        logWarn("%! tried to map area to virtual pd %h that was already mapped, virt %h -> phys "
                "%h, table contains %h",
                "addrspace",
                directory,
                virtualAddr,
                physicalAddr,
                table[pi]);

        EvaKernel::panic("%! duplicate mapping", "addrspace");
    }
    TemporaryPagingUtil::unmap(tempTableAddr);
}

/**
 * Unmaps the given virtual page in the current address space.
 *
 * @param virt:		the virtual address to unmap
 */
void AddressSpace::unmap(VirtAddr virtualAddress) {
    uint32_t ti = TABLE_IN_DIRECTORY_INDEX(virtualAddress);
    uint32_t pi = PAGE_IN_TABLE_INDEX(virtualAddress);

    PageDirectory directory = (PageDirectory)CONST_RECURSIVE_PAGE_DIRECTORY_ADDRESS;
    PageTable     table     = CONST_RECURSIVE_PAGE_TABLE(ti);
    if ( !directory[ti] )
        return;

    // Remove address from table
    if ( table[pi] != 0 ) {
        table[pi] = 0;

        // Flush address
        INVLPG(virtualAddress);
    }
}

/**
 * Switches to the given page directory.
 *
 * @param dir:		the directory to switch to
 */
void AddressSpace::switchToSpace(PageDirectory directory) {
    asm volatile("mov %0, %%cr3" ::"b"(directory));
}

/**
 * Returns the currently set page directory.
 *
 * @return the page directory
 */
PageDirectory AddressSpace::getCurrentSpace() {
    uint32_t directory;
    asm volatile("mov %%cr3, %0" : "=r"(directory));
    return (PageDirectory)directory;
}

/**
 * Reads for a given virtual address (which must exist in the currently mapped
 * address space) the underlying physical address.
 *
 * @param addr:		the address to resolve
 * @return the physical address
 */
PhysAddr AddressSpace::virtualToPhysical(VirtAddr addr) {
    uint32_t  ti    = TABLE_IN_DIRECTORY_INDEX(addr);
    uint32_t  pi    = PAGE_IN_TABLE_INDEX(addr);
    PageTable table = CONST_RECURSIVE_PAGE_TABLE(ti);

    if ( !table )
        return 0;
    return table[pi] & ~PAGE_ALIGN_MASK;
}
