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

#include <fstream>
#include <functional>
#include <gui/menu.hpp>
#include <Utils/PropertyFileParser.hh>

using namespace std;

/*
 *	constructor, espected path to configuration file
 */
ButtonMenu::ButtonMenu(const string& pathToConfiguration) {
    getMapFromPath(pathToConfiguration);
}

/*
 *	constructor with map of configuration, bounds and geoshape base espected
 */
ButtonMenu::ButtonMenu(const map<string, string>& configuration,
                       const Rectangle&           bounds,
                       Geoshape*                  where) {
    create(configuration, bounds);
    show(where);
}

/*
 *	constructor with map of configuration, bounds and window base espected
 */
ButtonMenu::ButtonMenu(const map<string, string>& configuration,
                       const Rectangle&           bounds,
                       Window*                    where) {
    create(configuration, bounds);
    show(where);
}

/*
 * fill cfg map with configuration file in file from path
 */
void ButtonMenu::getMapFromPath(const string& path) {
    // opening file in path
    ifstream file(path);

    // parse it and copy to local configuration
    Utils::PropertyFileParser parser(file);
    cfg = parser.properties();

    // closing file
    file.close();
}

/*
 * configure and create button
 */
void ButtonMenu::configure() {
    // local index for buttons disposition
    int i = 0;

    // parse configuration map
    for ( pair<string, string> current : cfg ) {
        pngPaths.push_back(parseCurrent(current));
        configureCurrent(current, i);
        i++;
    }
}

/*
 * parse provided couple of key/value and add it to ButtonList
 */
string ButtonMenu::parseCurrent(const pair<string, string>& current) {
    // local string for parameters split
    string icon;
    string command;

    // check index of parameters separator
    int index = current.second.find('&');

    // if twice parameters exist
    if ( index != -1 ) {
        // split string to command and icont path
        command = current.second.substr(0, index);
        icon    = current.second.substr(index + 1);
    }

    // else copy value to command
    else
        command = current.second;

    // add button to list
    buttons.add(current.first,
                [command]() { s_spawn(command.c_str(), "", "/", SECURITY_LEVEL_APPLICATION); });

    // return icon path
    return icon;
}

/*
 * configure the current button
 */
void ButtonMenu::configureCurrent(const pair<string, string>& current, int index) {
    // configure bounds
    if ( index == 0 )
        buttonBounds = Rectangle(0, height, buttonBounds.width, buttonHeight);

    else {
        height       = height + buttonHeight;
        buttonBounds = Rectangle(0, height, buttonBounds.width, buttonHeight);
    }

    // configure current button in buttonlist
    buttons.configure(current.first,
                      buttonBounds,
                      current.first,
                      pngPaths[index],
                      Point(0, 0),
                      ARGB(10, 0, 0, 0),
                      RGB(255, 255, 255));
}

/*
 * create menu
 */
void ButtonMenu::create(const string& pathToConfiguration, const Rectangle& bounds) {
    getMapFromPath(pathToConfiguration);
    setBounds(bounds);
    configure();
}

/*
 *	create menu
 */
void ButtonMenu::create(const map<string, string>& configuration, const Rectangle& bounds) {
    cfg = configuration;
    setBounds(bounds);
    configure();
}

/*
 *	set size of menu
 */
void ButtonMenu::setBounds(const Rectangle& bounds) {
    bds          = bounds;
    buttonBounds = Rectangle(0, 0, bounds.width, buttonHeight);
}

/*
 *	menu on desire Geoshape form
 */
void ButtonMenu::show(Geoshape* where) {
    // get bounds from geoshape
    Rectangle bounds = where->getBounds();

    // if it have a smaller height set correct
    if ( bounds.height < this->height ) {
        // create new bounds and set it
        bounds = Rectangle(bounds.x, bounds.y, bounds.width, this->height + 60);
        where->setBounds(bounds);
    }

    // show on screen
    buttons.show(where);
}
