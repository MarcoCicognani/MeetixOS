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

#include <Api.h>
#include <GUI/Component/Button.hh>
#include <GUI/Properties.hh>
#include <GUI/Protocol.hh>

/**
 *
 */
Button* Button::create() {
    return Component::create<Button, UI_COMPONENT_TYPE_BUTTON>();
}

/**
 *
 */
void Button::set_enabled(bool enabled) {
    Component::set_numeric_property(UI_PROPERTY_ENABLED, enabled);
}

/**
 *
 */
bool Button::is_enabled() {
    uint32_t value;
    Component::get_numeric_property(UI_PROPERTY_ENABLED, &value);
    return value;
}
