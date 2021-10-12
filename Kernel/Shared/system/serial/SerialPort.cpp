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

#include <system/IOPorts.hpp>
#include <system/serial/SerialPort.hpp>

/**
 * initialize the serial communcation from the provided port
 *
 * @param port:					the port that serial use for communcation
 * @param interruptsEnabled:	tell to initializer if interrupts are enabled
 */
void SerialPort::initializePort(uint16_t port, bool interruptsEnabled) {
    // Disable the interrupts
    IOPorts::writeByte(port + SERIAL_PORT_OFFSET_INTERRUPT_ENABLE, 0x00);

    // Set DLAB
    IOPorts::writeByte(port + SERIAL_PORT_OFFSET_LINE_CONTROL, 0x80);

    // Divisor to 3, baud 38400
    IOPorts::writeByte(port + SERIAL_PORT_OFFSET_DIVISOR_LEAST, 0x03); // Low byte
    IOPorts::writeByte(port + SERIAL_PORT_OFFSET_DIVISOR_MOST, 0x00);  // High byte

    // Set line control to 0x03. Bits:
    // 0: 1  Charlen 8
    // 1: 1  ^
    // 2: 0  one stop bit
    // 3: 0  no parity
    // 4: 0  ^
    // 5: 0  ^
    IOPorts::writeByte(port + SERIAL_PORT_OFFSET_LINE_CONTROL, 0x03);

    if ( interruptsEnabled ) {
        // Enable FIFO
        IOPorts::writeByte(port + SERIAL_PORT_OFFSET_INT_FIFO, 0xC7);

        // Enable IRQs with RTS/DSR set
        IOPorts::writeByte(port + SERIAL_PORT_OFFSET_MODEM_CONTROL, 0x0B);

        // Enable the interrupts
        // 0: 1  Receiver has data interrupt on
        // 1: 1  Transmitter empty interrupt on
        // 2: 0  Error/break interrupt off
        // 3: 0  Status change interrupt off
        IOPorts::writeByte(port + SERIAL_PORT_OFFSET_INTERRUPT_ENABLE, 0x02);
    }

    else {
        // Turn off FIFO
        IOPorts::writeByte(port + SERIAL_PORT_OFFSET_INT_FIFO, 0x00);

        // Loopback off, no IRQs, not RTS/DTR
        IOPorts::writeByte(port + SERIAL_PORT_OFFSET_MODEM_CONTROL, 0x00);
    }
}

/**
 * read serial from provided port code
 *
 * @param port:		the port where read
 * @return the readed value
 */
uint8_t SerialPort::read(uint16_t port) {
    // Wait byte available
    while ( !(IOPorts::readByte(port + SERIAL_PORT_OFFSET_LINE_STATUS) & 0x01) )
        ;

    // Receive byte
    return IOPorts::readByte(port + SERIAL_PORT_OFFSET_DATA_REGISTER);
}

/**
 * write a value on a specific serial port
 *
 * @param port:		the port where write
 * @param value:	the value to write
 */
void SerialPort::write(uint16_t port, uint8_t value) {
    // Wait until ready
    while ( !(IOPorts::readByte(port + SERIAL_PORT_OFFSET_LINE_STATUS) & 0x20) )
        ;

    // Write byte
    IOPorts::writeByte(port + SERIAL_PORT_OFFSET_DATA_REGISTER, value);
}
