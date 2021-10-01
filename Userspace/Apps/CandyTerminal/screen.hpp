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

#include <io/keyboard.hpp>
#include <stdint.h>
#include <string>

#ifndef SCREEN_HPP_
#    define SCREEN_HPP_

/**
 *
 */
typedef uint8_t ScreenColor;

#    define SC_BLACK    0x0
#    define SC_BLUE     0x1
#    define SC_GREEN    0x2
#    define SC_CYAN     0x3
#    define SC_RED      0x4
#    define SC_MAGENTA  0x5
#    define SC_BROWN    0x6
#    define SC_LGRAY    0x7
#    define SC_DARKGRAY 0x8
#    define SC_LBLUE    0x9
#    define SC_LGREEN   0xA
#    define SC_LCYAN    0xB
#    define SC_LRED     0xC
#    define SC_LMAGENTA 0xD
#    define SC_YELLOW   0xE
#    define SC_WHITE    0xF

#    define SC_COLOR(ba, fo) (fo | (ba << 4))

#    define VT100_COLOR_BLACK   0
#    define VT100_COLOR_RED     1
#    define VT100_COLOR_GREEN   2
#    define VT100_COLOR_YELLOW  3
#    define VT100_COLOR_BLUE    4
#    define VT100_COLOR_MAGENTA 5
#    define VT100_COLOR_CYAN    6
#    define VT100_COLOR_WHITE   7

/**
 * OEM-US special characters
 */
#    define OEMUS_CHAR_UE ((char)0x81) /*�*/

/**
 *
 */
class Screen {
public:
    ScreenColor colorForeground = SC_WHITE;
    ScreenColor colorBackground = SC_BLACK;
    bool        _lock           = false;

    virtual ~Screen() {
    }

    virtual Keyboard::Info readInput()              = 0;
    virtual void           clean()                  = 0;
    virtual void           backspace()              = 0;
    virtual void           writeChar(char c)        = 0;
    virtual void           moveCursor(int x, int y) = 0;
    virtual int            getCursorX()             = 0;
    virtual int            getCursorY()             = 0;

    virtual void setColorForeground(int c) {
        colorForeground = c;
    }
    virtual void setColorBackground(int c) {
        colorBackground = c;
    }

    virtual int getColorForeground() {
        return colorForeground;
    }
    virtual int getColorBackground() {
        return colorBackground;
    }

    virtual int getWidth()  = 0;
    virtual int getHeight() = 0;

    virtual void lock() {
        s_atomic_lock(&_lock);
    }
    virtual void unlock() {
        _lock = false;
    }

    virtual void setScrollAreaScreen()             = 0;
    virtual void setScrollArea(int start, int end) = 0;
    virtual void scroll(int value)                 = 0;

    virtual void setCursorVisible(bool visible) = 0;
};

#endif
