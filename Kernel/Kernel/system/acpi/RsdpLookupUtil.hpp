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

#ifndef SYSTEM_ACPI_RSDPLOOKUPUTIL
#define SYSTEM_ACPI_RSDPLOOKUPUTIL

#include "Api/StdInt.h"
#include "Api/Types.h"

#include <system/acpi/rsdp.hpp>

/**
 *
 */
class RsdpLookupUtil {
public:
    /**
     * Looks for the RSDP in the range from "start" to "end".
     *
     * @param start:	the start index
     * @param end:		the end index
     * @return the RSDP, otherwise 0.
     */
    static RsdpDescriptor* findRSDPinRange(uint32_t start, uint32_t end);

    /**
     * Searches for the RSDP in the Extended BIOS Data Area.
     *
     * @return the RSDP, otherwise 0.
     */
    static RsdpDescriptor* findRSDPinEBDA();

    /**
     * Validates the "rsdp" by checking its revision and then using the
     * right checksum method.
     *
     * @return true if the pointer is valid
     */
    static bool validateRSDP(RsdpDescriptor* rsdp);

    /**
     * Attempts to find the Root System Description Pointer
     *
     * @return either the RSDP or 0
     */
    static RsdpDescriptor* findRSDP();
};

#endif
