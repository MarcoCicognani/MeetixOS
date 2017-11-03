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

#include "eva/types.h"
#include <system/acpi/madt.hpp>
#include <system/system.hpp>
#include <logger/logger.hpp>
#include <system/interrupts/lapic.hpp>
#include <system/interrupts/IoapicManager.hpp>

/**
 * Reads the Multiple APIC Descriptor Table
 *
 * @param madtHeader
 *		the acpi table to parse
 */
void Madt::parse(AcpiTableHeader *madtSdtHeader)
{
	MadtHeader *madtHeader = (MadtHeader*) madtSdtHeader;

	uint32_t entriesLength = (madtHeader->header.length - sizeof(MadtHeader));
	uint8_t *entriesData = (uint8_t*) (((Address) madtHeader) + sizeof(MadtHeader));

	/**
	 * TODO: tuxie mentioned that the value in the ACPI tables might not be trustworthy
	 * due to GRUB changing the APIC location and not updating the MADT. Check this.
	 */
	logDebug("%! reported LAPIC address %h", "madt", madtHeader->localControllerAddress);
	Lapic::prepare(madtHeader->localControllerAddress);

	logDebug("%! listing entries in MADT:", "madt");
	uint32_t pos = 0;
	while (pos < entriesLength)
	{
		MadtEntryHeader *entryHeader = (MadtEntryHeader*) &entriesData[pos];

		// switching acpi table
		switch (entryHeader->deviceType)
		{
			// Local APIC
			case 0:
			{
				MadtLapicEntry *entry = (MadtLapicEntry*) entryHeader;

				// only useable if entry flag 1
				if (entry->flags == 1)
				{
					logDebug("%# lapic, id: %i, processorId: %i, flags: %i", entry->apicId, entry->processorId, entry->flags);
					System::addProcessor(entry->apicId);
				}
			}
			break;

			// IO Apic
			case 1:
			{
				MadtIoapicEntry *entry = (MadtIoapicEntry*) entryHeader;
				logDebug("%# ioapic, id: %i, address: %h", entry->ioapicId, entry->ioapicAddress);
				IoApicManager::create(entry->ioapicId, entry->ioapicAddress, entry->globalSystemInterruptBase);
			}
			break;

			// Interrupt Source Override
			case 2:
			{
				#if	LOGGING_DEBUG
					MadtInterruptSrcOverrideEntry *entry = (MadtInterruptSrcOverrideEntry*) entryHeader;
					logDebug("%# int src override, irqSource: %i, globInt: %i, busSource: %i", entry->irqSource, entry->globalSystemInterrupt, entry->busSource);
				#endif
			}
			break;

			default: logDebug("%# device of unknown type %i", entryHeader->deviceType); break;
		}

		// increase position
		pos += entryHeader->recordLength;
	}
}
