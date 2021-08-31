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

#include <logger/logger.hpp>

/**
 * Prints the given format message using the given parameters.
 *
 * @see printFormatted
 *
 * @param message		the message pattern
 * @param ...			the parameters
 */
void Logger::print(const char *message, ...)
{
	va_list valist;
	va_start(valist, message);
	Logger::printFormatted(message, valist);
	va_end(valist);
}

/**
 * Prints a line using the print method, appending a line break.
 *
 * @see printFormatted
 *
 * @param message		the message pattern
 * @param ...			the parameters
 */
void Logger::println(const char *message, ...)
{
	va_list valist;
	va_start(valist, message);
	Logger::printFormatted(message, valist);
	va_end(valist);
	Logger::printCharacter('\n');
}

/**
 * Only needed in kernel implementation
 */
void Logger::manualLock()
{
}

/**
 * Only needed in kernel implementation
 */
void Logger::manualUnlock()
{
}
