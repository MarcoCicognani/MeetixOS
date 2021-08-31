/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                       *
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

#include <memory/gdt/gdt.hpp>
#include <memory/gdt/GdtMounter.hpp>
#include <logger/logger.hpp>

/**
 * Fills the given GDTEntry with the given data.
 *
 * @param gdtEntry:		the target GDT entry
 * @param base:			the base address to write
 * @param limit:		the limit to write
 * @param access:		the access flag to write
 * @param granularity:	the granularity to write
 */
void Gdt::createGate(GdtEntry *gdtEntry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
	gdtEntry->baseLow = (base & 0xFFFF);
	gdtEntry->baseMiddle = (base >> 16) & 0xFF;
	gdtEntry->baseHigh = (base >> 24) & 0xFF;

	gdtEntry->limitLow = (limit & 0xFFFF);
	gdtEntry->limitHigh = limit >> 16;

	gdtEntry->granularity = granularity;

	gdtEntry->access = access;
}
