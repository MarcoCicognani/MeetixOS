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

#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include <components/component.hpp>
#include <Graphics/Metrics/Rectangle.hh>

/**
 *
 */
class Screen_t : public Component_t {
private:
    /**
     * Area that is invalid and needs to be copied to the video output.
     */
    Graphics::Metrics::Rectangle invalid;

public:
    /**
     *
     */
    virtual ~Screen_t() {
    }

    /**
     * Overrides the default invalidation method. On the component, this method
     * just dispatches to the parent, but here we must remember the invalidation.
     */
    virtual void markDirty(Graphics::Metrics::Rectangle rect);

    /**
     *
     */
    Graphics::Metrics::Rectangle grabInvalid() {
        Graphics::Metrics::Rectangle ret = invalid;
        invalid                          = Graphics::Metrics::Rectangle();
        return ret;
    }
};

#endif
