/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM * This software is derived
 * from the Ghost operating system project,           * written by Max Schlüssel
 * <lokoxe@gmail.com>. Copyright 2012-2017            * https://ghostkernel.org/
 * * https://github.com/maxdev1/ghost *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#include <Graphics/Color.hh>
#include <IO/Keyboard.hh>
#include <stdint.h>
#include <string>

#ifndef _SCREEN_HPP_
#    define _SCREEN_HPP_

#    define SCREEN_WIDTH  80
#    define SCREEN_HEIGHT 25
#    define VIDEO_MEMORY  0xB8000

/**
 * OEM-US special characters
 */
#    define OEMUS_CHAR_UE ((char)0x81) /*�*/

/**
 *
 */
class Screen {
public:
    virtual ~Screen() {
    }

    virtual void clean()      = 0;
    virtual void deactivate() = 0;
    virtual void activate()   = 0;

    virtual void close() = 0;

    virtual void backspace()               = 0;
    virtual void cleanLine(int lineLength) = 0;
    virtual void write(std::string message, Graphics::Color::ArgbGradient color = Graphics::Color::as_rgb(255, 255, 255), bool visible = true)
        = 0;
    virtual void writeChar(char c, Graphics::Color::ArgbGradient color = Graphics::Color::as_rgb(255, 255, 255)) = 0;
    virtual void updateCursor()                                        = 0;

    virtual bool setColor(std::string color) = 0;

    virtual IO::Keyboard::Info readInput(bool* cancelCondition) = 0;

    virtual void workingDirectoryChanged(std::string str) {
    }
};

#endif
