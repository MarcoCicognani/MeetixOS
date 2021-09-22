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
#include <gui/properties.hpp>
#include <gui/textfield.hpp>
#include <gui/uispech.hpp>

/**
 *
 */
Textfield* Textfield::create() {
    return createComponent<Textfield, UI_COMPONENT_TYPE_TEXTFIELD>();
}

/**
 *
 */
void Textfield::setSecure(bool secure) {
    Component::setNumericProperty(UI_PROPERTY_SECURE, secure);
}

/**
 *
 */
bool Textfield::isSecure() {
    uint32_t value;
    Component::getNumericProperty(UI_PROPERTY_SECURE, &value);
    return value;
}
