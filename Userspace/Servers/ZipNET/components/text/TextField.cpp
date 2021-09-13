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
#include <components/text/move/DefaultCaretMoveStrategy.hpp>
#include <components/text/TextField.hpp>
#include <events/FocusEvent.hpp>
#include <events/KeyEvent.hpp>
#include <events/MouseEvent.hpp>
#include <graphics/text/fontldr.hpp>
#include <graphics/text/fontmgr.hpp>
#include <gui/properties.hpp>
#include <sstream>

/**
 *
 */
TextField_t::TextField_t()
    : cursor(0), marker(0), scrollX(0), secure(false), focused(false),
      visualStatus(TextFieldVisualStatus_t::NORMAL), alignment(TextAlignment::CENTER), fontSize(14),
      textColor(RGB(0, 0, 0)), insets(Insets(5, 5, 5, 5)), ghost(false) {
    caretMoveStrategy = DefaultCaretMoveStrategy_t::getInstance();
    type              = UI_COMPONENT_TYPE_TEXTFIELD;

    viewModel = TextLayouter::getInstance()->initializeBuffer();
    setFont(FontLoader::get("default"));
}

/**
 *
 */
TextField_t::~TextField_t() {
    delete font;
    delete viewModel;
}

/**
 *
 */
void TextField_t::setText(std::string newText) {
    text = newText;
    markFor(COMPONENT_REQUIREMENT_UPDATE);
}

/**
 *
 */
void TextField_t::update() {
    // Perform layouting
    Rectangle bounds = getBounds();

    std::string visibleText = text;

    if ( ghost )
        visibleText = ghostText;

    else if ( secure ) {
        visibleText = "";
        for ( int i = 0; i < text.length(); i++ )
            visibleText = visibleText + "*";
    }

    TextLayouter::getInstance()->layout(graphics.getContext(),
                                        visibleText.c_str(),
                                        font,
                                        fontSize,
                                        Rectangle(0, 0, bounds.width, bounds.height),
                                        alignment,
                                        viewModel,
                                        false);
    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void TextField_t::paint() {
    if ( font == 0 )
        return;

    cr     = graphics.getContext();
    bounds = getBounds();
    clearSurface();

    // background
    cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(sColor));
    cairo_rectangle(cr, 0, 0, bounds.width, bounds.height);
    cairo_fill(cr);

    // border
    Color_t borderColor;
    if ( focused )
        borderColor = ARGB(255, 0, 200, 0);

    else if ( visualStatus == TextFieldVisualStatus_t::HOVERED )
        borderColor = ARGB(255, 255, 255, 255);

    else
        borderColor = ARGB(255, 180, 180, 180);

    cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(borderColor));
    cairo_rectangle(cr, 0, 0, bounds.width, bounds.height);
    cairo_stroke(cr);

    // Scroll
    applyScroll();

    int pos    = 0;
    int first  = marker < cursor ? marker : cursor;
    int second = marker > cursor ? marker : cursor;

    // Paint marking
    if ( focused ) {
        pos = 0;
        for ( PositionedGlyph& g : viewModel->positions ) {
            Color_t color = textColor;
            if ( first != second && pos >= first && pos < second ) {
                cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(ARGB(255, 0, 200, 0)));
                Rectangle before = positionToCursorBounds(pos);
                Rectangle after  = positionToCursorBounds(pos + 1);
                cairo_rectangle(cr, before.x, before.y, after.x - before.x, before.height);
                cairo_fill(cr);

                color = ARGB(255, 255, 255, 255);
            }
            ++pos;
        }
    }

    // Paint glyphs
    pos = 0;
    for ( PositionedGlyph& g : viewModel->positions ) {
        Rectangle onView = glyphToView(g);
        Color_t   color  = textColor;

        if ( ghost )
            color = ARGB(160, 0, 0, 0);

        else if ( focused && first != second && pos >= first && pos < second )
            color = ARGB(255, 255, 255, 255);

        cairo_save(cr);
        cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(color));
        cairo_translate(cr, onView.x - g.glyph->x, onView.y - g.glyph->y); // TODO?
        cairo_glyph_path(cr, g.glyph, g.glyphCount);
        cairo_fill(cr);
        cairo_restore(cr);
        ++pos;
    }

    // Paint cursor
    if ( focused ) {
        ghost = false;

        markFor(COMPONENT_REQUIREMENT_UPDATE);
        cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(ARGB(255, 60, 60, 60)));

        Rectangle bounds = positionToCursorBounds(cursor);

        cairo_rectangle(cr, bounds.x, bounds.y, bounds.width, bounds.height);
        cairo_fill(cr);
    }

    if ( !focused && text.empty() ) {
        ghost = true;
        markFor(COMPONENT_REQUIREMENT_UPDATE);
    }
}

