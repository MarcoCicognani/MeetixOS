/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef MEETIX_LIBRARY_UI_BUTTONLIST
#define MEETIX_LIBRARY_UI_BUTTONLIST

#include <Api.h>
#include <functional>
#include <gui/button.hpp>
#include <gui/geoshape.hpp>
#include <gui/window.hpp>
#include <string>
#include <unordered_map>

/*
 * container class to create button list
 */
class ButtonList {
public:
    // constructors
    ButtonList() {
    }
    ButtonList(const std::string& name, Button* button, const std::function<void(void)>& handler) {
        add(name, button, handler);
    }

    // destructor
    ~ButtonList();

    // overloads of add method
    void add(const std::string& name, ActionListener* listener);
    void add(const std::string& name, Button* button, ActionListener* listener);

    void add(const std::string& name, const std::function<void(void)>& handler);
    void add(const std::string& name, Button* button, const std::function<void(void)>& handler);

    // get method, interact with single objects
    Button* get(const std::string& name);
    Button* operator[](const std::string& name);

    // overloaded configure method, each method provide different combination of parameters
    void configure(const std::string& name, const Graphics::Metrics::Rectangle& bounds);
    void configure(const std::string&                  name,
                   const Graphics::Metrics::Rectangle& bounds,
                   const std::string&                  title);
    void configure(const std::string&                  name,
                   const Graphics::Metrics::Rectangle& bounds,
                   const std::string&                  title,
                   Graphics::Color::ArgbGradient       buttonColor,
                   Graphics::Color::ArgbGradient       titleColor);
    void configure(const std::string&                  name,
                   const Graphics::Metrics::Rectangle& bounds,
                   const std::string&                  title,
                   const std::string&                  pathToPng,
                   const Graphics::Metrics::Point&     pngPosition);
    void configure(const std::string&                  name,
                   const Graphics::Metrics::Rectangle& bounds,
                   const std::string&                  title,
                   const std::string&                  pathToPng,
                   const Graphics::Metrics::Point&     pngPosition,
                   Graphics::Color::ArgbGradient       buttonColor,
                   Graphics::Color::ArgbGradient       titleColor);

    // method to display on screen buttons of map
    void show(Window* where);
    void show(Geoshape* where);
    void show(const std::string& name, Window* where);
    void show(const std::string& name, Geoshape* where);

private:
    // maps that contains button and handlers
    std::unordered_map<std::string, Button*>         buttons;
    std::unordered_map<std::string, ActionListener*> handlers;
};

#endif
