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

#include "HeadlessScreen.hpp"

#include <eva.h>
#include <string.h>
#include <utils/utils.hpp>

static uint32_t screenIdCounter = 0;

/**
 *
 */
HeadlessScreen::HeadlessScreen() {
    id            = screenIdCounter++;
    outputCurrent = (uint8_t*)VIDEO_MEMORY;

    offset          = 0;
    activeProcessId = 0;
    lock            = false;
    clean();
}

/**
 *
 */
void HeadlessScreen::clean() {
    AtomicLock(&lock);
    for ( uint32_t off = 0; off < SCREEN_HEIGHT * SCREEN_WIDTH * 2; off += 2 ) {
        outputCurrent[off]     = ' ';
        outputCurrent[off + 1] = SC_COLOR(SC_BLACK, SC_WHITE);
    }
    offset = 0;
    lock   = false;
}

/**
 *
 */
void HeadlessScreen::updateVisualCursor() {
    int x = (offset % (SCREEN_WIDTH * 2)) / 2;
    int y = offset / (SCREEN_WIDTH * 2);

    AtomicLock(&lock);
    uint16_t position = (y * SCREEN_WIDTH) + x;

    Utils::Cpu::outportByte(0x3D4, 0x0F);
    Utils::Cpu::outportByte(0x3D5, (uint8_t)(position & 0xFF));
    Utils::Cpu::outportByte(0x3D4, 0x0E);
    Utils::Cpu::outportByte(0x3D5, (uint8_t)((position >> 8) & 0xFF));
    lock = false;
}

/**
 *
 */
void HeadlessScreen::moveCursor(int x, int y) {
    AtomicLock(&lock);

    offset = (y * SCREEN_WIDTH * 2) + x * 2;
    if ( offset < 0 )
        offset = 0;

    else if ( offset > (SCREEN_WIDTH * 2 * SCREEN_HEIGHT) )
        offset = SCREEN_WIDTH * 2 * SCREEN_HEIGHT;

    lock = false;

    updateVisualCursor();
}

/**
 *
 */
void HeadlessScreen::writeChar(char c) {
    // Print to the screen
    AtomicLock(&lock);

    if ( c == '\n' ) {
        offset = offset + SCREEN_WIDTH * 2;
        offset = offset - (offset % (SCREEN_WIDTH * 2));
    }

    else {
        outputCurrent[offset++] = c;
        outputCurrent[offset++] = (uint8_t)SC_COLOR(colorBackground, colorForeground);
    }

    lock = false;

    // Ensure valid offset
    normalize();

    // Re-position BIOS cursor
    updateVisualCursor();
}

/**
 *
 */
void HeadlessScreen::backspace() {
    AtomicLock(&lock);

    offset                  = offset - 2;
    outputCurrent[offset++] = ' ';
    ++offset; // keep color
    offset = offset - 2;

    lock = false;

    updateVisualCursor();
}

/**
 *
 */
void HeadlessScreen::normalize() {
    AtomicLock(&lock);

    if ( offset >= SCREEN_WIDTH * SCREEN_HEIGHT * 2 ) {
        offset = offset - SCREEN_WIDTH * 2;

        uint32_t lineBytes  = SCREEN_WIDTH * 2;
        uint32_t screenSize = SCREEN_HEIGHT * lineBytes;

        memcpy(outputCurrent, &outputCurrent[SCREEN_WIDTH * 2], screenSize - lineBytes);

        for ( uint32_t i = 0; i < SCREEN_WIDTH * 2; i += 2 ) {
            outputCurrent[screenSize - lineBytes + i]     = ' ';
            outputCurrent[screenSize - lineBytes + i + 1] = SC_BLACK;
        }
    }

    lock = false;
}
/**
 *
 */
void HeadlessScreen::setCursorVisible(bool visible) {
    cursorVisible = visible;
    if ( visible )
        updateVisualCursor();
    else {
        AtomicLock(&lock);
        // effectively hides the cursor
        Utils::Cpu::outportByte(0x3D4, 0x0F);
        Utils::Cpu::outportByte(0x3D5, (uint8_t)(-1 & 0xFF));
        Utils::Cpu::outportByte(0x3D4, 0x0E);
        Utils::Cpu::outportByte(0x3D5, (uint8_t)((-1 >> 8) & 0xFF));
        lock = false;
    }
}

/**
 *
 */
void HeadlessScreen::setScrollAreaScreen() {
    scrollAreaScreen = true;
}

/**
 *
 */
void HeadlessScreen::setScrollArea(int start, int end) {
    scrollAreaScreen = false;
    AtomicLock(&lock);
    scrollAreaStart = start > SCREEN_HEIGHT ? SCREEN_HEIGHT : start;
    scrollAreaEnd   = end > SCREEN_HEIGHT ? SCREEN_HEIGHT : end;
    lock            = false;
}

/**
 *
 */
void HeadlessScreen::scroll(int value) {
    int scrollStart = scrollAreaScreen ? 0 : scrollAreaStart;
    int scrollEnd   = scrollAreaScreen ? SCREEN_HEIGHT : scrollAreaEnd;

    AtomicLock(&lock);

    if ( value > 0 ) {
        for ( int i = scrollEnd - 2; i >= scrollStart; i-- )
            memcpy(&outputCurrent[(i + value) * SCREEN_WIDTH * 2],
                   &outputCurrent[i * SCREEN_WIDTH * 2],
                   SCREEN_WIDTH * 2);
    }

    else {
        for ( int i = scrollStart; i < scrollEnd; i++ )
            memcpy(&outputCurrent[i * SCREEN_WIDTH * 2],
                   &outputCurrent[(i + value) * SCREEN_WIDTH * 2],
                   SCREEN_WIDTH * 2);
    }

    lock = false;
}

/**
 *
 */
Keyboard::Info HeadlessScreen::readInput() {
    return Keyboard::read();
}

/**
 *
 */
int HeadlessScreen::getCursorX() {
    return (offset % (SCREEN_WIDTH * 2)) / 2;
}

/**
 *
 */
int HeadlessScreen::getCursorY() {
    return offset / (SCREEN_WIDTH * 2);
}

/**
 *
 */
int HeadlessScreen::getWidth() {
    return SCREEN_WIDTH;
}

/**
 *
 */
int HeadlessScreen::getHeight() {
    return SCREEN_HEIGHT;
}