/**
 *
 */
void TextField_t::setColor(Color_t shapeColor, Color_t titleColor) {
    sColor    = shapeColor;
    textColor = titleColor;

    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void TextField_t::applyScroll() {
    int       cursorX = positionToUnscrolledCursorX(cursor);
    Rectangle bounds  = getBounds();

    // scroll
    if ( scrollX + cursorX > bounds.width - insets.right )
        scrollX = bounds.width - cursorX - insets.right;

    else if ( scrollX + cursorX < insets.left )
        scrollX = -cursorX + insets.left + bounds.width / 2;
}

/**
 *
 */
int TextField_t::positionToUnscrolledCursorX(int pos) {
    int cursorX        = insets.left;
    int positionsCount = viewModel->positions.size();
    for ( int i = 0; i < positionsCount; i++ ) {
        PositionedGlyph& g = viewModel->positions[i];
        // After last?
        if ( i == positionsCount - 1 && pos == positionsCount )
            cursorX = g.position.x + insets.left + g.advance.x;

        // Anywhere inside
        if ( i == pos )
            cursorX = g.position.x + insets.left - 1;
    }

    return cursorX;
}

/**
 *
 */
void TextField_t::setCursor(int pos) {
    cursor = pos;
    if ( cursor < 0 )
        cursor = 0;
    if ( cursor > text.length() )
        cursor = text.length();

    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void TextField_t::setMarker(int pos) {
    marker = pos;
    if ( marker < 0 )
        marker = 0;
    if ( marker > text.length() )
        marker = text.length();

    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void TextField_t::backspace(Keyboard::Info& info) {
    if ( text.length() > 0 ) {
        Range selected = getSelectedRange();

        int leftcut = selected.getFirst();
        if ( info.alt )
            leftcut = caretMoveStrategy->calculateSkip(text, leftcut, CaretDirection_t::LEFT);

        else if ( info.ctrl )
            leftcut = 0;

        int rightcut = selected.getLast();

        if ( rightcut - leftcut == 0 )
            leftcut--;

        if ( leftcut >= 0 && rightcut <= text.length() ) {
            std::string beforeCursor = text.substr(0, leftcut);
            std::string afterCursor  = text.substr(rightcut);
            text                     = beforeCursor + afterCursor;
            setCursor(leftcut);
            setMarker(leftcut);

            markFor(COMPONENT_REQUIREMENT_UPDATE);
        }
    }
}

/**
 *
 */
void TextField_t::insert(std::string ins) {
    Range selected = getSelectedRange();

    int first = selected.getFirst();
    int last  = selected.getLast();

    if ( first < 0 )
        first = 0;

    if ( last > text.size() )
        last = text.size();

    std::string beforeCursor = text.substr(0, first);
    std::string afterCursor  = text.substr(last);

    text = beforeCursor + ins + afterCursor;
    setCursor(first + ins.length());
    setMarker(first + ins.length());
    markFor(COMPONENT_REQUIREMENT_UPDATE);
}

/**
 *
 */
bool TextField_t::handle(Event_t& event) {
    static bool shiftDown = false;

    KeyEvent_t* keyEvent = dynamic_cast<KeyEvent_t*>(&event);
    if ( keyEvent ) {
        if ( keyEvent->info.key == "KEY_SHIFT_L" )
            shiftDown = keyEvent->info.pressed;

        if ( keyEvent->info.pressed ) {
            if ( keyEvent->info.key == "KEY_BACKSPACE" )
                backspace(keyEvent->info);

            else if ( keyEvent->info.key == "KEY_ARROW_LEFT" )
                caretMoveStrategy->moveCaret(this, CaretDirection_t::LEFT, keyEvent->info);

            else if ( keyEvent->info.key == "KEY_ARROW_RIGHT" )
                caretMoveStrategy->moveCaret(this, CaretDirection_t::RIGHT, keyEvent->info);

            else if ( keyEvent->info.key == "KEY_A" && keyEvent->info.ctrl ) {
                marker = 0;
                cursor = text.length();
                markFor(COMPONENT_REQUIREMENT_PAINT);
            }

            else {
                char c = Keyboard::charForKey(keyEvent->info);

                if ( c != -1 ) {
                    std::stringstream s;
                    s << c;
                    insert(s.str());
                }
            }
        }
        return true;
    }

    MouseEvent_t* mouseEvent = dynamic_cast<MouseEvent_t*>(&event);
    if ( mouseEvent ) {
        if ( mouseEvent->type == MOUSE_EVENT_ENTER ) {
            visualStatus = TextFieldVisualStatus_t::HOVERED;

            Cursor_t::set("text");
            markFor(COMPONENT_REQUIREMENT_PAINT);
        }

        else if ( mouseEvent->type == MOUSE_EVENT_LEAVE ) {
            visualStatus = TextFieldVisualStatus_t::NORMAL;

            Cursor_t::set("default");
            markFor(COMPONENT_REQUIREMENT_PAINT);
        }

        else if ( mouseEvent->type == MOUSE_EVENT_PRESS ) {
            Point p           = mouseEvent->position;
            int   clickCursor = viewToPosition(p);

            if ( mouseEvent->clickCount > 2 ) {
                marker = 0;
                cursor = text.length();
            }

            else if ( mouseEvent->clickCount == 2 ) {
                marker
                    = caretMoveStrategy->calculateSkip(text, clickCursor, CaretDirection_t::LEFT);
                cursor
                    = caretMoveStrategy->calculateSkip(text, clickCursor, CaretDirection_t::RIGHT);
            }

            else {
                cursor = clickCursor;
                if ( !shiftDown )
                    marker = cursor;
            }

            markFor(COMPONENT_REQUIREMENT_PAINT);
        }

        else if ( mouseEvent->type == MOUSE_EVENT_DRAG ) {
            Point p = mouseEvent->position;
            cursor  = viewToPosition(p);
            markFor(COMPONENT_REQUIREMENT_PAINT);
        }

        return true;
    }

    FocusEvent_t* focusEvent = dynamic_cast<FocusEvent_t*>(&event);
    if ( focusEvent ) {
        if ( focusEvent->type == FOCUS_EVENT_GAINED )
            focused = true;

        else
            focused = false;
        markFor(COMPONENT_REQUIREMENT_PAINT);

        return true;
    }

    return false;
}

/**
 *
 */
int TextField_t::viewToPosition(Point p) {
    int pos = 0;

    for ( int i = 0; i < viewModel->positions.size(); i++ ) {
        PositionedGlyph g      = viewModel->positions[i];
        Rectangle       onView = glyphToView(g);

        if ( p.x < onView.x + onView.width / 2 )
            break;

        ++pos;
    }
    return pos;
}

/**
 *
 */
Rectangle TextField_t::glyphToView(PositionedGlyph& g) {
    int yOffset = getBounds().height / 2 - fontSize / 2 - 2;
    int x       = scrollX + g.position.x + insets.left;
    int y       = g.position.y + yOffset;
    return Rectangle(x, y, g.size.width, g.size.height);
}

/**
 *
 */
Rectangle TextField_t::positionToCursorBounds(int pos) {
    int cursorX     = positionToUnscrolledCursorX(pos);
    int caretHeight = fontSize + 4;

    return Rectangle(scrollX + cursorX, getBounds().height / 2 - caretHeight / 2, 1, caretHeight);
}

/**
 *
 */
void TextField_t::setTitleFont(std::string fontName) {
    setFont(FontLoader::get(fontName));
}

/*
 *
 */
void TextField_t::setFontSize(int size) {
    fontSize = size;
}

/*
 *
 */
void TextField_t::setTitleAlignment(TextAlignment alignment) {
    this->alignment = alignment;
    markFor(COMPONENT_REQUIREMENT_ALL);
}

/**
 *
 */
void TextField_t::setFont(Font_t* f) {
    font = f;
}

/**
 *
 */
void TextField_t::setGhostText(std::string text) {
    ghost     = true;
    ghostText = text;

    markFor(COMPONENT_REQUIREMENT_UPDATE);
}

/**
 *
 */
Range TextField_t::getSelectedRange() {
    return Range(marker, cursor);
}

/**
 *
 */
void TextField_t::setFocus(bool focus) {
    focused = focus;
    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
bool TextField_t::getNumericProperty(int property, uint32_t* out) {
    if ( property == UI_PROPERTY_SECURE ) {
        *out = secure;
        return true;
    }

    return false;
}

/**
 *
 */
bool TextField_t::setNumericProperty(int property, uint32_t value) {
    if ( property == UI_PROPERTY_SECURE ) {
        secure = value;
        return true;
    }

    return false;
}
