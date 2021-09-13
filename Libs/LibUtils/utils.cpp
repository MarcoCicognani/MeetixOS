/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                  *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwctype>
#include <eva.h>
#include <utils/utils.hpp>

/**
 * trim the whitespaces of the provided string and return the valid part
 *
 * @param str:		the string to trim
 * @return the trimmed string
 */
std::string Utils::trim(std::string str) {
    int left = 0;
    while ( isspace(str[left]) )
        ++left;

    int right = str.length();
    while ( isspace(str[right - 1]) )
        --right;

    // string is all whitespaces
    if ( right <= left )
        return "";

    // return the valid part
    return str.substr(left, right - left);
}

/**
 * prints a log on current log interface of the system
 *
 * @param message:		the message to print to log
 * @param ...:			parameters for formatted message
 */
void Utils::log(const std::string& message, ...) {
    va_list l;
    va_start(l, message);
    log(message.c_str(), l);
    va_end(l);
}

/**
 * prints a log on current log interface of the system
 *
 * @param message:		the message to print to log
 * @param ...:			parameters for formatted message
 */
void Utils::log(const char* message, ...) {
    va_list l;
    va_start(l, message);
    log(message, l);
    va_end(l);
}

/**
 * prints a log on current log interface of the system
 *
 * @param message:		the message to print to log
 * @param l:			arguments list
 */
void Utils::log(const char* message, va_list l) {
    uint32_t msglen = strlen(message);
    uint32_t buflen = msglen * 2;
    char*    buf    = new char[buflen];
    vsnprintf(buf, buflen, message, l);
    Log(buf);
    delete buf;
}

/**
 * reads a byte from the provided port
 *
 * @param port:		the port where read to
 * @return the readed value
 */
uint8_t Utils::Cpu::inportByte(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

/**
 * writes a byte on the provided port
 *
 * @param port:		the port where write to
 * @param data:		the data to write
 */
void Utils::Cpu::outportByte(uint16_t port, uint8_t value) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

/**
 * reads a short from the provided port
 *
 * @param port:		the port where read to
 * @return the readed value
 */
uint16_t Utils::Cpu::inportShort(uint16_t port) {
    uint16_t value;
    asm volatile("inw %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

/**
 * writes a short on the provided port
 *
 * @param port:		the port where write to
 * @param data:		the data to write
 */
void Utils::Cpu::outportShort(uint16_t port, uint16_t value) {
    asm volatile("outw %1, %0" : : "dN"(port), "a"(value));
}

/**
 * reads an integer from the provided port
 *
 * @param port:		the port where read to
 * @return the readed value
 */
uint32_t Utils::Cpu::inportInt(uint16_t port) {
    uint32_t value;
    asm volatile("inl %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

/**
 * writes an integer on the provided port
 *
 * @param port:		the port where write to
 * @param data:		the data to write
 */
void Utils::Cpu::outportInt(uint16_t port, uint32_t value) {
    asm volatile("outl %1, %0" : : "dN"(port), "a"(value));
}
