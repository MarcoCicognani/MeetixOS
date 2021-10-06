/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
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

#ifndef _GEOSHAPE_
#define _GEOSHAPE_

#include <components/ColoredComponent.hpp>
#include <components/component.hpp>
#include <components/label.hpp>
#include <components/panel.hpp>
#include <components/PngComponent.hpp>
#include <components/TitledComponent.hpp>

class Geoshape_t : public Component_t,
                   public TitledComponent_t,
                   public PngComponent_t,
                   public ColoredComponent_t {
private:
    bool resizable;

    Panel_t panel;
    Label_t label;

    Graphics::Metrics::Rectangle geoshapeBounds;
    cairo_t*                     geoshapeSurface;

public:
    Geoshape_t();

    virtual ~Geoshape_t() {
    }

    /**
     *
     */
    Panel_t* getPanel() {
        return &panel;
    };

    /**
     *	component
     */
    virtual void addChild(Component_t* component);
    virtual void layout();
    virtual void paint();
    virtual bool handle(Event_t& event);
    virtual void setFocus(bool focus) {
    }

    /**
     *	titled component
     */
    virtual void        setTitle(std::string title);
    virtual std::string getTitle();
    virtual void        setTitleFont(std::string fontName);
    virtual void        setFontSize(int size) {
    }
    virtual void setTitleAlignment(Graphics::Text::Alignment alignment);

    /**
     *	png component
     */
    virtual void setPNG(std::string path, Graphics::Metrics::Point position);
    virtual void PngAnimation(std::string              path,
                              Graphics::Metrics::Point PNGstartAnimation,
                              Graphics::Metrics::Point PNGendAnimation,
                              size_t                   sleep);

    /**
     *	colored component
     */
    virtual void setColor(Graphics::Color::ArgbGradient color,
                          Graphics::Color::ArgbGradient tltColor);
};

#endif
