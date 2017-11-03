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

#ifndef EVA_SHARED_VIDEO_PRETTYBOOT
#define EVA_SHARED_VIDEO_PRETTYBOOT

#include <stdarg.h>
#include "eva/stdint.h"
#include <BuildConfig.hpp>

/**
 * create macros to pretty boot functions
 */
#if PRETTY_BOOT
#define PRETTY_BOOT_STATUS(text, percent, color)		PrettyBoot::updateStatus(text, percent, color)
#define PRETTY_BOOT_FAIL(text)							PrettyBoot::fail(text)

#else
#define PRETTY_BOOT_STATUS(text, percent, color)        {}
#define PRETTY_BOOT_FAIL(text)                          {}
#endif

/**
 * colors are always avaible
 */
#define RED 	4
#define GREEN	32

/**
 * the class provide methods to create a text progress bar on screen to show the system loading
 */
class PrettyBoot
{
private:
	/**
	 * print the progress bar on screen for percent with color
	 *
	 * @param percent:		the percentage value of the bar
	 * @param color:		the color of the bar
	 */
	static void printProgressBar(int percent, uint8_t color);

	/**
	 * print the provided string centered on the screen
	 *
	 * @param string:	the string to write to
	 * @param y:		the y postion of the screen
	 * @param color:	the color for the string
	 */
	static void printCentered(const char *string, int y, uint8_t color);

public:
	/**
	 * initialize the screen for pretty boot
	 *
	 * @param-opt clearScreen:	cleares the screen before use
	 */
	static void enable(bool clearScreen = true);

	/**
	 * print the provided string and draw the bar with provided percent
	 *
	 * @param string:	the string to write to
	 * @param y:		the y postion of the screen
	 * @param color:	the color for the string
	 */
	static void updateStatus(const char *string, int percent, uint8_t color);

	/**
	 * called in case of fail, print a red bar
	 *
	 * @param string:	the error string to print
	 */
	static void fail(const char *string);
};

#endif
