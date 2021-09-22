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

#ifndef SYSTEM_ACPI_MADT
#define SYSTEM_ACPI_MADT

#include "Api/StdInt.h"

#include <system/acpi/AcpiTableHeader.hpp>

/**
 * generic madt header
 */
struct MadtHeader {
    AcpiTableHeader header;

    uint32_t localControllerAddress;
    uint32_t flags;
} A_PACKED;

/**
 * Header that each entry has
 */
struct MadtEntryHeader {
    uint8_t deviceType;
    uint8_t recordLength;
} A_PACKED;

/**
 * Entry describing a local APIC
 */
struct MadtLapicEntry {
    MadtEntryHeader header;
    uint8_t         processorId;
    uint8_t         apicId;
    uint32_t        flags;
} A_PACKED;

/**
 * Entry describing an IO APIC
 */
struct MadtIoapicEntry {
    MadtEntryHeader header;

    uint8_t  ioapicId;
    uint8_t  reserved0;
    uint32_t ioapicAddress;
    uint32_t globalSystemInterruptBase;
} A_PACKED;

/**
 * Entry describing an interrupt source override
 */
struct MadtInterruptSrcOverrideEntry {
    MadtEntryHeader header;

    uint8_t  busSource;
    uint8_t  irqSource;
    uint32_t globalSystemInterrupt;
    uint16_t flags;
} A_PACKED;

/**
 * Multiple APIC Descriptor Table
 */
class Madt {
public:
    /**
     * Reads the Multiple APIC Descriptor Table
     *
     * @param madtHeader:		the acpi table to parse
     */
    static void parse(AcpiTableHeader* madtHeader);
};

#endif
