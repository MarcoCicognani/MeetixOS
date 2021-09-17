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

#ifndef __EVA_SHARED_SYSTEM_IOPORTS__
#define __EVA_SHARED_SYSTEM_IOPORTS__

#include "eva/stdint.h"

/**
 * class to manage the low level I/O on the CPU
 */
class IOports {
public:
    /**
     * read a specific type of value from the specified port
     *
     * @param port:		the port where read
     * @return the value readed
     */
    template<typename T>
    static T in(uint16_t port) {
        T value;
        asm volatile("in %w1, %0" : "=a"(value) : "Nd"(port));
        return value;
    }

    /**
     * Outputs the value to the specified port
     *
     * @param port: 	the port
     * @param val:  	the value
     */
    template<typename T>
    static void out(unsigned port, T val) {
        asm volatile("out %0, %w1" : : "a"(val), "Nd"(port));
    }

    /**
     * reads a byte from the specified port
     *
     * @param port:		the port where read
     * @return the readed value
     */
    static uint8_t readByte(uint16_t port);

    /**
     * writes a byte on a specified port
     *
     * @param port:		the port where write
     * @param data:		the data to write
     */
    static void writeByte(uint16_t port, uint8_t data);

    /**
     * read a short from a specified port
     *
     * @param port:		the port where read
     * @return the readed value
     */
    static uint16_t readShort(uint16_t port);

    /**
     * writes a short on a specified port
     *
     * @param port:		the port where write
     * @param data:		the data to write
     */
    static void writeShort(uint16_t port, uint16_t data);

    /**
     * read an integer from a specified port
     *
     * @param port:		the port where read
     * @return the readed value
     */
    static uint32_t readInt(uint16_t port);

    /**
     * writes an integer on a specified port
     *
     * @param port:		the port where write
     * @param data:		the data to write
     */
    static void writeInt(uint16_t port, uint32_t data);
};

#endif
