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

#ifndef SYSTEM_ACPI_ACPI
#define SYSTEM_ACPI_ACPI

#include "Api/Kernel.h"
#include "Api/StdInt.h"
#include "Api/Types.h"
#include "Api/utils/llist.hpp"

#include <system/acpi/AcpiEntry.hpp>
#include <system/acpi/AcpiTableHeader.hpp>
#include <system/acpi/rsdp.hpp>

// Set a maximum for SDTs to avoid problems with possible junk tables
#define SDT_MAXIMUM_BYTES 0x10000

/**
 * Advanced Configuration and Power Interface
 */
class Acpi {
public:
    /**
     * get the ACPI entry with provided signature
     *
     * @param signature:		signature of the entry
     * @return the ACPI entry if exist
     */
    static AcpiEntry* getEntryWithSignature(const char* signature);

    /**
     * return the ACPI entry list
     *
     * @return the acpi list
     */
    static AcpiEntry* getEntries();

    /**
     * check ACPI entries
     *
     * @return true if there are entries
     */
    static bool hasEntries();

    /**
     * read the RSDP
     */
    static void gatherInformation();

private:
    /**
     * Prepares the root SDT (RSTD or XSDT) by mapping the required
     * memory into the virtual address space.
     *
     * @param rsdp:		the rsdp descriptor where we start mapping
     */
    static void prepareRootSDT(RsdpDescriptor* rsdp);

    /**
     * Temporarily maps the SDT at the given location to the virtual address space,
     * retrieves its length and then unmaps it again.
     *
     * @return the length of the table or 0 if failed
     */
    static uint32_t getLengthOfUnmappedSDT(PhysAddr tableLocation);

    /**
     * Maps the table at the given physical address into the virtual space.
     * First uses the {getLengthOfUnmappedSDT} to get the length of the table
     * and then maps the required size in the address space.
     *
     * @return the header of the mapped table or nullptr
     */
    static AcpiTableHeader* mapSDT(PhysAddr tableLocation);

    /**
     * Validates the whole system descriptor table with the given header
     *
     * @param header:		header to validate
     * @return whether the SDT is valid
     */
    static bool validateSDT(AcpiTableHeader* header);

    /**
     * Returns the number of entries that the RSDT with the given header has
     *
     * @return the number of entries
     */
    static uint32_t getRSDTentryCount();

    /**
     * Returns the RSDT entry at the given index.
     *
     * @param index:		the index of the RSD entry
     * @return the physical address of the entry
     */
    static PhysAddr getRSDTentry(uint32_t index);
};

#endif
