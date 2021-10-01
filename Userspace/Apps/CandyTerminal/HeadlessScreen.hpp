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

#ifndef __TERMINAL_SCREEN_HEADLESS__
#define __TERMINAL_SCREEN_HEADLESS__

#include "screen.hpp"

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
#define VIDEO_MEMORY  0xB8000

/**
 *
 */
class HeadlessScreen : public Screen {
private:
    uint32_t id;
    uint8_t* outputCurrent;

    uint32_t offset;

    uint32_t activeProcessId;
    bool  lock = false;
    void     normalize();

    bool cursorVisible = true;
    void updateVisualCursor();

    bool scrollAreaScreen = true;
    int  scrollAreaStart  = 0;
    int  scrollAreaEnd    = 0;

public:
    HeadlessScreen();

    Keyboard::Info readInput();
    void           clean();
    void           backspace();
    void           writeChar(char c);
    void           moveCursor(int x, int y);
    int            getCursorX();
    int            getCursorY();
    int            getWidth();
    int            getHeight();

    void setScrollAreaScreen();
    void setScrollArea(int start, int end);
    void scroll(int value);

    void setCursorVisible(bool visible);
};

#endif
