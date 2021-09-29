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

#include <graphics/text/fontldr.hpp>
#include <Utils/Utils.hh>

/**
 * load a font file from an arbitrary path
 *
 * @param path:		the path of the font file
 * @param name:		the name of the font
 * @return the font object instance or nullptr
 */
Font_t* FontLoader::getFontAtPath(const std::string& path, const std::string& name) {
    // open the file
    auto file = fopen(path.c_str(), "r");
    if ( file ) {
        // get the font instance from file
        auto font = Font_t::fromFile(file, name);
        fclose(file);
        return font;
    }

    return nullptr;
}

/**
 * load a font file from predefinited system path
 *
 * @param name:		the name of the font
 * @return the font object instance or nullptr
 */
Font_t* FontLoader::getSystemFont(const std::string& name) {
    return getFontAtPath("/MeetiX/Configs/WM/Fonts/" + name + ".ttf", name);
}

/**
 * get the font object from a name, if it doesn't exist
 * check througt the filesystem
 *
 * @param name:		the name of the font
 * @return the font instance or nullptr
 */
Font_t* FontLoader::get(const std::string& name) {
    auto font = getSystemFont(name);
    if ( !font )
        font = getDefault();

    return font;
}

/**
 * @return the default font instance
 */
Font_t* FontLoader::getDefault() {
    return getSystemFont("default");
}
