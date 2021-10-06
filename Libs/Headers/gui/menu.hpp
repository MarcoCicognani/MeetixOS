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

#ifndef MEETIX_LIBRARY_UI_MENU
#define MEETIX_LIBRARY_UI_MENU

#include <gui/btnlist.hpp>
#include <vector>

/*
 *	this class provide a more complex list for button, is a simple way
 *	to create structurate menu list with button and icons
 */
class ButtonMenu {
private:
    // local list for button list
    ButtonList buttons;

    // local configuration
    std::map<std::string, std::string> cfg;

    // local vector that content paths to buttons icon
    std::vector<std::string> pngPaths;

    // standard button heigth
    const int buttonHeight = 30;

    // first heigth
    int height = 30;

    // local bounds
    Graphics::Metrics::Rectangle bds;

    // used for dimension of each button
    Graphics::Metrics::Rectangle buttonBounds;

    // copy into cfg map the configuration in file on path
    void getMapFromPath(const std::string& path);

    // configure internal map
    void configure();

    // process each pair of key/value in cfg map
    std::string parseCurrent(const std::pair<std::string, std::string>& current);

    // configure current button on button list
    void configureCurrent(const std::pair<std::string, std::string>& current, int index);

public:
    // empty constructor
    ButtonMenu(){};

    // constructor, espected path to configuration file
    ButtonMenu(const std::string& pathToConfiguration);

    // constructor with map of configuration, bounds and geoshape base espected
    ButtonMenu(const std::map<std::string, std::string>& configuration,
               const Graphics::Metrics::Rectangle&       bounds,
               Geoshape*                                 where);

    // constructor with map of configuration, bounds and window base espected
    ButtonMenu(const std::map<std::string, std::string>& configuration,
               const Graphics::Metrics::Rectangle&       bounds,
               Window*                                   where);

    // create menu from file configuration
    void create(const std::string& pathToConfiguration, const Graphics::Metrics::Rectangle& bounds);

    // create menu from map configuration
    void create(const std::map<std::string, std::string>& configuration,
                const Graphics::Metrics::Rectangle&       bounds);

    // set size of menu
    inline void setBounds(const Graphics::Metrics::Rectangle& bounds);

    // menu on desire Geoshape form
    void show(Geoshape* where);

    // menu on desire Window form
    inline void show(Window* where) {
        buttons.show(where);
    }
};

#endif
