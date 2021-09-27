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

#ifndef EVA_SHARED_MULTIBOOT_MULTIBOOT
#define EVA_SHARED_MULTIBOOT_MULTIBOOT

#include "Api/StdInt.h"

/**
 * Multiboot related constants
 */
#define MULTIBOOT_HEADER_MAGIC     0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

/**
 * a.out symbol table
 */
struct MultibootAoutSymbolTable {
    uint32_t tabSize;
    uint32_t strSize;
    uint32_t address;
    uint32_t reserved;
} A_PACKED;

/**
 * ELF section header
 */
struct MultibootElfSectionHeaderTable {
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
} A_PACKED;

/**
 * Structure of the multiboot information. This struct is
 * built by GRUB somewhere in memory, and a pointer to it is
 * passed to the multiboot-compliant loader assembler stub.
 */
struct MultibootInformation {
    uint32_t flags;
    uint32_t memoryLower;
    uint32_t memoryUpper;
    uint32_t bootDevice;
    uint32_t cmdline;
    uint32_t modulesCount;
    uint32_t modulesAddress;
    union {
        MultibootAoutSymbolTable       aoutSymbolTable;
        MultibootElfSectionHeaderTable elfSectionHeaderTable;
    } tables;
    uint32_t memoryMapLength;
    uint32_t memoryMapAddress;
} A_PACKED;

/**
 * Structure of a multiboot module.
 */
struct MultibootModule {
    uint32_t moduleStart;
    uint32_t moduleEnd;
    char*    path;
    uint32_t reserved;
} A_PACKED;

/**
 * Structure of one entry in the memory map.
 */
struct MultibootMmap {
    uint32_t size;
    uint32_t baseAddressLower;
    uint32_t baseAddressHigher;
    uint32_t lengthLower;
    uint32_t lengthHigher;
    uint32_t type;
} A_PACKED;

#endif
