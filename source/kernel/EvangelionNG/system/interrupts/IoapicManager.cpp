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

#include <system/interrupts/IoapicManager.hpp>
#include <system/interrupts/ioapic.hpp>

#include <system/IOPorts.hpp>
#include <logger/logger.hpp>
#include <EvangelionNG.hpp>
#include <system/processor.hpp>

/**
 * global linked list of IoApic object
 */
static IoApic *first = 0;

/**
 * create the first IoApic object
 *
 * @param id:							the IOApic id
 * @param ioapicAddress:				the physical address of the IOApic
 * @param globalSystemInterruptBase:	base interrupt managed by IOApic
 */
void IoApicManager::create(uint32_t id, PhysicalAddress ioapicAddress, uint32_t globalSystemInterruptBase)
{
	first = new IoApic(id, ioapicAddress, globalSystemInterruptBase, first);
}

/**
 * return the list with all IoApic entries
 *
 * @return the list with all IoApic entries
 */
IoApic *IoApicManager::getEntries()
{
	return first;
}

/**
 * check if there are IOApic objects
 *
 * @return true if there are entries
 */
bool IoApicManager::areAvailable()
{
	return first != 0;
}

/**
 * create a redirection entry from provided parameters
 *
 * @param source:				the responsible IOApic object
 * @param irq:					the irq code
 * @param destinationApic:		IOApic destination code
 * @return true if successful
 */
bool IoApicManager::createIsaRedirectionEntry(uint32_t source, uint32_t irq, uint32_t destinationApic)
{
	IoApic *n = getResponsibleFor(source);

	if (n)
	{
		uint64_t redirectionTableEntry = 0;
		redirectionTableEntry |= IOAPIC_REDTBL_INTVEC_MAKE(0x20 + irq);
		redirectionTableEntry |= IOAPIC_REDTBL_DELMOD_FIXED;
		redirectionTableEntry |= IOAPIC_REDTBL_DESTMOD_PHYSICAL;
		redirectionTableEntry |= IOAPIC_REDTBL_INTPOL_HIGH_ACTIVE;
		redirectionTableEntry |= IOAPIC_REDTBL_TRIGGERMOD_EDGE;
		redirectionTableEntry |= IOAPIC_REDTBL_INTMASK_UNMASKED;
		redirectionTableEntry |= IOAPIC_REDTBL_DESTINATION_MAKE(destinationApic, IOAPIC_REDTBL_DESTINATION_FLAG_PHYSICAL);

		n->setRedirectionEntry(source, redirectionTableEntry);

		logDebug("%! wrote ISA redirection entry %i -> %i", "ioapicmgr", source, irq);
		return true;
	}

	else
	{
		logWarn("%! found no response I/O APIC for interrupt %i", "ioapicmgr", source);
		return false;
	}
}

/**
 * mask the provided irq
 *
 * @param irq:		the irq to be mapped
 */
void IoApicManager::maskIrq(uint32_t irq)
{
	IoApic *n = getResponsibleFor(irq);

	if (n)
	{
		logDebug("masking irq %i", irq);
		n->mask(irq);
	}

	else
	{
		logWarn("%! tried to mask IRQ %i that no IOAPIC is responsible for", "ioapicmgr", irq);
	}
}

/**
 * unmask the provided irq
 *
 * @param irq:		the irq to be unmasked
 */
void IoApicManager::unmaskIrq(uint32_t irq)
{
	IoApic *n = getResponsibleFor(irq);

	if (n)
	{
		logDebug("unmasking irq %i", irq);
		n->unmask(irq);
	}

	else
	{
		logWarn("%! tried to unmask IRQ %i that no IOAPIC is responsible for", "ioapicmgr", irq);
	}
}

/**
 * get the io apic instance from code
 *
 * @param source:		identifier of the IoApic instance
 * @return the IoApic object if exist
 */
IoApic *IoApicManager::getResponsibleFor(uint32_t source)
{
	IoApic *n = first;
	while (n)
	{
		if (source >= n->getGlobalSystemInterruptBase() && source < (n->getGlobalSystemInterruptBase() + n->getRedirectEntryCount())) break;
		n = n->getNext();
	}
	return n;
}
