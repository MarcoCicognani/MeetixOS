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

#ifndef EVA_SHARED_MEMORY_GDT_GDT
#define EVA_SHARED_MEMORY_GDT_GDT

#include "eva/stdint.h"
#include <memory/gdt/gdt.hpp>
#include <memory/gdt/GdtPointer.hpp>
#include <memory/gdt/GdtEntry.hpp>
#include <memory/gdt/GdtMacros.hpp>

/**
 * Class for writing values to a GDT entry
 */
class Gdt
{
public:

	/**
	 * Fills the given GDTEntry with the given data.
	 *
	 * @param gdtEntry:		the target GDT entry
	 * @param base:			the base address to write
	 * @param limit:		the limit to write
	 * @param access:		the access flag to write
	 * @param granularity:	the granularity to write
	 */
	static void createGate(GdtEntry *gdtEntry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
};

#endif
