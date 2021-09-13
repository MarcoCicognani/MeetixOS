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

#ifndef MEETIX_LIBRARY_UI_MOUSELISTENER
#define MEETIX_LIBRARY_UI_MOUSELISTENER

#include <cstdint>
#include <gui/listener.hpp>
#include <gui/uispech.hpp>

/**
 *
 */
class MouseListener : public Listener {
public:
    virtual ~MouseListener() {
    }

    virtual void process(UiComponentEventHeader* header) {
        UiComponentMouseEvent* event = (UiComponentMouseEvent*)header;
        handleMouseEvent(event);
    }

    virtual void handleMouseEvent(UiComponentMouseEvent* e) = 0;
};

#endif
