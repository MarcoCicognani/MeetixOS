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

#ifndef EVA_INTERRUPTS_IDT_IDT
#define EVA_INTERRUPTS_IDT_IDT

#include "eva/stdint.h"
#include <system/interrupts/descriptors/idt.hpp>

/**
 * Interrupt descriptor table setup class
 */
class Idt
{
public:
	/**
	 * Installs the interrupt descriptor table.
	 */
	static void prepare();

	/**
	 * Installs the interrupt descriptor table.
	 *
	 * @reentrancy the same table is loaded on each core, therefore no locking necessary
	 */
	static void load();

	/**
	 * Fills the given values into the given IDT entry
	 *
	 * @param index:			gate index
	 * @param base:				gate base address
	 * @param kernelSegment: 	kernel code segment descriptor index
	 * @param flags:			the flags to apply
	 */
	static void createGate(uint32_t index, uint32_t base, uint16_t kernelSegment, uint8_t flags);

};

#endif
