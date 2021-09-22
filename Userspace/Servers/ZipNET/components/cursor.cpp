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

#include <Api.h>
#include <components/cursor.hpp>
#include <events/MouseEvent.hpp>
#include <utils/fparser.hpp>
#include <utils/utils.hpp>
#include <zipNET.hpp>

Cursor& Cursor::instance() {
    static Cursor* s_cursor{ nullptr };
    if ( !s_cursor ) {
        s_cursor = new Cursor{};
    }

    return *s_cursor;
}

/**
 *
 */
bool Cursor::set(std::string name) {
    bool success = true;

    if ( cursorConfigurations.count(name) > 0 )
        currentConfiguration = &cursorConfigurations[name];
    else if ( cursorConfigurations.count("default") > 0 )
        currentConfiguration = &cursorConfigurations["default"];
    else
        success = false;

    screen = ZipNET::instance()->screen;
    if ( screen )
        screen->markDirty(getArea());

    return success;
}

/**
 *
 */
bool Cursor::load(const std::string& cursor_path) {
    // Open config file
    ifstream in(cursor_path + "/Cursor.cfg");
    if ( !in.good() )
        return false;

    PropertyFileParser properties(in);
    auto               content = properties.getProperties();

    // Read required params
    auto name      = content["name"];
    auto hitpointX = content["hitpoint.x"];
    auto hitpointY = content["hitpoint.y"];
    auto image     = content["image"];

    if ( name.empty() || hitpointX.empty() || hitpointY.empty() || image.empty() )
        return false;

    // Convert hitpoint
    std::stringstream stx;
    stx << hitpointX;
    int hPointX;
    stx >> hPointX;

    std::stringstream sty;
    sty << hitpointY;
    int hPointY;
    sty >> hPointY;

    auto cursor_image_path = cursor_path + "/" + image;

    // check if file exists
    auto image_fd = s_open(cursor_image_path.c_str());
    if ( image_fd == FD_NONE ) {
        return false;
    }
    s_close(image_fd);

    // load cursor
    CursorConfiguration cursor_config;
    cursor_config.surface = cairo_image_surface_create_from_png(cursor_image_path.c_str());
    if ( cursor_config.surface == nullptr ) {
        std::stringstream out;
        out << "failed to load cursor image at " << cursor_image_path << " for configuration "
            << cursor_path;
        Utils::log(out.str());

        return false;
    }

    cursor_config.hitpoint     = Point(hPointX, hPointY);
    cursor_config.size         = Dimension(cairo_image_surface_get_width(cursor_config.surface),
                                           cairo_image_surface_get_height(cursor_config.surface));
    cursorConfigurations[name] = cursor_config;
    return true;
}

/**
 *
 */
void Cursor::paint(Graphics* global) {
    cr = global->getContext();
    cairo_reset_clip(cr);

    if ( currentConfiguration ) {
        // draw cursor image
        cairo_set_source_surface(cr,
                                 currentConfiguration->surface,
                                 position.x - currentConfiguration->hitpoint.x,
                                 position.y - currentConfiguration->hitpoint.y);
        cairo_paint(cr);
    } else {
        // draw fallback cursor
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, position.x, position.y, FALLBACK_CURSOR_SIZE, FALLBACK_CURSOR_SIZE);
        cairo_fill(cr);
    }
}

/**
 *
 */
Rectangle Cursor::getArea() {
    // get area for current cursor
    if ( currentConfiguration )
        return Rectangle(position.x - currentConfiguration->hitpoint.x,
                         position.y - currentConfiguration->hitpoint.y,
                         currentConfiguration->size.width,
                         currentConfiguration->size.height);

    // fallback cursor is just a square
    return { position.x, position.y, FALLBACK_CURSOR_SIZE, FALLBACK_CURSOR_SIZE };
}
