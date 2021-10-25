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

#ifndef __WINDOW__
#define __WINDOW__

#include <components/ColoredComponent.hpp>
#include <components/component.hpp>
#include <components/label.hpp>
#include <components/panel.hpp>
#include <components/PngComponent.hpp>
#include <components/TitledComponent.hpp>
#include <GUI/Properties.hh>
#include <GUI/Protocol.hh>

/**
 * constants for border sizes
 */
#define DEFAULT_BORDER_WIDTH 7
#define DEFAULT_CORNER_SIZE  15

/**
 * modes used when resizing windows
 */
enum WindowResizeMode
{
    RESIZE_MODE_NONE,
    RESIZE_MODE_MOVE,
    RESIZE_MODE_TOP,
    RESIZE_MODE_TOP_RIGHT,
    RESIZE_MODE_RIGHT,
    RESIZE_MODE_BOTTOM_RIGHT,
    RESIZE_MODE_BOTTOM,
    RESIZE_MODE_BOTTOM_LEFT,
    RESIZE_MODE_LEFT,
    RESIZE_MODE_TOP_LEFT
};

/**
 *
 */
class Window_t : public Component_t,
                 public TitledComponent_t,
                 public PngComponent_t,
                 public ColoredComponent_t {
private:
    int  borderWidth;
    int  cornerSize;
    bool resizable;

    cairo_t*                     cr;
    Graphics::Metrics::Rectangle bounds;

    Label_t label;
    Panel_t panel;

    bool crossPressed;
    bool crossHovered;
    bool maximizeHovered;
    bool maximizePressed;
    bool isFullScreen;
    bool focused;

    int                          shadowSize;
    Graphics::Metrics::Rectangle initialBounds;
    Graphics::Metrics::Rectangle crossBounds;
    Graphics::Metrics::Rectangle maximizeBounds;
    string                       style;

public:
    Window_t();

    virtual ~Window_t() {
    }

    /**
     *
     */
    Panel_t* getPanel() {
        return &panel;
    }

    /**
     * component
     */
    virtual void setFocus(bool focus);
    virtual void addChild(Component_t*          component,
                          ChildComponentRefType ref_type = ChildComponentRefType::Default);
    virtual void layout();
    virtual void paint();

    /**
     *	png component
     */
    virtual void setPNG(std::string path, Graphics::Metrics::Point position);
    virtual void PngAnimation(std::string              path,
                              Graphics::Metrics::Point PNGstartAnimation,
                              Graphics::Metrics::Point PNGendAnimation,
                              size_t                   sleep);

    /**
     * color component
     */
    virtual void setColor(Graphics::Color::ArgbGradient color,
                          Graphics::Color::ArgbGradient tltColor);

    /**
     *
     */
    virtual void handleBoundChange(Graphics::Metrics::Rectangle oldBounds);
    virtual bool handle(Event_t& e);
    virtual bool getNumericProperty(int property, uint32_t* out);
    virtual bool setNumericProperty(int property, uint32_t value);

    /**
     *	titled component
     */
    virtual void        setTitle(std::string title);
    virtual std::string getTitle();
    virtual void        setTitleFont(std::string fontName);
    virtual void        setFontSize(int size) {
    }
    virtual void setTitleAlignment(Graphics::Text::Alignment alignment);

    bool isWindow() const override {
        return true;
    }

    /**
     *
     */
    virtual void closeAll();
};

#endif
