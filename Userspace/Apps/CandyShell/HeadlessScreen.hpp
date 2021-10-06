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

#include "screen.hpp"

/**
 *
 */
class HeadlessScreen : public Screen {
private:
    uint32_t id;

    uint8_t* outputBuffer;
    uint8_t* outputCurrent;

    uint32_t offset;

    uint32_t activeProcessId;
    bool     lock;
    void     normalize();
    void     moveCursor(uint16_t x, uint16_t y);

public:
    HeadlessScreen();

    void clean();
    void deactivate();
    void activate();

    void close();

    bool setColor(std::string color);

    void backspace();
    void cleanLine(int lineLength);
    void write(std::string message, Graphics::Color::ArgbGradient color = Graphics::Color::as_rgb(255, 255, 255), bool visible = true);
    void writeChar(char c, Graphics::Color::ArgbGradient color = Graphics::Color::as_rgb(255, 255, 255));
    void updateCursor();

    IO::Keyboard::Info readInput(bool* cancelCondition);
};
