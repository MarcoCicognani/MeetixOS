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

#ifndef SYSTEM_ACPI_ACPIENTRY
#define SYSTEM_ACPI_ACPIENTRY

#include "Api/Kernel.h"
#include "Api/StdInt.h"

#include <system/acpi/AcpiTableHeader.hpp>
#include <system/acpi/rsdp.hpp>

/**
 * Entries for a linked list of all ACPI tables
 */
class AcpiEntry {
public:
    /**
     * internal data
     */
    AcpiTableHeader* header; // ACPI table

    /**
     * linkage data
     */
    AcpiEntry* next; // next acpi entry

    /**
     * constructor
     *
     * @param _header:		the value for the internal table header
     */
    AcpiEntry(AcpiTableHeader* _header, AcpiEntry* _next) : header(_header), next(_next) {
    }

    /**
     * check ACPI signature on provided string
     *
     * @param str:		signature name
     * @return true if has provided signature, false otherwise
     */
    bool hasSignature(const char* str);
};

#endif
