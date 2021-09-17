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

#ifndef EVA_TEMPORARY_PAGING_UTIL
#define EVA_TEMPORARY_PAGING_UTIL

#include "eva/stdint.h"

#include <memory/memory.hpp>
#include <memory/paging.hpp>

/**
 * The temporary paging util keeps a small stack of virtual addresses
 * and uses them to map any physical page so it can be arbitrarily written.
 */
class TemporaryPagingUtil {
public:
    /**
     * keep a stack of physical pages and initialize the paging on it
     */
    static void initialize();

    /**
     * map the physical address to virtual address
     *
     * @param phys:		the physical address to map
     * @return the virtual address
     */
    static VirtualAddress map(PhysicalAddress phys);

    /**
     * unmap the given virtual address
     *
     * @param virt:		the virtual address to be unmapped
     */
    static void unmap(VirtualAddress virt);
};

#endif
