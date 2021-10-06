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
#include <layout/FlowLayoutManager.hpp>
#include <typeinfo>
#include <vector>

/**
 *
 */
void FlowLayoutManager_t::layout() {
    if ( !component )
        return;

    std::vector<Component_t*>& children = component->getChildren();

    int x          = 0;
    int y          = 0;
    int lineHeight = 0;

    auto parentBounds = component->getBounds();
    for ( Component_t* c : children ) {
        auto preferredSize = c->getPreferredSize();

        if ( x + preferredSize.width() > parentBounds.width() ) {
            x = 0;
            y += lineHeight;
            lineHeight = 0;
        }

        c->setBounds({ x, y, preferredSize.width(), preferredSize.height() });
        x += preferredSize.width();

        if ( preferredSize.height() > lineHeight )
            lineHeight = preferredSize.height();
    }
}
