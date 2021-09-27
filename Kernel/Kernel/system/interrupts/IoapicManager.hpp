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

#ifndef EVA_INTERRUPTS_IOAPICMANAGER
#define EVA_INTERRUPTS_IOAPICMANAGER

#include "Api/Kernel.h"
#include "Api/StdInt.h"
#include "Api/utils/llist.hpp"

#include <system/interrupts/ioapic.hpp>

/**
 * I/O APIC manager
 */
class IoApicManager {
public:
    /**
     * create the first IoApic object
     *
     * @param id:							the IOApic id
     * @param ioapicAddress:				the physical address of the IOApic
     * @param globalSystemInterruptBase:	base interrupt managed by IOApic
     */
    static void create(uint32_t id, PhysAddr ioapicAddress, uint32_t globalSystemInterruptBase);

    /**
     * return the list with all IoApic entries
     *
     * @return the list with all IoApic entries
     */
    static IoApic* getEntries();

    /**
     * check if there are IOApic objects
     *
     * @return true if there are entries
     */
    static bool areAvailable();

    /**
     * create a redirection entry from provided parameters
     *
     * @param source:				the responsible IOApic object
     * @param irq:					the irq code
     * @param destinationApic:		IOApic destination code
     * @return true if successful
     */
    static bool createIsaRedirectionEntry(uint32_t source, uint32_t irq, uint32_t destinationApic);

    /**
     * mask the provided irq
     *
     * @param irq:		the irq to be mapped
     */
    static void maskIrq(uint32_t irq);

    /**
     * unmask the provided irq
     *
     * @param irq:		the irq to be unmasked
     */
    static void unmaskIrq(uint32_t irq);

private:
    /**
     * get the io apic instance from code
     *
     * @param source:		identifier of the IoApic instance
     * @return the IoApic object if exist
     */
    static IoApic* getResponsibleFor(uint32_t source);
};

#endif
