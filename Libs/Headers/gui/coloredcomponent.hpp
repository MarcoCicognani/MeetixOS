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

#ifndef MEETIX_LIBRARY_UI_COLOR
#define MEETIX_LIBRARY_UI_COLOR

#include <Api.h>
#include <cstdint>
#include <Graphics/Color.hh>
#include <gui/component.hpp>
#include <gui/ui.hpp>
#include <gui/uispech.hpp>

/*
 *
 */
class ColoredComponent {
private:
    uint32_t id;

protected:
    ColoredComponent(uint32_t id) : id(id) {
    }

public:
    virtual bool setColor(Graphics::Color::ArgbGradient shapeColor,
                          Graphics::Color::ArgbGradient titleColor);
};

#endif
