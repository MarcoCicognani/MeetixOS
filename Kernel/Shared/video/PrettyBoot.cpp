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

#include <BuildConfig.hpp>
#include <logger/logger.hpp>
#include <memory/memory.hpp>
#include <utils/string.hpp>
#include <video/ConsoleVideo.hpp>
#include <video/PrettyBoot.hpp>

/**
 * print the progress bar on screen for percent with color
 *
 * @param percent:		the percentage value of the bar
 * @param color:		the color of the bar
 */
void PrettyBoot::printProgressBar(int percent, uint8_t color) {
    // draw the bar container
    for ( int i = 10; i < 70; i++ ) {
        ConsoleVideo::putChar(i, 11, ' ', 0x80);
        ConsoleVideo::putChar(i, 12, ' ', 0x80);
    }

    // calculate the length of the bar
    int cells = (60 * percent) / 100;

    // draw the bar
    for ( int i = 10; i < 10 + cells; i++ ) {
        ConsoleVideo::putChar(i, 11, ' ', color);
        ConsoleVideo::putChar(i, 12, ' ', color);
    }
}

/**
 * print the provided string centered on the screen
 *
 * @param string:	the string to write to
 * @param y:		the y postion of the screen
 * @param color:	the color for the string
 */
void PrettyBoot::printCentered(const char* string, int y, uint8_t color) {
    // calculate the length and the start cell
    int strl      = String::length(string);
    int leftBound = 40 - strl / 2;

    // draw the background
    for ( int i = 0; i < 80; i++ )
        ConsoleVideo::putChar(i, y, ' ', 0x00);

    // print the string now
    ConsoleVideo::putString(leftBound, y, string, color);
}

/**
 * initialize the screen for pretty boot
 *
 * @param-opt clearScreen:	cleares the screen before use
 */
void PrettyBoot::enable(bool clearScreen) {
    Logger::setVideo(false);
    if ( clearScreen )
        ConsoleVideo::clear();
    ConsoleVideo::setVisualCursor(-1, -1);
}

/**
 * print the provided string and draw the bar with provided percent
 *
 * @param string:	the string to write to
 * @param y:		the y postion of the screen
 * @param color:	the color for the string
 */
void PrettyBoot::updateStatus(const char* string, int percent, uint8_t color) {
    printCentered(KERNEL_NAME, 0, 32);
    printProgressBar(percent, color);
    printCentered(string, 20, 0x07);
}

/**
 * called in case of fail, print a red bar
 *
 * @param string:	the error string to print
 */
void PrettyBoot::fail(const char* string) {
    printCentered(KERNEL_NAME, 0, 32);
    printProgressBar(100, 0xC0);
    printCentered(string, 20, RED);
}
