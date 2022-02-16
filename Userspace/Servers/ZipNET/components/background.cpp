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

#include <components/background.hpp>
#include <libgen.h>
#include <LibUtils/Environment.hh>

/**
 *
 */
Background_t::Background_t(Graphics::Metrics::Rectangle bounds) {
    setBounds(bounds);
    setZIndex(0);

    surface = cairo_image_surface_create_from_png(
        ("/MeetiX/Configs/WM/Backgrounds/" + Utils::Environment::theme() + ".png").c_str());
}

/**
 *
 */
void Background_t::paint() {
    cairo_t* cr     = graphics.cairo_context();
    auto     bounds = getBounds();

    cairo_set_source_surface(cr, surface, bounds.x(), bounds.y());
    cairo_paint(cr);
}

/**
 *
 */
bool Background_t::setBackground(const std::string& path) {
    // free memory
    cairo_surface_destroy(surface);

    // get new surface
    surface = cairo_image_surface_create_from_png(path.c_str());
    if ( surface ) {
        // set new theme
        std::string newTheme{ basename((char*)path.c_str()) };
        newTheme = newTheme.substr(0, newTheme.find('.'));
        Utils::Environment::set("THEME", newTheme);

        // paint the new background
        paint();
        return true;
    }

    return false;
}

/**
 *
 */
bool Background_t::handle(Event_t& e) {
    return true;
}
