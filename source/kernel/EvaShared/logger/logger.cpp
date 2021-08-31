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
#include <utils/string.hpp>
#include <video/ConsoleVideo.hpp>
#include <system/serial/SerialPort.hpp>
#include <system/BiosDataArea.hpp>
#include "debug/DebugInterface.hpp"

/**
 * Width of a log entry header
 */
static const uint32_t HEADER_WIDTH = 15;

/**
 * Logger flags
 */
static bool logSerial = false;
static bool logVideo = true;

/**
 * enables the serial COM1 port for logging
 */
void Logger::enableSerialPortLogging()
{
	logSerial = true;
}

/**
 * Enables or disables the serial output.
 *
 * @param serial:		whether to enable the serial output
 */
void Logger::setSerial(bool serial)
{
	logSerial = serial;
}

/**
 * Enables or disables the video output.
 *
 * @param video:		whether to enable the video output
 */
void Logger::setVideo(bool video)
{
	logVideo = video;
}

/**
 * Prints the given message, using the arguments from the given variable
 * argument list, formatting the values as:
 *
 * %i		as a value with base 10 using printNumber
 * %h		as a value with base 16 using printNumber
 * %b		true or false
 * %s		a plain string
 * %%		a '%' character
 *
 * The following options are also available and should be added only at
 * the start of the pattern:
 *
 * %!		adds the respective value as an header to the line
 * %*		allows changing the color of the header to the respective value
 * %#		indents the following message by the width of an header
 * 			(this option does not require a value!)
 *
 * @param message:		the message pattern
 * @param va:			the list of arguments
 */
void Logger::printFormatted(const char *constMessage, va_list valist)
{
	// copy as non const pointer the parameter
	char *message = (char*) constMessage;

	// set the default color
	uint16_t headerColor = 0x07;

	// parse the message
	while (*message)
	{
		// format identifier
		if (*message == '%')
		{
			// increase pointer, need next character to understand format
			++message;

			// integer
			if (*message == 'i')
			{
				int32_t val = va_arg(valist, int32_t);
				printNumber(val, 10);
			}

			// positive hex number
			else if (*message == 'h')
			{
				uint32_t val = va_arg(valist, uint32_t);
				printPlain("0x");
				printNumber(val, 16);
			}

			// boolean
			else if (*message == 'b')
			{
				int val = va_arg(valist, int);
				printPlain((const char*) (val ? "true" : "false"));
			}

			// char
			else if (*message == 'c')
			{
				int val = va_arg(valist, int);
				printCharacter((char) val);
			}

			// string
			else if (*message == 's')
			{
				char *val = va_arg(valist, char*);
				printPlain(val);
			}

			// indented printing
			else if (*message == '#')
			{
				for (uint32_t i = 0; i < HEADER_WIDTH + 2; i++)
					printCharacter(' ');
			}

			// indented header printing
			else if (*message == '!')
			{
				char *val = va_arg(valist, char*);
				uint32_t headerlen = String::length(val);

				if (headerlen < HEADER_WIDTH)
				{
					for (uint32_t i = 0; i < HEADER_WIDTH - headerlen; i++)
						printCharacter(' ');
				}

				printCharacter('[');
				ConsoleVideo::setColor(headerColor);
				print(val);
				ConsoleVideo::setColor(0x0F);
				printCharacter(']');
			}

			// escaped %
			else if (*message == '%') printCharacter(*message);

			// header color change
			else if (*message == '*') headerColor = (uint16_t) (va_arg(valist, int));
		}

		// print character
		else printCharacter(*message);

		// get next character
		++message;
	}
}

/**
 * Prints a number with a base.
 *
 * @param number:		the number to print
 * @param base:			the base to use
 * 						- if the base is 10 then signs are added
 * 						- if the base is 16 a preceding '0x' is added
 */
void Logger::printNumber(uint32_t number, uint16_t base)
{
	// Remember if negative
	uint8_t negative = 0;
	if (base == 10)
	{
		negative = ((int32_t) number) < 0;
		if (negative) number = -number;
	}

	// Write chars in reverse order, not nullterminated
	char revbuf[32];

	char *cbufp = revbuf;
	int len = 0;
	do
	{
		*cbufp++ = "0123456789ABCDEF"[number % base];
		++len;
		number /= base;
	}
	while (number);

	// If base is 16, write 0's until 8
	if (base == 16)
	{
		while (len < 8)
		{
			*cbufp++ = '0';
			++len;
		}
	}

	// Reverse buffer
	char buf[len + 1];
	for (int i = 0; i < len; i++)
		buf[i] = revbuf[len - i - 1];
	buf[len] = 0;

	// Print number
	if (negative) printCharacter('-');
	printPlain(buf);
}

/**
 * Prints the given message.
 *
 * @param message:		the message to print
 */
void Logger::printPlain(const char *constMessage)
{
	char *message = (char*) constMessage;
	while (*message)
		printCharacter(*message++);
}

/**
 * Prints the given character, redirecting it the video and/or serial output.
 *
 * @param c:	the character to print
 */
void Logger::printCharacter(char c)
{
	if (logVideo) ConsoleVideo::print(c);
	if (logSerial) DebugInterface::writeLogCharacter(c);
}
