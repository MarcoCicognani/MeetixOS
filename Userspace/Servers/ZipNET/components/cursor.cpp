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

#include <components/cursor.hpp>
#include <eva.h>
#include <events/MouseEvent.hpp>
#include <utils/fparser.hpp>
#include <utils/utils.hpp>
#include <zipNET.hpp>

using namespace std;

static map<string, CursorConfiguration> cursorConfigurations;
static CursorConfiguration*             currentConfiguration = 0;

static cairo_t*  cr;
static Screen_t* screen;

Point Cursor_t::position;
Point Cursor_t::nextPosition;

MouseButton  Cursor_t::pressedButtons     = MOUSE_EVENT_NONE;
MouseButton  Cursor_t::nextPressedButtons = MOUSE_EVENT_NONE;
Component_t* Cursor_t::draggedComponent   = 0;
Component_t* Cursor_t::hoveredComponent   = 0;
Component_t* Cursor_t::focusedComponent   = 0;

/**
 *
 */
bool Cursor_t::set(string name) {
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
bool Cursor_t::load(string cursorPath) {
    // Open config file
    ifstream in(cursorPath + "/cursor.cfg");
    if ( !in.good() )
        return false;

    PropertyFileParser  properties(in);
    map<string, string> content = properties.getProperties();

    // Read required params
    string name      = content["name"];
    string hitpointX = content["hitpoint.x"];
    string hitpointY = content["hitpoint.y"];
    string image     = content["image"];

    if ( name.empty() || hitpointX.empty() || hitpointY.empty() || image.empty() )
        return false;

    // Convert hitpoint
    stringstream stx;
    stx << hitpointX;
    int hPointX;
    stx >> hPointX;

    stringstream sty;
    sty << hitpointY;
    int hPointY;
    sty >> hPointY;

    string cursorImagePath = (cursorPath + "/" + image);

    // check if file exists
    File_t cursorImageFile = Open(cursorImagePath.c_str());
    if ( cursorImageFile == FD_NONE ) {
        return false;
    }
    Close(cursorImageFile);

    // load cursor
    CursorConfiguration pack;
    pack.surface = cairo_image_surface_create_from_png(cursorImagePath.c_str());
    if ( pack.surface == nullptr ) {
        stringstream out;
        out << "failed to load cursor image at " << cursorImagePath << " for configuration "
            << cursorPath;
        Utils::log(out.str());

        return false;
    }

    pack.hitpoint              = Point(hPointX, hPointY);
    pack.size                  = Dimension(cairo_image_surface_get_width(pack.surface),
                                           cairo_image_surface_get_height(pack.surface));
    cursorConfigurations[name] = pack;

    return true;
}

/**
 *
 */
void Cursor_t::paint(Graphics* global) {
    cr = global->getContext();
    cairo_reset_clip(cr);

    if ( currentConfiguration ) {
        // draw cursor image
        cairo_set_source_surface(cr,
                                 currentConfiguration->surface,
                                 position.x - currentConfiguration->hitpoint.x,
                                 position.y - currentConfiguration->hitpoint.y);
        cairo_paint(cr);
    }

    else {
        // draw fallback cursor
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, position.x, position.y, FALLBACK_CURSOR_SIZE, FALLBACK_CURSOR_SIZE);
        cairo_fill(cr);
    }
}

/**
 *
 */
Rectangle Cursor_t::getArea() {
    // get area for current cursor
    if ( currentConfiguration )
        return Rectangle(position.x - currentConfiguration->hitpoint.x,
                         position.y - currentConfiguration->hitpoint.y,
                         currentConfiguration->size.width,
                         currentConfiguration->size.height);

    // fallback cursor is just a square
    return Rectangle(position.x, position.y, FALLBACK_CURSOR_SIZE, FALLBACK_CURSOR_SIZE);
}
