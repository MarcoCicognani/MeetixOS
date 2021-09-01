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

#ifndef __SCROLLBAR__
#define __SCROLLBAR__

#include <components/ButtonState.hpp>
#include <components/component.hpp>

class Scrollbar_t;

/**
 *
 */
enum class ScrollbarOrientation_t : uint8_t
{
	VERTICAL,
	HORIZONTAL
};

/**
 *
 */
class ScrollHandler_t
{
public:
	virtual ~ScrollHandler_t() {}
	virtual void handleScroll(Scrollbar_t *bar) = 0;
};

/**
 *
 */
class Scrollbar_t: public Component_t
{
private:
	ScrollbarOrientation_t orientation;

	int modelPosition;
	int modelVisibleArea;
	int modelTotalArea;

	ButtonState_t decButtonState;
	ButtonState_t incButtonState;

	bool knobDrag;
	int dragPressPosition;
	int dragViewPosition;

	ScrollHandler_t *scrollHandler;

	cairo_t *cr;
	Rectangle bounds;
	Rectangle knob;

public:
	Scrollbar_t(ScrollbarOrientation_t orientation) : orientation(orientation), modelPosition(0), modelVisibleArea(0), modelTotalArea(0), knobDrag(false), dragPressPosition(0), dragViewPosition(0), scrollHandler(0) {}

	/*
	 * component
	 */
	virtual void paint();
	virtual bool handle(Event_t &event);

	virtual void setScrollHandler(ScrollHandler_t *handler) { scrollHandler = handler; }
	virtual ScrollHandler_t *gettScrollHandler() { return scrollHandler; }

	void setModelArea(int visibleArea, int totalArea);
	int getModelVisibleArea() const { return modelVisibleArea; }
	int getModelTotalArea() const { return modelTotalArea; }
	void setModelPosition(int position);
	int getModelPosition() const { return modelPosition; }
	int getViewMax();
	int getKnobSize();

	Rectangle calculateKnob();

};

#endif
