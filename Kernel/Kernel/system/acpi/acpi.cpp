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

#include "eva/types.h"

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <memory/AddressSpace.hpp>
#include <system/acpi/acpi.hpp>
#include <system/acpi/madt.hpp>
#include <system/acpi/RsdpLookupUtil.hpp>
#include <utils/string.hpp>

/**
 * Remembers the root header and whether its an XSDT
 */
static AcpiTableHeader* rootHeader = 0;
static bool             rootIsXSDT = false;

/**
 * List of all tables in the root
 */
static AcpiEntry* first = 0;

/**
 * get the ACPI entry with provided signature
 *
 * @param signature:		signature of the entry
 * @return the ACPI entry if exist
 */
AcpiEntry* Acpi::getEntryWithSignature(const char* signature) {
    AcpiEntry* cur = first;
    while ( cur ) {
        if ( cur->hasSignature(signature) )
            return cur;
        cur = cur->next;
    }
    return 0;
}

/**
 * return the ACPI entry list
 *
 * @return the acpi list
 */
AcpiEntry* Acpi::getEntries() {
    return first;
}

/**
 * check ACPI entries
 *
 * @return true if there are entries
 */
bool Acpi::hasEntries() {
    return first != 0;
}

/**
 * read the RSDP
 */
void Acpi::gatherInformation() {
    logDebug("%! gathering information", "acpi");

    // Find root table pointer
    RsdpDescriptor* rsdp = RsdpLookupUtil::findRSDP();
    if ( rsdp ) {
        // Prepare the root table
        Acpi::prepareRootSDT(rsdp);

        // Go through the existing root entries
        for ( uint32_t i = 0; i < getRSDTentryCount(); i++ ) {
            PhysicalAddress entry = getRSDTentry(i);
            if ( entry != 0 ) {
                AcpiTableHeader* sdt = mapSDT(entry);

                // Could not be mapped? Skip
                if ( !sdt )
                    continue;

                // Create the entry
                AcpiEntry* entry = new AcpiEntry(sdt, first);
                first            = entry;
            }
        }

    }

    else {
        logDebug("%! could not find RSDP", "acpi");
    }
}

/**
 * Prepares the root SDT (RSTD or XSDT) by mapping the required
 * memory into the virtual address space.
 *
 * @param rsdp:		the rsdp descriptor where we start mapping
 */
void Acpi::prepareRootSDT(RsdpDescriptor* rsdp) {
    PhysicalAddress rootTableLocation = 0;

    rootIsXSDT = false;

    // If ACPI 2.0 or higher, try to use the XSDT
    if ( rsdp->revision > 0 ) {
        RsdpDescriptor20* rsdp20 = (RsdpDescriptor20*)rsdp;
        if ( rsdp20->xsdtAddress != 0 ) {
#if _ARCH_X86_64_
            rootTableLocation = rsdp20->xsdtAddress;
            logDebug("%! found XSDT in 64bit range", "acpi");
            rootIsXSDT = true;
#elif _ARCH_X86_
            if ( rsdp20->xsdtAddress < 0xFFFFFFFF ) {
                rootIsXSDT = true;
                logDebug("%! found XSDT in 32bit range", "acpi");
                rootTableLocation = rsdp20->xsdtAddress;
            }

            else {
                rootIsXSDT = false;
                logDebug("%! found XSDT, but range too high for 32bits, attempting to use RSDT",
                         "acpi");
            }
#endif
        }
    }

    // No XSDT? Use RSDT
    if ( !rootIsXSDT ) {
        logDebug("%! no XSDT; using RSDT", "acpi");
        rootTableLocation = rsdp->rsdtAddress;
    }

    // No header? Failed
    if ( !rootTableLocation ) {
        logWarn("%! RSDP did not contain a valid RSDT/XSDT address", "acpi");
        return;
    }

    logDebug("%! root table starts at phys %h", "acpi", rootTableLocation);

    // Map table in virtual address space
    AcpiTableHeader* header = mapSDT(rootTableLocation);
    if ( !header ) {
        logWarn("%! could not map root system descriptor table", "acpi");
        return;
    }

    rootHeader = header;
}

/**
 * Temporarily maps the SDT at the given location to the virtual address space,
 * retrieves its length and then unmaps it again.
 *
 * @return the length of the table or 0 if failed
 */
