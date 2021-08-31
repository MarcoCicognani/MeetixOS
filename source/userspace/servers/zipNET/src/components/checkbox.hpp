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

#ifndef __CHECKBOX__
#define __CHECKBOX__

#include <components/component.hpp>
#include <components/label.hpp>
#include <components/ColoredComponent.hpp>

#define DEFAULT_BOX_SIZE		20
#define DEFAULT_BOX_TEXT_GAP	5

/**
 *
 */
class Checkbox_t: public Component_t, public ColoredComponent_t
{
private:
	Label_t label;
	bool checked;
	int boxSize;
	int boxTextGap;

	bool hovered;
	bool pressed;

	cairo_t *surface;
	cairo_t *microSurface;

	Rectangle bounds;

public:
	Checkbox_t();

	/*
	 * Components
	 */
	virtual void layout();
	virtual void paint();
	virtual bool handle(Event_t& e);
	virtual void handleBoundChange(Rectangle oldBounds);

	/*
	 * colored components
	 */
	virtual void setColor(Color_t color, Color_t tltColor) { shapeColor = color; }

	Label_t &getLabel() { return label; }
};

#endif
