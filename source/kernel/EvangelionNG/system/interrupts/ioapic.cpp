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

#include <system/interrupts/ioapic.hpp>

#include <memory/AddressSpace.hpp>
#include <logger/logger.hpp>
#include <EvangelionNG.hpp>

/**
 * map the current object on the memory
 */
void IoApic::createMapping()
{
	// Get a virtual range for mapping
	virtualAddress = EvaKernel::evaKernelRangePool->allocate(2);
	if (!virtualAddress) EvaKernel::panic("%! could not get a virtual range for mapping", "ioapic");

	// Add the physical offset to the virtual address
	virtualAddress += physicalAddress - PAGE_ALIGN_DOWN(physicalAddress);

	// Map the two pages
	logDebug("%! mapped at %h (phys %h)", "ioapic", virtualAddress, physicalAddress);
	AddressSpace::map(PAGE_ALIGN_DOWN(virtualAddress), PAGE_ALIGN_DOWN(physicalAddress), DEFAULT_KERNEL_TABLE_FLAGS, DEFAULT_KERNEL_PAGE_FLAGS);
	AddressSpace::map(PAGE_ALIGN_DOWN(virtualAddress) + PAGE_SIZE, PAGE_ALIGN_DOWN(physicalAddress) + PAGE_SIZE, DEFAULT_KERNEL_TABLE_FLAGS, DEFAULT_KERNEL_PAGE_FLAGS);
}

/**
 * initialize the object
 */
void IoApic::initialize()
{
	createMapping();

	// Get ID
	uint32_t idValue = read(IOAPIC_ID);
	uint32_t reportedId = (idValue >> 24) & 0xF;

	// If not right ID, reprogram it
	if (reportedId != id)
	{
		logWarn("%! has different ID (%i) than what ACPI reported (%i), reprogramming", "ioapic", idValue, reportedId);

		// Remove the actual ID bits
		idValue &= ~(0xF << 24);
		// Set new ID bits
		idValue |= (id & 0xF) << 24;
		// Write value
		write(IOAPIC_ID, idValue);
	}

	// Get version
	uint32_t versionValue = read(IOAPIC_VER);
	uint32_t version = versionValue & 0xFF;
	redirectEntryCount = (versionValue >> 16) & 0xFF;
	logInfo("%! id %i: version %i, redirect entries: %i", "ioapic", id, version, redirectEntryCount);
}

/**
 * write on the provided ioapic regex the provided value
 *
 * @param reg:		the regex where we write
 * @param value:	the value to be written
 */
void IoApic::write(uint32_t reg, uint32_t value)
{
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGSEL)) = reg;
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGWIN)) = value;
}

/**
 * read on the provided ioapic regex
 *
 * @param reg:		the regex where we read
 * @return the readed value
 */
uint32_t IoApic::read(uint32_t reg)
{
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGSEL)) = reg;
	return *((volatile uint32_t*) (virtualAddress + IOAPIC_REGWIN));
}

/**
 * get the redirection entry from provided index
 *
 * @param index:		the memory index
 * @return the index of the redirection
 */
uint64_t IoApic::getRedirectionEntry(uint32_t index)
{
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGSEL)) = IOAPIC_REDTBL_BASE + index * 2;
	uint64_t lo = *((volatile uint32_t*) (virtualAddress + IOAPIC_REGWIN));
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGSEL)) = IOAPIC_REDTBL_BASE + index * 2 + 1;
	uint64_t hi = *((volatile uint32_t*) (virtualAddress + IOAPIC_REGWIN));

	return (hi << 32) | lo;
}

/**
 * set a new value of redirection entry on index
 *
 * @param index:		the index of the redirection to be modified
 * @param value:		the new value of the redirection
 */
void IoApic::setRedirectionEntry(uint32_t index, uint64_t value)
{
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGSEL)) = IOAPIC_REDTBL_BASE + index * 2;
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGWIN)) = value & 0xFFFFFFFF;
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGSEL)) = IOAPIC_REDTBL_BASE + index * 2 + 1;
	*((volatile uint32_t*) (virtualAddress + IOAPIC_REGWIN)) = value >> 32;
}

/**
 * mask the provided source
 *
 * @param source:		the index to be masked
 */
void IoApic::mask(uint32_t source)
{
	uint32_t entryIndex = source - globalSystemInterruptBase;
	uint64_t entry = getRedirectionEntry(entryIndex);

	entry &= ~(IOAPIC_REDTBL_MASK_INTMASK);
	entry |= IOAPIC_REDTBL_INTMASK_MASKED;

	setRedirectionEntry(entryIndex, entry);
}

/**
 * unmask the provided source
 *
 * @param source:		the index to be unmasked
 */
void IoApic::unmask(uint32_t source)
{
	uint32_t entryIndex = source - globalSystemInterruptBase;
	uint64_t entry = getRedirectionEntry(entryIndex);

	entry &= ~(IOAPIC_REDTBL_MASK_INTMASK);
	entry |= IOAPIC_REDTBL_INTMASK_UNMASKED;

	setRedirectionEntry(entryIndex, entry);
}