uint32_t Acpi::getLengthOfUnmappedSDT(PhysicalAddress tableLocation) {
    // Align the location down, we will allocate 2 pages to make sure the
    // header is within the range
    PhysicalAddress physStart   = PAGE_ALIGN_DOWN(tableLocation);
    VirtualAddress  virtualBase = EvaKernel::evaKernelRangePool->allocate(2);

    if ( !AddressSpace::map(virtualBase,
                            physStart,
                            DEFAULT_KERNEL_TABLE_FLAGS,
                            DEFAULT_KERNEL_PAGE_FLAGS) ) {
        logWarn("%! could not create virtual mapping (1) for SDT %h", "acpi", tableLocation);
        return 0;
    }
    if ( !AddressSpace::map(virtualBase + PAGE_SIZE,
                            physStart + PAGE_SIZE,
                            DEFAULT_KERNEL_TABLE_FLAGS,
                            DEFAULT_KERNEL_PAGE_FLAGS) ) {
        logWarn("%! could not create virtual mapping (2) for SDT %h", "acpi", tableLocation);
        return 0;
    }

    // Calculate the offset of the header within the table
    uint32_t mappingOffset = tableLocation - physStart;

    // Take length from the header
    AcpiTableHeader* header = (AcpiTableHeader*)(virtualBase + mappingOffset);
    uint32_t         length = header->length;

    // Unmap the two mapped pages
    AddressSpace::unmap(virtualBase);
    AddressSpace::unmap(virtualBase + PAGE_SIZE);
    EvaKernel::evaKernelRangePool->free(virtualBase);

    return length;
}

/**
 * Maps the table at the given physical address into the virtual space.
 * First uses the {getLengthOfUnmappedSDT} to get the length of the table
 * and then maps the required size in the address space.
 *
 * @return the header of the mapped table or nullptr
 */
AcpiTableHeader* Acpi::mapSDT(PhysicalAddress tableLocation) {
    // Retrieve the tables length
    uint32_t tableLength = getLengthOfUnmappedSDT(tableLocation);
    if ( !tableLength ) {
        logWarn("%! could not map SDT at phys %h, could not get table length",
                "acpi",
                tableLocation);
        return 0;
    }

    // Does the length make sense?
    if ( tableLength > SDT_MAXIMUM_BYTES ) {
        logWarn("%! SDT at %h was skipped due to illegal length (%h)",
                "acpi",
                tableLocation,
                tableLength);
        return 0;
    }

    // Down/upalign physical range
    PhysicalAddress physStart = PAGE_ALIGN_DOWN(tableLocation);
    PhysicalAddress physEnd   = PAGE_ALIGN_UP(tableLocation + tableLength);

    // Calculate offset of header within first page
    uint32_t mappingOffset = tableLocation - physStart;

    // Calculate amount of physical pages and allocate virtual range
    uint32_t       pages       = (physEnd - physStart) / PAGE_SIZE;
    VirtualAddress virtualBase = EvaKernel::evaKernelRangePool->allocate(pages);

    // Could not find a virtual range of that size
    if ( !virtualBase ) {
        logWarn("%! could not find a free virtual range to map an SDT of size %i pages",
                "acpi",
                pages);
        return 0;
    }

    // Map the pages
    for ( VirtualAddress off = 0; off < (physEnd - physStart); off = off + PAGE_SIZE )
        AddressSpace::map(virtualBase + off,
                          physStart + off,
                          DEFAULT_KERNEL_TABLE_FLAGS,
                          DEFAULT_KERNEL_PAGE_FLAGS);

    // Get the header pointer
    AcpiTableHeader* header = (AcpiTableHeader*)(virtualBase + mappingOffset);

    // Validate the table
    if ( !validateSDT(header) ) {
        logWarn("%! descriptor table was not valid", "acpi");
        return 0;
    }

    // Now return the tables header
    return header;
}

/**
 * Validates the whole system descriptor table with the given header
 *
 * @param header:		header to validate
 * @return whether the SDT is valid
 */
bool Acpi::validateSDT(AcpiTableHeader* header) {
    uint8_t  sum        = 0;
    uint8_t* tableBytes = reinterpret_cast<uint8_t*>(header);

    for ( uint32_t i = 0; i < header->length; i++ )
        sum += tableBytes[i];

    return sum == 0;
}

/**
 * Returns the number of entries that the RSDT with the given header has
 *
 * @return the number of entries
 */
uint32_t Acpi::getRSDTentryCount() {
    uint32_t entryBytes = rootHeader->length - sizeof(AcpiTableHeader);
    if ( rootIsXSDT )
        return entryBytes / 8;
    else
        return entryBytes / 4;
}

/**
 * Returns the RSDT entry at the given index.
 *
 * @param index:		the index of the RSD entry
 * @return the physical address of the entry
 */
PhysicalAddress Acpi::getRSDTentry(uint32_t index) {
    VirtualAddress startOfEntries = ((VirtualAddress)rootHeader) + sizeof(AcpiTableHeader);

    if ( rootIsXSDT )
        return ((uint64_t*)startOfEntries)[index];
    else
        return ((uint32_t*)startOfEntries)[index];

    return 0;
}
