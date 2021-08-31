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

#ifndef __SCROLLPANE__
#define __SCROLLPANE__

#include <components/component.hpp>
#include <components/scrollbar.hpp>
#include <components/panel.hpp>

/**
 *
 */
class Scrollpane_t: public Component_t, public ScrollHandler_t
{
private:
	Component_t *viewPort;
	Point scrollPosition;
	Scrollbar_t verticalScrollbar;
	Scrollbar_t horizontalScrollbar;

public:
	Scrollpane_t() : scrollPosition(Point(0, 0)), viewPort(nullptr), verticalScrollbar(ScrollbarOrientation_t::VERTICAL), horizontalScrollbar(ScrollbarOrientation_t::HORIZONTAL) {}

	virtual Point getPosition() const { return scrollPosition; }
	virtual void layout();
	virtual void setPosition(Point &position);
	virtual void setViewPort(Component_t *content);
	virtual Component_t *getViewPort() const { return viewPort; }
	virtual void handleScroll(Scrollbar_t *bar);

};

#endif
