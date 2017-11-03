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

#ifndef EVA_SHARED_SYSTEM_SERIAL_SERIALPORT
#define EVA_SHARED_SYSTEM_SERIAL_SERIALPORT

#include "eva/stdint.h"

/**
 * Constants for COM port communcation
 */
#define SERIAL_PORT_OFFSET_DATA_REGISTER		0  // without DLAB, register for receiving and writing
#define SERIAL_PORT_OFFSET_INTERRUPT_ENABLE		1  // without DLAB, interrupt enable register
#define SERIAL_PORT_OFFSET_DIVISOR_LEAST		0  // with DLAB, least significant divisor byte
#define SERIAL_PORT_OFFSET_DIVISOR_MOST			1  // with DLAB, most significant divisor byte
#define SERIAL_PORT_OFFSET_INT_FIFO				2  // interrupt identification and FIFO control register
#define SERIAL_PORT_OFFSET_LINE_CONTROL			3  // line control register
#define SERIAL_PORT_OFFSET_MODEM_CONTROL		4  // modem control register
#define SERIAL_PORT_OFFSET_LINE_STATUS			5  // line status register
#define SERIAL_PORT_OFFSET_MODEM_STATUS			6  // modem status register
#define SERIAL_PORT_OFFSET_SCRATCH				7  // scratch register

/**
 * Class for serial port access
 */
class SerialPort
{
public:
	/**
	 * initialize the serial communcation from the provided port
	 *
	 * @param port:					the port that serial use for communcation
	 * @param interruptsEnabled:	tell to initializer if interrupts are enabled
	 */
	static void initializePort(uint16_t port, bool interruptsEnabled);

	/**
	 * read serial from provided port code
	 *
	 * @param port:		the port where read
	 * @return the readed value
	 */
	static uint8_t read(uint16_t port);

	/**
	 * write a value on a specific serial port
	 *
	 * @param port:		the port where write
	 * @param value:	the value to write
	 */
	static void write(uint16_t port, uint8_t value);
};

#endif
