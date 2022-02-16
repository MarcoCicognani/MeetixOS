/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#include "geoshape.hpp"

#include <cairo/cairo.h>
#include <components/cursor.hpp>
#include <events/FocusEvent.hpp>
#include <events/KeyEvent.hpp>
#include <events/MouseEvent.hpp>
#include <LibGraphics/Color.hh>
#include <LibGraphics/Text/FontManager.hh>
#include <LibGUI/Properties.hh>
#include <math.h>
#include <zipNET.hpp>

/**
 *
 */
Geoshape_t::Geoshape_t() {
    resizable  = false;
    pathToLoad = false;
    shapeColor = Graphics::Color::as_argb(10, 200, 200, 200);
    type       = UI_COMPONENT_TYPE_GEOSHAPE;

    Component_t::addChild(&panel);
    Component_t::addChild(&label);
}

/**
 *
 */
void Geoshape_t::addChild(Component_t* component) {
    panel.addChild(component);
}

/**
 *
 */
void Geoshape_t::layout() {
    auto layoutBounds = getBounds();

    label.setBounds(Graphics::Metrics::Rectangle(6, 3, layoutBounds.width() - 30, 25));

    panel.setBounds(Graphics::Metrics::Rectangle(0, 0, layoutBounds.width(), layoutBounds.height()));
    panel.setBackground(Graphics::Color::as_argb(0, 0, 0, 0));
}

/**
 *
 */
void Geoshape_t::paint() {
    geoshapeSurface = graphics.cairo_context();
    geoshapeBounds  = getBounds();
    clearSurface();

    cairo_set_source_rgba(geoshapeSurface, ARGB_TO_CAIRO_PARAMS(shapeColor));
    cairo_rectangle(geoshapeSurface, 0, 0, geoshapeBounds.width(), geoshapeBounds.height());
    cairo_fill(geoshapeSurface);

    if ( pathToLoad ) {
        pngSurface = graphics.cairo_context();

        cairo_set_source_surface(pngSurface, png, pngPosition.x(), pngPosition.y());
        cairo_paint(pngSurface);
    }
}

/**
 *
 */
bool Geoshape_t::handle(Event_t& event) {
    if ( Component_t::handle(event) )
        return true;

    markFor(COMPONENT_REQUIREMENT_PAINT);
    return false;
}

/**
 *
 */
void Geoshape_t::setTitle(std::string title) {
    label.setTitle(title);
}

/**
 *
 */
std::string Geoshape_t::getTitle() {
    return label.getTitle();
}

/*
 *
 */
void Geoshape_t::setTitleFont(std::string fontName) {
    label.setTitleFont(fontName);
}

/*
 *
 */
void Geoshape_t::setTitleAlignment(Graphics::Text::Alignment alignment) {
    label.setTitleAlignment(alignment);
}

/**
 *
 */
void Geoshape_t::setPNG(std::string path, Graphics::Metrics::Point positon) {
    pathToLoad  = true;
    pngPosition = positon;

    png = cairo_image_surface_create_from_png(path.c_str());

    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void Geoshape_t::PngAnimation(std::string              path,
                              Graphics::Metrics::Point PNGstartAnimation,
                              Graphics::Metrics::Point PNGendAnimation,
                              size_t                   sleep) {
}

/**
 *
 */
void Geoshape_t::setColor(Graphics::Color::ArgbGradient color, Graphics::Color::ArgbGradient tltColor) {
    shapeColor = color;

    label.setFontColor(tltColor);
    markFor(COMPONENT_REQUIREMENT_PAINT);
}
