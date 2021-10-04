/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef MEETIX_LIBRARY_UI_KEYLISTENER
#define MEETIX_LIBRARY_UI_KEYLISTENER

#include <cstdint>
#include <gui/listener.hpp>
#include <IO/Keyboard.hh>

/**
 *
 */
struct KeyEvent {
    IO::Keyboard::InfoBasic info;
};

/**
 *
 */
class KeyListener : public Listener {
public:
    virtual ~KeyListener() {
    }

    virtual void process(UiComponentEventHeader* header) {
        UiComponentKeyEvent* event = (UiComponentKeyEvent*)header;

        KeyEvent e;
        e.info = event->keyInfo;
        handleKeyEvent(e);
    }

    virtual void handleKeyEvent(KeyEvent& e) = 0;
};

#endif
