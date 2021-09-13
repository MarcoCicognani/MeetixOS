/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
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

#ifndef MEETIX_LIBRARY_UTILS_UTILS
#define MEETIX_LIBRARY_UTILS_UTILS

#include <stdarg.h>
#include <string>

/**
 * defines a pretty logging macro
 */
#define LOG(msg, ...) Utils::log(std::string(__PRETTY_FUNCTION__) + ": " + msg, __VA_ARGS__);

/**
 * static class that provides various utilities
 */
class Utils {
private:
    /**
     * remove constructor
     */
    Utils() = delete;

public:
    /**
     * trim the whitespaces of the provided string and return the valid part
     *
     * @param str:		the string to trim
     * @return the trimmed string
     */
    static std::string trim(std::string str);

    /**
     * prints a log on current log interface of the system
     *
     * @param message:		the message to print to log
     * @param ...:			parameters for formatted message
     */
    static void log(const std::string& message, ...);

    /**
     * prints a log on current log interface of the system
     *
     * @param message:		the message to print to log
     * @param ...:			parameters for formatted message
     */
    static void log(const char* message, ...);

    /**
     * prints a log on current log interface of the system
     *
     * @param message:		the message to print to log
     * @param l:			arguments list
     */
    static void log(const char* message, va_list l);

    /**
     * static class to send low level commands to cpu
     */
    class Cpu {
    private:
        /**
         * remove constructor
         */
        Cpu() = delete;

    public:
        /**
         * reads a byte from the provided port
         *
         * @param port:		the port where read to
         * @return the readed value
         */
        static uint8_t inportByte(uint16_t port);

        /**
         * writes a byte on the provided port
         *
         * @param port:		the port where write to
         * @param data:		the data to write
         */
        static void outportByte(uint16_t port, uint8_t data);

        /**
         * reads a short from the provided port
         *
         * @param port:		the port where read to
         * @return the readed value
         */
        static uint16_t inportShort(uint16_t port);

        /**
         * writes a short on the provided port
         *
         * @param port:		the port where write to
         * @param data:		the data to write
         */
        static void outportShort(uint16_t port, uint16_t data);

        /**
         * reads an integer from the provided port
         *
         * @param port:		the port where read to
         * @return the readed value
         */
        static uint32_t inportInt(uint16_t port);

        /**
         * writes an integer on the provided port
         *
         * @param port:		the port where write to
         * @param data:		the data to write
         */
        static void outportInt(uint16_t port, uint32_t data);
    };
};

#endif
