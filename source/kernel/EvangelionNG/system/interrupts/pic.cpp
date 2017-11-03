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

#include <system/interrupts/pic.hpp>
#include <system/IOPorts.hpp>

/**
 * Reports an end-of-interrupt for the interrupt with the given number
 *
 * @param intr:		the number of the interrupt which has ended
 */
void Pic::sendEoi(uint8_t intr)
{
	// If the interrupt is an IRQ (> 32) and on the second PIC (> 8)
	// we have to tell the slave that the interrupt has ended too
	if (intr >= 0x20 + 0x08) IOports::writeByte(PIC2_COMMAND, 0x20);
	IOports::writeByte(PIC1_COMMAND, 0x20);
}

/**
 * Masks the IRQ with the given number. The IRQ line is then
 * blocked until its unmasked again.
 *
 * @param irq:		the IRQ to be masked
 */
void Pic::maskIrq(uint8_t irq)
{
	uint16_t port;
	uint8_t value;

	if (irq < 8) port = PIC1_DATA;
	else
	{
		port = PIC2_DATA;
		irq -= 8;
	}

	value = IOports::readByte(port);
	value |= (1 << irq);
	IOports::writeByte(port, value);
}

/**
 * Unmasks the IRQ with the given number.
 *
 * @param irq:		the IRQ to be unmasked
 */
void Pic::unmaskIrq(uint8_t irq)
{
	uint16_t port;
	uint8_t value;

	if (irq < 8) port = PIC1_DATA;
	else
	{
		port = PIC2_DATA;
		irq -= 8;
	}

	value = IOports::readByte(port);
	value &= ~(1 << irq);
	IOports::writeByte(port, value);

}

/**
 * Remaps the IRQs which normally start at interrupt 0
 * so that they start at 0x20. This is done to avoid
 * conflicts between IRQs and exceptions.
 */
void Pic::remapIrqs()
{
	IOports::writeByte(PIC1_COMMAND, 0x11);
	IOports::writeByte(PIC2_COMMAND, 0x11);
	IOports::writeByte(PIC1_DATA, 0x20); // IRQ0 - IRQ7 => interrupts 0x20-0x27
	IOports::writeByte(PIC2_DATA, 0x28); // IRQ8 - IRQ15 => interrupts 0x28-0x2F
	IOports::writeByte(PIC1_DATA, 0x4);
	IOports::writeByte(PIC2_DATA, 0x2);
	IOports::writeByte(PIC1_DATA, 0x1);
	IOports::writeByte(PIC2_DATA, 0x1);
}

/**
 * Masks all interrupts
 */
void Pic::maskAll()
{
	IOports::writeByte(PIC1_DATA, 0xFF);
	IOports::writeByte(PIC2_DATA, 0xFF);
}

/**
 * Unmasks all interrupts
 */
void Pic::unmaskAll()
{
	IOports::writeByte(PIC1_DATA, 0x0);
	IOports::writeByte(PIC2_DATA, 0x0);
}
