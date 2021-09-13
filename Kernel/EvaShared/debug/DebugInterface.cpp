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

#include "debug/DebugInterface.hpp"

#include "system/serial/SerialPort.hpp"

/**
 * flag that determines whether the interface is initialized
 */
bool DebugInterfaceInitialized = false;

/**
 * serial port value
 */
static uint16_t serialPort;

/**
 * on full log interface use buffering
 */
#if DEBUG_INTERFACE_MODE == DEBUG_INTERFACE_MODE_FULL
static const int logBufferLength = 512;
static char      logBuffer[logBufferLength];
static int       logBuffered = 0;
#endif

/**
 * initializes the inerface from the provided port
 *
 * @param port:		the port used for writing and reading
 */
void DebugInterface::initialize(uint16_t port) {
    DebugInterfaceInitialized = true;
    serialPort                = port;
}

/**
 * writes a log character on serial buffer
 *
 * @param c:	the character to write
 */
void DebugInterface::writeLogCharacter(char c) {
    // check ready flag
    if ( !DebugInterfaceInitialized )
        return;

#if DEBUG_INTERFACE_MODE == DEBUG_INTERFACE_MODE_PLAIN_LOG
    SerialPort::write(serialPort, c);

#elif DEBUG_INTERFACE_MODE == DEBUG_INTERFACE_MODE_FULL
    // put char in buffer
    logBuffer[logBuffered++] = c;

    // write buffer to output on newline or full buffer
    if ( c == '\n' || logBuffered == logBufferLength - 1 ) {
        writeShort(DEBUG_MESSAGE_LOG);

        for ( int i = 0; i < logBuffered; i++ )
            SerialPort::write(serialPort, logBuffer[i]);

        SerialPort::write(serialPort, 0);

        logBuffered = 0;
    }
#endif
}

/**
 * writes a log byte byte on serial buffer
 *
 * @param value:	the value to write
 */
void DebugInterface::writeByte(uint8_t value) {
    SerialPort::write(serialPort, value);
}

/**
 * writes a log short value on serial buffer
 *
 * @param value:	the value to write
 */
void DebugInterface::writeShort(uint16_t value) {
    SerialPort::write(serialPort, (value >> 0) & 0xFF);
    SerialPort::write(serialPort, (value >> 8) & 0xFF);
}

/**
 * writes a log integer value on serial buffer
 *
 * @param value:	the value to write
 */
void DebugInterface::writeInt(uint32_t value) {
    SerialPort::write(serialPort, (value >> 0) & 0xFF);
    SerialPort::write(serialPort, (value >> 8) & 0xFF);
    SerialPort::write(serialPort, (value >> 16) & 0xFF);
    SerialPort::write(serialPort, (value >> 24) & 0xFF);
}

/**
 * writes a log long value on serial buffer
 *
 * @param value:	the value to write
 */
void DebugInterface::writeLong(uint64_t value) {
    SerialPort::write(serialPort, (value >> 0) & 0xFF);
    SerialPort::write(serialPort, (value >> 8) & 0xFF);
    SerialPort::write(serialPort, (value >> 16) & 0xFF);
    SerialPort::write(serialPort, (value >> 24) & 0xFF);
    SerialPort::write(serialPort, (value >> 32) & 0xFF);
    SerialPort::write(serialPort, (value >> 40) & 0xFF);
    SerialPort::write(serialPort, (value >> 48) & 0xFF);
    SerialPort::write(serialPort, (value >> 56) & 0xFF);
}

/**
 * writes a log string on serial buffer
 *
 * @param string:	the string to write
 */
void DebugInterface::writeString(const char* string) {
    const char* p = string;
    while ( *p )
        SerialPort::write(serialPort, *p++);
    SerialPort::write(serialPort, 0);
}
