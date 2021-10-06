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

#include "ConfigurationBasedVideoOutput.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <Utils/PropertyFileParser.hh>

using namespace std;

/**
 *
 */
bool ConfigurationBasedVideoOutput_t::initialize() {
    // open configuration file
    string   path = "/MeetiX/Configs/WM/PreferredRes.cfg";
    ifstream in(path);
    if ( !in.good() ) {
        cerr << "unable to read configuration file: '" << path << "'" << endl;
        return false;
    }

    // parse it
    Utils::PropertyFileParser parser(in);
    map<string, string>       properties = parser.properties();

    uint32_t width  = 0;
    uint32_t height = 0;
    uint32_t bpp    = 0;

    for ( const auto& entry : properties ) {
        stringstream s;
        s << entry.second;

        if ( entry.first == "width" )
            s >> width;
        else if ( entry.first == "height" )
            s >> height;
        else if ( entry.first == "bpp" )
            s >> bpp;
    }

    // check settings
    if ( !width || !height || !bpp ) {
        cerr << "invalid configuration. width: " << width << ", height: " << height
             << ", bpp: " << bpp << endl;
        return false;
    }

    // call initialization
    return initializeWithSettings(width, height, bpp);
}
