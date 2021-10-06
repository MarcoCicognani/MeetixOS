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

#include <components/checkbox.hpp>
#include <events/MouseEvent.hpp>

/**
 *
 */
Checkbox_t::Checkbox_t()
    : Component_t(true), checked(false), boxSize(DEFAULT_BOX_SIZE),
      boxTextGap(DEFAULT_BOX_TEXT_GAP), hovered(false), pressed(false) {
    addChild(&label);
}

/**
 *
 */
void Checkbox_t::layout() {
    auto preferredSize = label.getPreferredSize();
    if ( preferredSize.height() < boxSize + boxTextGap ) {
        preferredSize.set_height(boxSize + boxTextGap);
    }
    preferredSize.set_width(preferredSize.width() + preferredSize.height());
    setPreferredSize(preferredSize);
}

/**
 *
 */
void Checkbox_t::paint() {
    bounds  = getBounds();
    surface = graphics.cairo_context();
    clearSurface();

    if ( hovered )
        cairo_set_source_rgba(surface,
                              ARGB_TO_CAIRO_PARAMS(Graphics::Color::as_argb(255, 200, 200, 200)));

    else
        cairo_set_source_rgba(surface, ARGB_TO_CAIRO_PARAMS(shapeColor));

    cairo_rectangle(surface, bounds.x(), bounds.y(), bounds.width(), bounds.height());
    cairo_fill(surface);

    if ( checked ) {
        microSurface = graphics.cairo_context();

        cairo_set_source_rgba(microSurface,
                              ARGB_TO_CAIRO_PARAMS(Graphics::Color::as_argb(255, 0, 200, 0)));
        cairo_rectangle(microSurface,
                        bounds.x() + 2,
                        bounds.y() + 2,
                        bounds.width() - 4,
                        bounds.height() - 4);
    }
}

/**
 *
 */
bool Checkbox_t::handle(Event_t& event) {
    MouseEvent_t* mouseEvent = dynamic_cast<MouseEvent_t*>(&event);
    if ( mouseEvent ) {
        if ( mouseEvent->type == MOUSE_EVENT_ENTER ) {
            hovered = true;
            markFor(COMPONENT_REQUIREMENT_PAINT);

        }

        else if ( mouseEvent->type == MOUSE_EVENT_LEAVE ) {
            hovered = false;
            markFor(COMPONENT_REQUIREMENT_PAINT);

        }

        else if ( mouseEvent->type == MOUSE_EVENT_PRESS ) {
            pressed = true;
            markFor(COMPONENT_REQUIREMENT_PAINT);

        }

        else if ( mouseEvent->type == MOUSE_EVENT_RELEASE
                  || mouseEvent->type == MOUSE_EVENT_DRAG_RELEASE ) {
            pressed = false;

            auto minbounds = getBounds();
            minbounds.set_left(0);
            minbounds.set_top(0);
            if ( mouseEvent->type == MOUSE_EVENT_RELEASE
                 && minbounds.contains(mouseEvent->position) ) {
                checked = !checked;
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
void Checkbox_t::handleBoundChange(Graphics::Metrics::Rectangle oldBounds) {
    auto unpositioned = getBounds();
    unpositioned.set_left(boxSize + boxTextGap);
    unpositioned.set_top(0);
    this->label.setBounds(unpositioned);
}
