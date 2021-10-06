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

#include <components/scrollpane.hpp>
#include <events/MouseEvent.hpp>
#include <stdio.h>

/**
 *
 */
void Scrollpane_t::setViewPort(Component_t* component) {
    viewPort = component;
    addChild(component);

    addChild(&horizontalScrollbar);
    horizontalScrollbar.setScrollHandler(this);

    addChild(&verticalScrollbar);
    verticalScrollbar.setScrollHandler(this);
}

/**
 *
 */
void Scrollpane_t::layout() {
    auto bounds = getBounds();

    verticalScrollbar.setModelArea(bounds.height(), viewPort->getBounds().height());
    verticalScrollbar.setBounds(
        Graphics::Metrics::Rectangle(bounds.width() - 15, 0, 15, bounds.height() - 15));

    horizontalScrollbar.setModelArea(bounds.width(), viewPort->getBounds().width());
    horizontalScrollbar.setBounds(
        Graphics::Metrics::Rectangle(0, bounds.height() - 15, bounds.width() - 15, 15));
}

/**
 *
 */
void Scrollpane_t::setPosition(Graphics::Metrics::Point& newPosition) {
    if ( viewPort != nullptr ) {
        scrollPosition = newPosition;

        auto viewPortSize = viewPort->getBounds();
        auto bounds       = getBounds();

        // limit if too small
        if ( scrollPosition.x() > 0 )
            scrollPosition.set_x(0);
        else if ( viewPortSize.width() < bounds.width() )
            scrollPosition.set_x(0);
        else if ( scrollPosition.x() + viewPortSize.width() < bounds.width() )
            scrollPosition.set_x(bounds.width() - viewPortSize.width());

        if ( scrollPosition.y() > 0 )
            scrollPosition.set_y(0);
        else if ( viewPortSize.height() < bounds.height() )
            scrollPosition.set_y(0);
        else if ( scrollPosition.y() + viewPortSize.height() < bounds.height() )
            scrollPosition.set_y(bounds.height() - viewPortSize.height());

        viewPort->setBounds(Graphics::Metrics::Rectangle(scrollPosition.x(),
                                                         scrollPosition.y(),
                                                         viewPortSize.width(),
                                                         viewPortSize.height()));
    }
}

/**
 *
 */
void Scrollpane_t::handleScroll(Scrollbar_t* bar) {
    if ( bar == &verticalScrollbar ) {
        auto pos = scrollPosition;
        pos.set_y( -verticalScrollbar.getModelPosition());
        setPosition(pos);
    } else if ( bar == &horizontalScrollbar ) {
        auto pos = scrollPosition;
        pos.set_x( -horizontalScrollbar.getModelPosition());
        setPosition(pos);
    }
}
