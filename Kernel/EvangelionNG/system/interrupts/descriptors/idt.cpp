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

#include <system/interrupts/descriptors/idt.hpp>
#include <system/interrupts/descriptors/IdtPointer.hpp>
#include <system/interrupts/descriptors/IdtEntry.hpp>
#include <system/interrupts/descriptors/IdtMounter.hpp>
#include <system/interrupts/handling/InterruptDispatcher.hpp>
#include <system/system.hpp>
#include <logger/logger.hpp>

/**
 * IDT pointer structure
 */
IdtPointer idtPointer;

/**
 * Interrupt descriptor table (consisting of 256 IDT entries)
 */
IdtEntry idt[256];

/**
 * Installs the interrupt descriptor table.
 */
void Idt::prepare()
{
	// Create the IDT pointer
	idtPointer.limit = (sizeof(IdtEntry) * 256) - 1;
	idtPointer.base = (uint32_t) & idt;

	// Empty the IDT
	uint8_t *idtp = (uint8_t*) (&idt);
	for (uint32_t i = 0; i < sizeof(IdtEntry) * 256; i++)
		idtp[i] = 0;

	// Install interrupt handlers
	InterruptDispatcher::install();
}

/**
 * Installs the interrupt descriptor table.
 *
 * @reentrancy the same table is loaded on each core, therefore no locking necessary
 */
void Idt::load()
{
	// Load the IDT
	logDebug("%! descriptor table lays at %h", "idt", &idt);
	logDebug("%! pointer at %h, base %h, limit %h", "idt", &idtPointer, idtPointer.base, idtPointer.limit);

	_loadIdt((uint32_t) &idtPointer);

	logDebug("%! loaded on core %i", "idt", System::currentProcessorId());
}

/**
 * Fills the given values into the given IDT entry
 *
 * @param index:			gate index
 * @param base:				gate base address
 * @param kernelSegment: 	kernel code segment descriptor index
 * @param flags:			the flags to apply
 */
void Idt::createGate(uint32_t index, uint32_t base, uint16_t kernelSegment, uint8_t flags)
{
	idt[index].baseLow = base & 0xFFFF;
	idt[index].baseHigh = (base >> 16) & 0xFFFF;
	idt[index].kernelSegment = kernelSegment;
	idt[index].zero = 0;
	idt[index].flags = flags;
}
