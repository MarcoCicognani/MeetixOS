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

#include <cairo/cairo.h>
#include <components/label.hpp>
#include <events/MouseEvent.hpp>
#include <graphics/text/fontldr.hpp>

/**
 *
 */
Label_t::Label_t() : Component_t(true), alignment(TextAlignment::LEFT) {
    type = UI_COMPONENT_TYPE_LABEL;
    setFont(FontLoader::get("default"));
    shapeColor = RGB(0, 0, 0);
}

/*
 *
 */
void Label_t::setFontSize(int size) {
    fontSize = size;
}

/**
 *
 */
void Label_t::setFont(Font_t* newFont) {
    font     = newFont;
    fontSize = 14;
}

/**
 *
 */
void Label_t::setTitleFont(std::string fontName) {
    setFont(FontLoader::get(fontName));
    markFor(COMPONENT_REQUIREMENT_UPDATE);
}

/*
 *
 */
void Label_t::setTitleAlignment(TextAlignment alignment) {
    this->alignment = alignment;
    markFor(COMPONENT_REQUIREMENT_ALL);
}

/**
 *
 */
void Label_t::update() {
    Rectangle thisBounds(0, 0, getBounds().width, getBounds().height);

    // Check if the component was ever layouted, otherwise set to a high value
    if ( !thisBounds.width && !thisBounds.height ) {
        thisBounds.width  = 9999;
        thisBounds.height = 9999;
    }

    // get text bounds
    cr = graphics.getContext();
    cairo_set_font_face(cr, font->getFace());
    cairo_set_font_size(cr, fontSize);
    cairo_text_extents(cr, this->text.c_str(), &lastExtents);
    Dimension newPreferred(lastExtents.width + 3, lastExtents.height + 3);

    // Set new preferred size
    if ( getPreferredSize() != newPreferred )
        setPreferredSize(newPreferred);
    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void Label_t::paint() {
    clearSurface();

    cr     = graphics.getContext();
    bounds = getBounds();

    cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(shapeColor));

    int textLeft;
    int textBot = (bounds.height / 2 - lastExtents.height / 2) + lastExtents.height;

    if ( alignment == TextAlignment::CENTER )
        textLeft = bounds.width / 2 - lastExtents.width / 2;
    else if ( alignment == TextAlignment::RIGHT )
        textLeft = bounds.width - lastExtents.width;
    else
        textLeft = 0;

    cairo_move_to(cr, textLeft, textBot);
    cairo_set_font_face(cr, font->getFace());
    cairo_set_font_size(cr, fontSize);
    cairo_show_text(cr, text.c_str());
}

/**
 *
 */
bool Label_t::handle(Event_t& e) {
    return false;
}

/**
 *
 */
void Label_t::setTitle(std::string newText) {
    text = newText;
    markFor(COMPONENT_REQUIREMENT_UPDATE);
}

/**
 *
 */
std::string Label_t::getTitle() {
    return text;
}

/**
 *
 */
void Label_t::setFontColor(Color_t ftcol) {
    setColor(0, ftcol);
    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void Label_t::setAlignment(TextAlignment newAlignment) {
    alignment = newAlignment;
    markFor(COMPONENT_REQUIREMENT_UPDATE);
}

/**
 *
 */
TextAlignment Label_t::getAlignment() {
    return alignment;
}
