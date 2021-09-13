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

/**
 * reads a byte from the specified port
 *
 * @param port:		the port where read
 * @return the readed value
 */
uint8_t IOports::readByte(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

/**
 * writes a byte on a specified port
 *
 * @param port:		the port where write
 * @param data:		the data to write
 */
void IOports::writeByte(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

/**
 * read a short from a specified port
 *
 * @param port:		the port where read
 * @return the readed value
 */
uint16_t IOports::readShort(uint16_t port) {
    uint16_t value;
    asm volatile("inw %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

/**
 * writes a short on a specified port
 *
 * @param port:		the port where write
 * @param data:		the data to write
 */
void IOports::writeShort(uint16_t port, uint16_t value) {
    asm volatile("outw %1, %0" : : "dN"(port), "a"(value));
}

/**
 * read an integer from a specified port
 *
 * @param port:		the port where read
 * @return the readed value
 */
uint32_t IOports::readInt(uint16_t port) {
    uint32_t value;
    asm volatile("inl %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

/**
 * writes an integer on a specified port
 *
 * @param port:		the port where write
 * @param data:		the data to write
 */
void IOports::writeInt(uint16_t port, uint32_t value) {
    asm volatile("outl %1, %0" : : "dN"(port), "a"(value));
}
