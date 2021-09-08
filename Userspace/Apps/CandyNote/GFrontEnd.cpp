/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#include "GFrontEnd.hpp"

#include <io/keyboard.hpp>
#include <tasking/lock.hpp>

Window*    window;
Textfield* textBox;
Button*    save;

GraphicFrontEnd_t::GraphicFrontEnd_t() {
    initialize();
}

void GraphicFrontEnd_t::initialize() {
    Dimension resolution = UI::getResolution();

    window = Window::create();
    window->setBounds(Rectangle(resolution.width / 2 - 200, resolution.height / 2 - 300, 400, 600));
    window->setTitle("CandyNote");
    window->setVisible(true);

    window->onClose([] {
        UI::close();
        Kill(GetTid());
    });

    textBox = Textfield::create();
    textBox->setBounds(Rectangle(0, 20, 400, 570));
}

void GraphicFrontEnd_t::paintEntry() {
    this->paintLoop();
}

void GraphicFrontEnd_t::paintLoop() {
}

void GraphicFrontEnd_t::clean() {
}

void GraphicFrontEnd_t::end() {
}

void GraphicFrontEnd_t::write(string message, string title) {
}

Keyboard::Info GraphicFrontEnd_t::readInput() {
    return { };
}
