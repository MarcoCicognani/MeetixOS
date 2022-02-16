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

#ifndef __TEXT_FIELD__
#define __TEXT_FIELD__

#include <components/ColoredComponent.hpp>
#include <components/text/TextComponent.hpp>
#include <components/TitledComponent.hpp>
#include <LibGraphics/Metrics/Insets.hh>
#include <LibGraphics/Text/Layouter.hh>
#include <LibIO/Keyboard.hh>
#include <list>
#include <string>

/**
 *
 */
enum class TextFieldVisualStatus_t : uint8_t {
    NORMAL,
    HOVERED
};

/**
 *
 */
class TextField_t : public TextComponent_t,
                    public TitledComponent_t,
                    public ColoredComponent_t {
private:
    std::string               text;
    TextFieldVisualStatus_t   visualStatus;
    Graphics::Text::Alignment alignment;
    bool                      focused;
    bool                      secure;

    Graphics::Text::Font* font;
    std::string           ghostText;
    bool                  ghost;

    int                           scrollX;
    int                           fontSize;
    Graphics::Color::ArgbGradient textColor;
    Graphics::Color::ArgbGradient sColor;
    Graphics::Metrics::Insets     insets;

    int cursor;
    int marker;

    Graphics::Text::Layouted* viewModel;

    cairo_t*                     cr;
    Graphics::Metrics::Rectangle bounds;

    void loadDefaultFont();
    void applyScroll();

public:
    TextField_t();
    virtual ~TextField_t();

    /**
     * component
     */
    virtual void update();
    virtual void paint();
    virtual bool handle(Event_t& e);
    virtual void setFocus(bool focus);
    virtual bool getNumericProperty(int property, uint32_t* out);
    virtual bool setNumericProperty(int property, uint32_t value);

    /**
     * text component
     */
    virtual void                     setCursor(int pos);
    virtual int                      getCursor() { return cursor; }
    virtual void                     setMarker(int pos);
    virtual int                      getMarker() { return marker; }
    virtual Graphics::Metrics::Range getSelectedRange();
    virtual void                     setGhostText(std::string text);
    virtual void                     setText(std::string text);
    virtual std::string              getText() { return text; }

    /**
     * titled component
     */
    virtual void        setTitle(std::string title) { setText(title); }
    virtual std::string getTitle() { return getText(); }
    virtual void        setFont(Graphics::Text::Font* f);
    virtual void        setTitleFont(std::string fontName);
    virtual void        setFontSize(int size);
    virtual void        setTitleAlignment(Graphics::Text::Alignment alignment);

    /**
     * colored component
     */
    virtual void setColor(Graphics::Color::ArgbGradient ShapeColor, Graphics::Color::ArgbGradient titleColor);

    /**
     * textfield
     */
    void                         backspace(IO::Keyboard::Info& info);
    void                         insert(std::string text);
    int                          viewToPosition(Graphics::Metrics::Point p);
    Graphics::Metrics::Rectangle glyphToView(Graphics::Text::PositionedGlyph& g);
    int                          positionToUnscrolledCursorX(int pos);
    Graphics::Metrics::Rectangle positionToCursorBounds(int pos);
};

#endif
