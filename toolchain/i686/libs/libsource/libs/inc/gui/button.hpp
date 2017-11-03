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

#ifndef MEETIX_LIBRARY_UI_BUTTON
#define MEETIX_LIBRARY_UI_BUTTON

#include <gui/actioncomponent.hpp>
#include <gui/component.hpp>
#include <gui/titledcomponent.hpp>
#include <gui/pngcomponent.hpp>
#include <gui/coloredcomponent.hpp>
#include <cstdint>

/**
 *
 */
class Button : public Component, public TitledComponent, public ActionComponent, public PngComponent, public ColoredComponent
{
protected:
	/*
	 *
	 */
	Button(uint32_t id) : Component(id,UI_COMPONENT_TYPE_BUTTON), TitledComponent(id), ActionComponent(this, id), PngComponent(id), ColoredComponent(id) {}

public:
	virtual ~Button() {}

	/*
	 *
	 */
	static Button *create();

	/*
	 *
	 */
	void setEnabled(bool enabled);

	/*
	 *
	 */
	bool isEnabled();
};

#endif
