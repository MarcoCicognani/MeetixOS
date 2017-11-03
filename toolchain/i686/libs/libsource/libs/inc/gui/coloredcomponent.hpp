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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#ifndef MEETIX_LIBRARY_UI_COLOR
#define MEETIX_LIBRARY_UI_COLOR

#include <eva.h>
#include <cstdint>
#include <gui/uispech.hpp>
#include <graphics/color.hpp>
#include <gui/component.hpp>
#include <gui/ui.hpp>

/*
 *
 */
class ColoredComponent
{
private:
	uint32_t id;

protected:
	ColoredComponent(uint32_t id) : id(id) {}

public:
	virtual bool setColor(Color_t shapeColor, Color_t titleColor);
};

#endif
