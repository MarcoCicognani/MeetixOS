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

#include <components/component.hpp>
#include <layout/GridLayoutManager.hpp>
#include <typeinfo>
#include <vector>

/**
 *
 */
GridLayoutManager_t::GridLayoutManager_t(int columns, int rows)
    : columns(columns), rows(rows), padding(Graphics::Metrics::Insets(0, 0, 0, 0)), horizontalCellSpace(0),
      verticalCellSpace(0) {
}

/**
 *
 */
void GridLayoutManager_t::layout() {
    if ( !component )
        return;

    std::vector<Component_t*>& children = component->getChildren();

    auto usedBounds = component->getBounds();
    usedBounds.set_left(padding.left());
    usedBounds.set_top(padding.top());
    usedBounds.set_width(usedBounds.width() - (padding.left() + padding.right()));
    usedBounds.set_height(usedBounds.height() - (padding.top() + padding.bottom()));

    int x         = usedBounds.x();
    int y         = usedBounds.y();
    int rowHeight = 0;

    int widthPerComponent = (columns > 0) ? (usedBounds.width() / columns) : usedBounds.width();

    for ( Component_t* c : children ) {
        int usedHeight = (rows > 0) ? (usedBounds.height() / rows) : c->getPreferredSize().height();

        if ( x + widthPerComponent > usedBounds.width() ) {
            x = usedBounds.x();
            y += rowHeight;
            rowHeight = 0;
        }

        c->setBounds({ x, y, widthPerComponent, usedHeight });
        x += widthPerComponent;

        if ( usedHeight > rowHeight )
            rowHeight = usedHeight;
    }
}
