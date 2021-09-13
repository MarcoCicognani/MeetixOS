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

#ifndef MEETIX_LIBRARY_UI_ACTION_COMPONENT
#define MEETIX_LIBRARY_UI_ACTION_COMPONENT

#include <cstdint>
#include <eva.h>
#include <gui/actionlistener.hpp>
#include <gui/uispech.hpp>

/**
 * Component that is capable of receiving action events
 */
class ActionComponent {
private:
    UiComponentID id;

protected:
    Component* self;

    ActionComponent(Component* self, UiComponentID id) : self(self), id(id) {
    }

public:
    /**
     *
     */
    virtual ~ActionComponent() {
    }

    /**
     *
     */
    bool setActionListener(ActionListener* l);
};

#endif
