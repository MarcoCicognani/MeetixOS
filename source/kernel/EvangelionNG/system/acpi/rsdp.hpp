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

#ifndef SYSTEM_ACPI_RSDP
#define SYSTEM_ACPI_RSDP

#include "eva/stdint.h"

// "RSD PTR ", vice-versa (little endian)
#define RSDPTR_MAGIC	0x2052545020445352

/**
 * Root System Description Pointer 1.0
 */
struct RsdpDescriptor
{
	char signature[8];      // This 8-byte string (not null terminated!) must contain "RSD PTR ". It stands on a 16-byte boundary.
	uint8_t checksum;       // The value to add to all the other bytes (of the Version 1.0 table) to calculate the Checksum of the table.
	char oemId[6];          // The specification says: "An OEM-supplied string that identifies the OEM".
	uint8_t revision;       // ACPI revision
	uint32_t rsdtAddress;   // 32-bit physical address of the RSDT table
}__attribute__((packed));

/**
 * Root System Description Pointer 2.0
 */
struct RsdpDescriptor20
{
	RsdpDescriptor descriptor;

	uint32_t length;          // The size of the entire table since offset 0 to the end.
	uint64_t xsdtAddress;     // 64-bit physical address of the XSDT table
	uint8_t extendedChecksum; // checksum of the entire table
	uint8_t reserved[3];      // 3 bytes to be ignored in reading and that must not be written
}__attribute__((packed));

#endif
