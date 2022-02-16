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
#include <LibGraphics/Text/FontLoader.hh>
#include <LibGraphics/Text/FontManager.hh>
#include <LibGUI/Properties.hh>
#include <sstream>

/**
 *
 */
TextField_t::TextField_t()
    : cursor(0)
    , marker(0)
    , scrollX(0)
    , secure(false)
    , focused(false)
    , visualStatus(TextFieldVisualStatus_t::NORMAL)
    , alignment(Graphics::Text::Alignment::CENTER)
    , fontSize(14)
    , textColor(Graphics::Color::as_rgb(0, 0, 0))
    , insets(Graphics::Metrics::Insets(5, 5, 5, 5))
    , ghost(false) {
    caretMoveStrategy = DefaultCaretMoveStrategy_t::getInstance();
    type              = UI_COMPONENT_TYPE_TEXTFIELD;

    viewModel = Graphics::Text::Layouter::instance().init_buffer();
    setFont(Graphics::Text::FontLoader::get("default"));
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
    auto bounds = getBounds();

    std::string visibleText = text;

    if ( ghost )
        visibleText = ghostText;

    else if ( secure ) {
        visibleText = "";
        for ( int i = 0; i < text.length(); i++ )
            visibleText = visibleText + "*";
    }

    Graphics::Text::Layouter::instance().layout(graphics.cairo_context(),
                                                visibleText.c_str(),
                                                font,
                                                fontSize,
                                                Graphics::Metrics::Rectangle(0, 0, bounds.width(), bounds.height()),
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

    cr     = graphics.cairo_context();
    bounds = getBounds();
    clearSurface();

    // background
    cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(sColor));
    cairo_rectangle(cr, 0, 0, bounds.width(), bounds.height());
    cairo_fill(cr);

    // border
    Graphics::Color::ArgbGradient borderColor;
    if ( focused )
        borderColor = Graphics::Color::as_argb(255, 0, 200, 0);

    else if ( visualStatus == TextFieldVisualStatus_t::HOVERED )
        borderColor = Graphics::Color::as_argb(255, 255, 255, 255);

    else
        borderColor = Graphics::Color::as_argb(255, 180, 180, 180);

    cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(borderColor));
    cairo_rectangle(cr, 0, 0, bounds.width(), bounds.height());
    cairo_stroke(cr);

    // Scroll
    applyScroll();

    int pos    = 0;
    int first  = marker < cursor ? marker : cursor;
    int second = marker > cursor ? marker : cursor;

    // Paint marking
    if ( focused ) {
        pos = 0;
        for ( auto& g : viewModel->m_positioned_glyphs ) {
            Graphics::Color::ArgbGradient color = textColor;
            if ( first != second && pos >= first && pos < second ) {
                cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(Graphics::Color::as_argb(255, 0, 200, 0)));
                auto before = positionToCursorBounds(pos);
                auto after  = positionToCursorBounds(pos + 1);
                cairo_rectangle(cr, before.x(), before.y(), after.x() - before.x(), before.height());
                cairo_fill(cr);

                color = Graphics::Color::as_argb(255, 255, 255, 255);
            }
            ++pos;
        }
    }

    // Paint glyphs
    pos = 0;
    for ( auto& g : viewModel->m_positioned_glyphs ) {
        auto                          onView = glyphToView(g);
        Graphics::Color::ArgbGradient color  = textColor;

        if ( ghost )
            color = Graphics::Color::as_argb(160, 0, 0, 0);

        else if ( focused && first != second && pos >= first && pos < second )
            color = Graphics::Color::as_argb(255, 255, 255, 255);

        cairo_save(cr);
        cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(color));
        cairo_translate(cr, onView.x() - g.m_cairo_glyph->x,
                        onView.y() - g.m_cairo_glyph->y); // TODO?
        cairo_glyph_path(cr, g.m_cairo_glyph, g.m_cairo_glyph_count);
        cairo_fill(cr);
        cairo_restore(cr);
        ++pos;
    }

    // Paint cursor
    if ( focused ) {
        ghost = false;

        markFor(COMPONENT_REQUIREMENT_UPDATE);
        cairo_set_source_rgba(cr, ARGB_TO_CAIRO_PARAMS(Graphics::Color::as_argb(255, 60, 60, 60)));

        auto bounds = positionToCursorBounds(cursor);

        cairo_rectangle(cr, bounds.x(), bounds.y(), bounds.width(), bounds.height());
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
void TextField_t::setColor(Graphics::Color::ArgbGradient shapeColor, Graphics::Color::ArgbGradient titleColor) {
    sColor    = shapeColor;
    textColor = titleColor;

    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void TextField_t::applyScroll() {
    int  cursorX = positionToUnscrolledCursorX(cursor);
    auto bounds  = getBounds();

    // scroll
    if ( scrollX + cursorX > bounds.width() - insets.right() )
        scrollX = bounds.width() - cursorX - insets.right();

    else if ( scrollX + cursorX < insets.left() )
        scrollX = -cursorX + insets.left() + bounds.width() / 2;
}

/**
 *
 */
int TextField_t::positionToUnscrolledCursorX(int pos) {
    int cursorX        = insets.left();
    int positionsCount = viewModel->m_positioned_glyphs.size();
    for ( int i = 0; i < positionsCount; i++ ) {
        auto& g = viewModel->m_positioned_glyphs[i];
        // After last?
        if ( i == positionsCount - 1 && pos == positionsCount )
            cursorX = g.m_position.x() + insets.left() + g.m_advance.x();

        // Anywhere inside
        if ( i == pos )
            cursorX = g.m_position.x() + insets.left() - 1;
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
void TextField_t::backspace(IO::Keyboard::Info& info) {
    if ( text.length() > 0 ) {
        auto selected = getSelectedRange();

        int leftcut = selected.first();
        if ( info.m_alt )
            leftcut = caretMoveStrategy->calculateSkip(text, leftcut, CaretDirection_t::LEFT);

        else if ( info.m_ctrl )
            leftcut = 0;

        int rightcut = selected.last();

        if ( rightcut - leftcut == 0 )
            leftcut--;

        if ( leftcut >= 0 && rightcut <= text.length() ) {
            auto beforeCursor = text.substr(0, leftcut);
            auto afterCursor  = text.substr(rightcut);
            text              = beforeCursor + afterCursor;
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
    auto selected = getSelectedRange();

    int first = selected.first();
    int last  = selected.last();

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
        if ( keyEvent->info.m_key == "KEY_SHIFT_L" )
            shiftDown = keyEvent->info.m_is_pressed;

        if ( keyEvent->info.m_is_pressed ) {
            if ( keyEvent->info.m_key == "KEY_BACKSPACE" )
                backspace(keyEvent->info);

            else if ( keyEvent->info.m_key == "KEY_ARROW_LEFT" )
                caretMoveStrategy->moveCaret(this, CaretDirection_t::LEFT, keyEvent->info);

            else if ( keyEvent->info.m_key == "KEY_ARROW_RIGHT" )
                caretMoveStrategy->moveCaret(this, CaretDirection_t::RIGHT, keyEvent->info);

            else if ( keyEvent->info.m_key == "KEY_A" && keyEvent->info.m_ctrl ) {
                marker = 0;
                cursor = text.length();
                markFor(COMPONENT_REQUIREMENT_PAINT);
            }

            else {
                char c = IO::Keyboard::instance().char_for_key(keyEvent->info);

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

            Cursor::instance().set("text");
            markFor(COMPONENT_REQUIREMENT_PAINT);
        }

        else if ( mouseEvent->type == MOUSE_EVENT_LEAVE ) {
            visualStatus = TextFieldVisualStatus_t::NORMAL;

            Cursor::instance().set("default");
            markFor(COMPONENT_REQUIREMENT_PAINT);
        }

        else if ( mouseEvent->type == MOUSE_EVENT_PRESS ) {
            auto p           = mouseEvent->position;
            int  clickCursor = viewToPosition(p);

            if ( mouseEvent->clickCount > 2 ) {
                marker = 0;
                cursor = text.length();
            }

            else if ( mouseEvent->clickCount == 2 ) {
                marker = caretMoveStrategy->calculateSkip(text, clickCursor, CaretDirection_t::LEFT);
                cursor = caretMoveStrategy->calculateSkip(text, clickCursor, CaretDirection_t::RIGHT);
            }

            else {
                cursor = clickCursor;
                if ( !shiftDown )
                    marker = cursor;
            }

            markFor(COMPONENT_REQUIREMENT_PAINT);
        }

        else if ( mouseEvent->type == MOUSE_EVENT_DRAG ) {
            auto p = mouseEvent->position;
            cursor = viewToPosition(p);
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
int TextField_t::viewToPosition(Graphics::Metrics::Point p) {
    int pos = 0;

    for ( auto& g : viewModel->m_positioned_glyphs ) {
        auto onView = glyphToView(g);

        if ( p.x() < onView.x() + onView.width() / 2 )
            break;

        ++pos;
    }
    return pos;
}

/**
 *
 */
Graphics::Metrics::Rectangle TextField_t::glyphToView(Graphics::Text::PositionedGlyph& g) {
    int yOffset = getBounds().height() / 2 - fontSize / 2 - 2;
    int x       = scrollX + g.m_position.x() + insets.left();
    int y       = g.m_position.y() + yOffset;
    return { x, y, g.m_size.width(), g.m_size.height() };
}

/**
 *
 */
Graphics::Metrics::Rectangle TextField_t::positionToCursorBounds(int pos) {
    int cursorX     = positionToUnscrolledCursorX(pos);
    int caretHeight = fontSize + 4;

    return { scrollX + cursorX, getBounds().height() / 2 - caretHeight / 2, 1, caretHeight };
}

/**
 *
 */
void TextField_t::setTitleFont(std::string fontName) {
    setFont(Graphics::Text::FontLoader::get(fontName));
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
void TextField_t::setTitleAlignment(Graphics::Text::Alignment alignment) {
    this->alignment = alignment;
    markFor(COMPONENT_REQUIREMENT_ALL);
}

/**
 *
 */
void TextField_t::setFont(Graphics::Text::Font* f) {
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
Graphics::Metrics::Range TextField_t::getSelectedRange() {
    return Graphics::Metrics::Range(marker, cursor);
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
