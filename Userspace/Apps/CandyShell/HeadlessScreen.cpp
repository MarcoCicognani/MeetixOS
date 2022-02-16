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

#include <Api.h>
#include <LibUtils/Utils.hh>
#include <string.h>

static uint8_t* outputVideoDirect = (uint8_t*)VIDEO_MEMORY;
static uint32_t screenIdCounter   = 0;

/**
 *
 */
HeadlessScreen::HeadlessScreen() {
    id               = screenIdCounter++;
    outputBuffer     = new uint8_t[SCREEN_WIDTH * SCREEN_HEIGHT * 2];
    m_output_current = outputBuffer;

    m_offset         = 0;
    m_active_proc_id = 0;
    lock             = false;
    clean();
}

/**
 *
 */
void HeadlessScreen::clean() {
    s_atomic_lock(&lock);
    for ( uint32_t off = 0; off < SCREEN_HEIGHT * SCREEN_WIDTH * 2; off = off + 2 ) {
        outputBuffer[off]     = ' ';
        outputBuffer[off + 1] = (uint8_t)Graphics::Color::as_rgb(0, 0, 0);
    }
    m_offset = 0;
    lock     = false;
}

/**
 *
 */
void HeadlessScreen::activate() {
    s_atomic_lock(&lock);
    memcpy((uint8_t*)VIDEO_MEMORY, outputBuffer, SCREEN_HEIGHT * SCREEN_WIDTH * 2);
    m_output_current = outputVideoDirect;
    lock             = false;
}

/*
 *
 */
void HeadlessScreen::close() {
    delete outputBuffer;
    delete m_output_current;

    delete this;
}

/**
 *
 */
bool HeadlessScreen::setColor(std::string color) {
    return false;
}

/**
 *
 */
void HeadlessScreen::deactivate() {
    s_atomic_lock(&lock);
    memcpy(outputBuffer, (uint8_t*)VIDEO_MEMORY, SCREEN_HEIGHT * SCREEN_WIDTH * 2);
    m_output_current = outputBuffer;
    lock             = false;
}

/**
 *
 */
void HeadlessScreen::moveCursor(uint16_t x, uint16_t y) {
    s_atomic_lock(&lock);
    uint16_t position = (y * SCREEN_WIDTH) + x;

    Utils::PortIO::write_u8(0x3D4, 0x0F);
    Utils::PortIO::write_u8(0x3D5, (uint8_t)(position & 0xFF));
    Utils::PortIO::write_u8(0x3D4, 0x0E);
    Utils::PortIO::write_u8(0x3D5, (uint8_t)((position >> 8) & 0xFF));
    lock = false;
}

/**
 *
 */
void HeadlessScreen::updateCursor() {
    moveCursor((m_offset % (SCREEN_WIDTH * 2)) / 2, m_offset / (SCREEN_WIDTH * 2));
}

/**
 *
 */
void HeadlessScreen::writeChar(char c, Graphics::Color::ArgbGradient color) {
    if ( c == '\n' ) {
        m_offset = m_offset + SCREEN_WIDTH * 2;
        m_offset = m_offset - m_offset % (SCREEN_WIDTH * 2);
    }

    else {
        m_output_current[m_offset++] = c;
        m_output_current[m_offset++] = (uint8_t)color;
    }

    normalize();
}

/**
 *
 */
void HeadlessScreen::backspace() {
    s_atomic_lock(&lock);
    m_offset                     = m_offset - 2;
    m_output_current[m_offset++] = ' ';
    ++m_offset; // keep color
    m_offset = m_offset - 2;
    lock     = false;
}

/**
 *
 */
void HeadlessScreen::cleanLine(int lineLength) {
    for ( int i = 0; i < lineLength; i++ )
        backspace();
}

/**
 *
 */
void HeadlessScreen::write(std::string message, Graphics::Color::ArgbGradient color, bool visible) {
    s_atomic_lock(&lock);
    char* p = (char*)message.c_str();

    if ( visible ) {
        while ( *p )
            writeChar(*p++, color);

        lock = false;
    }

    else {
        writeChar('*', color);

        lock = false;
    }
}

/**
 *
 */
void HeadlessScreen::normalize() {
    if ( m_offset >= SCREEN_WIDTH * SCREEN_HEIGHT * 2 ) {
        m_offset = m_offset - SCREEN_WIDTH * 2;

        uint32_t lineBytes  = SCREEN_WIDTH * 2;
        uint32_t screenSize = SCREEN_HEIGHT * lineBytes;

        memcpy(m_output_current, &m_output_current[SCREEN_WIDTH * 2], screenSize - lineBytes);

        for ( uint32_t i = 0; i < SCREEN_WIDTH * 2; i += 2 ) {
            m_output_current[screenSize - lineBytes + i]     = ' ';
            m_output_current[screenSize - lineBytes + i + 1] = (uint8_t)Graphics::Color::as_rgb(0, 0, 0);
        }
    }
}

/**
 *
 */
IO::Keyboard::Info HeadlessScreen::readInput(bool* cancelCondition) {
    return IO::Keyboard::instance().read(cancelCondition);
}
