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

#ifndef __CURSOR__
#define __CURSOR__

#include <cairo/cairo.h>
#include <components/component.hpp>
#include <components/screen.hpp>
#include <cstdio>
#include <events/MouseEvent.hpp>
#include <fstream>
#include <Graphics/Metrics/Point.hh>
#include <GUI/Protocol.hh>
#include <map>
#include <sstream>

#define FALLBACK_CURSOR_SIZE 10

/**
 *
 */
struct CursorConfiguration {
    cairo_surface_t*             surface;
    Graphics::Metrics::Point     hitpoint;
    Graphics::Metrics::Dimension size;
};

/**
 *
 */
class Cursor {
public:
    static Cursor& instance();

    Graphics::Metrics::Point position{ 0, 0 };
    Graphics::Metrics::Point nextPosition{ 0, 0 };
    MouseButton              pressedButtons{ MOUSE_BUTTON_NONE };
    MouseButton              nextPressedButtons{ MOUSE_BUTTON_NONE };

    Component_t* draggedComponent{ nullptr };
    Component_t* hoveredComponent{ nullptr };
    Component_t* focusedComponent{ nullptr };

    /**
     *
     */
    void paint(Graphics::Context* global);

    /**
     *
     */
    Graphics::Metrics::Rectangle getArea();

    /**
     *
     */
    bool set(std::string name);

    /**
     *
     */
    bool load(const std::string& cursor_path);

private:
    Cursor() = default;

    map<string, CursorConfiguration> cursorConfigurations{};
    CursorConfiguration*             currentConfiguration{ nullptr };

    cairo_t*  cr{ nullptr };
    Screen_t* screen{ nullptr };
};

#endif
