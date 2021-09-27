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

#ifndef EVA_INTERRUPTS_PIC
#define EVA_INTERRUPTS_PIC

#include "Api/StdInt.h"

/**
 * PIC constants
 */
#define PIC1         0x20 // Master
#define PIC2         0xA0 // Slave
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

/**
 * Programmable interrupt controller driver
 */
class Pic {
public:
    /**
     * Reports an end-of-interrupt for the interrupt with the given number
     *
     * @param intr:		the number of the interrupt which has ended
     */
    static void sendEoi(uint8_t intr);

    /**
     * Masks the IRQ with the given number. The IRQ line is then
     * blocked until its unmasked again.
     *
     * @param irq:		the IRQ to be masked
     */
    static void maskIrq(uint8_t irq);

    /**
     * Unmasks the IRQ with the given number.
     *
     * @param irq:		the IRQ to be unmasked
     */
    static void unmaskIrq(uint8_t irq);

    /**
     * Remaps the IRQs which normally start at interrupt 0
     * so that they start at 0x20. This is done to avoid
     * conflicts between IRQs and exceptions.
     */
    static void remapIrqs();

    /**
     * Masks all interrupts
     */
    static void maskAll();

    /**
     * Unmasks all interrupts
     */
    static void unmaskAll();
};

#endif
