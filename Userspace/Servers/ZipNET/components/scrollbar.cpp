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

#include <components/scrollbar.hpp>
#include <events/MouseEvent.hpp>
#include <stdio.h>

/**
 *
 */
void Scrollbar_t::paint() {
    bounds = getBounds();
    cr     = graphics.getContext();
    clearSurface();

    knob = calculateKnob();

    // knob
    cairo_rectangle(cr, knob.x, knob.y, knob.width, knob.height);
    cairo_set_source_rgba(cr, 0, 0, 0, 0.5);
    cairo_fill(cr);
}

/**
 *
 */
bool Scrollbar_t::handle(Event_t& event) {
    MouseEvent_t* mouseEvent = dynamic_cast<MouseEvent_t*>(&event);
    if ( mouseEvent ) {
        if ( mouseEvent->type == MOUSE_EVENT_ENTER ) {
            markFor(COMPONENT_REQUIREMENT_PAINT);

        }

        else if ( mouseEvent->type == MOUSE_EVENT_PRESS ) {
            Rectangle knob = calculateKnob();
            if ( knob.contains(mouseEvent->position) ) {
                if ( orientation == ScrollbarOrientation_t::VERTICAL ) {
                    dragPressPosition = mouseEvent->position.y;
                    dragViewPosition  = knob.y;
                }

                else if ( orientation == ScrollbarOrientation_t::HORIZONTAL ) {
                    dragPressPosition = mouseEvent->position.x;
                    dragViewPosition  = knob.x;
                }
            }
            markFor(COMPONENT_REQUIREMENT_PAINT);

        }

        else if ( mouseEvent->type == MOUSE_EVENT_DRAG ) {
            int mousePosition;
            if ( orientation == ScrollbarOrientation_t::VERTICAL ) {
                mousePosition = mouseEvent->position.y;
            }

            else if ( orientation == ScrollbarOrientation_t::HORIZONTAL ) {
                mousePosition = mouseEvent->position.x;
            }

            int viewPosition = dragViewPosition + (mousePosition - dragPressPosition);
            int viewMax      = getViewMax();
            if ( viewPosition < 0 ) {
                viewPosition = 0;
            }

            else if ( viewPosition > viewMax ) {
                viewPosition = viewMax;
            }

            if ( viewMax == 0 ) {
                viewMax = 1;
            }
            int modelMax  = modelTotalArea - modelVisibleArea;
            modelPosition = (viewPosition * modelMax) / viewMax;
            if ( modelPosition < 0 ) {
                modelPosition = 0;
            }

            else if ( modelPosition > modelMax ) {
                modelPosition = modelMax;
            }

            if ( scrollHandler ) {
                scrollHandler->handleScroll(this);
            }

            markFor(COMPONENT_REQUIREMENT_PAINT);
        }
        return true;
    }

    return false;
}

/**
 *
 */
void Scrollbar_t::setModelArea(int visible, int total) {
    modelVisibleArea = visible;
    modelTotalArea   = total;

    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void Scrollbar_t::setModelPosition(int pos) {
    modelPosition = pos;

    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
Rectangle Scrollbar_t::calculateKnob() {
    Rectangle bounds = getBounds();

    // calculate knob size
    int knobSize = getKnobSize();

    // calculate area that knob can scroll on
    int viewMax  = getViewMax();
    int modelMax = modelTotalArea - modelVisibleArea;
    if ( modelMax == 0 ) {
        modelMax = 1;
    }
    int viewPosition = (viewMax * modelPosition) / modelMax;

    // create rectangle for knob
    if ( orientation == ScrollbarOrientation_t::VERTICAL )
        return Rectangle(0, viewPosition, bounds.width, knobSize);

    else if ( orientation == ScrollbarOrientation_t::HORIZONTAL )
        return Rectangle(viewPosition, 0, knobSize, bounds.height);

    // dummy
    return Rectangle(0, 0, 1, 1);
}

/**
 *
 */
int Scrollbar_t::getViewMax() {
    if ( orientation == ScrollbarOrientation_t::VERTICAL )
        return getBounds().height - getKnobSize();

    else if ( orientation == ScrollbarOrientation_t::HORIZONTAL )
        return getBounds().width - getKnobSize();

    return 0;
}

/**
 *
 */
int Scrollbar_t::getKnobSize() {
    int bounds;
    if ( orientation == ScrollbarOrientation_t::VERTICAL )
        bounds = getBounds().height;

    else if ( orientation == ScrollbarOrientation_t::HORIZONTAL )
        bounds = getBounds().width;

    int size = (bounds * modelVisibleArea) / modelTotalArea;
    if ( size < 20 )
        size = 20;

    return size;
}
