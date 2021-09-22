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

#ifndef __EVA_DEBUG_INTERFACE__
#define __EVA_DEBUG_INTERFACE__

#include "BuildConfig.hpp"
#include "debug/DebugInterfaceMode.hpp"
#include "debug/DebugProtocol.hpp"
#include "Api/StdInt.h"

/**
 * flag that determines whether the interface is initialized
 */
extern bool DebugInterfaceInitialized;

/**
 * debug interface to write on serial console
 */
class DebugInterface {
public:
    /**
     * initializes the inerface from the provided port
     *
     * @param port:		the port used for writing and reading
     */
    static void initialize(uint16_t port);

    /**
     * writes a s_log character on serial buffer
     *
     * @param c:	the character to write
     */
    static void writeLogCharacter(char c);

    /**
     * writes a s_log byte byte on serial buffer
     *
     * @param value:	the value to write
     */
    static void writeByte(uint8_t value);

    /**
     * writes a s_log short value on serial buffer
     *
     * @param value:	the value to write
     */
    static void writeShort(uint16_t value);

    /**
     * writes a s_log integer value on serial buffer
     *
     * @param value:	the value to write
     */
    static void writeInt(uint32_t value);

    /**
     * writes a s_log long value on serial buffer
     *
     * @param value:	the value to write
     */
    static void writeLong(uint64_t value);

    /**
     * writes a s_log string on serial buffer
     *
     * @param string:	the string to write
     */
    static void writeString(const char* string);
};

#endif
