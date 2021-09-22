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

#ifndef EVA_GDT_INITIALIZER
#define EVA_GDT_INITIALIZER

#include "Api/StdInt.h"

#include <memory/gdt/gdt.hpp>
#include <memory/gdt/tss.hpp>
#include <memory/memory.hpp>
#include <memory/paging.hpp>

/**
 * Number of entries in a GDT
 */
#define GDT_NUM_ENTRIES 7

/**
 * Global Descriptor Table List Entry
 */
struct GdtListEntry {
    GdtPointer ptr;
    GdtEntry   entry[GDT_NUM_ENTRIES];
    Tss        tss;
};

/**
 * GDT initialization manager
 */
class GdtManager {
public:
    /**
     * Creates the array of GDT-to-core assignment
     * structs. Only called by the BSP.
     */
    static void prepare();

    /**
     * Initializes the local GDT.
     */
    static void initialize();

    /**
     * Sets the ESP0 in the TSS. This is necessary for the system to know which stack
     * to use when switching from ring 3 to ring 0.
     *
     * @param esp0:		the address of the esp to set
     */
    static void setTssEsp0(VirtAddr esp0);

    /**
     * set the user thread address from the provided address
     *
     * @param userThreadAddr:		the new user thread address
     */
    static void setUserThreadAddress(VirtAddr userThreadAddr);
};

#endif
