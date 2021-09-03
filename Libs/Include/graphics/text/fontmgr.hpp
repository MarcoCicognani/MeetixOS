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

#ifndef MEETIX_LIBRARY_GRAPHICS_TEXT_FONTMANAGER
#define MEETIX_LIBRARY_GRAPHICS_TEXT_FONTMANAGER

#include <graphics/text/font.hpp>
#include <graphics/text/freetype.hpp>
#include <map>
#include <string>

/**
 * font manager, manteins trace of all created fonts and premits to create and destroy its
 */
class FontManager {
private:
    /**
     * fonts table
     */
    FT_Library                     library;      // freetype library
    std::map<std::string, Font_t*> fontRegistry; // fonts registry

    /**
     * constructor and destructor are private because this class is singletone
     */
    FontManager();
    ~FontManager();

    /**
     * Initializes a freetype library instance
     */
    void initializeEngine();

    /**
     * Destroys the freetype library instance
     */
    void destroyEngine();

public:
    /**
     * @return the instance of the font manager singleton
     */
    static FontManager* getInstance();

    /**
     * Creates a font with the "name", reading the font data "source".
     * The data within "source" is copied to the {Font} objects buffer.
     *
     * @param name:			name to which the font shall be registered
     * @param source:		font data
     * @param sourceLength:	length of the font data
     * @param style:		font style
     * @param hint:			whether to hint the font
     * @return whether the operation success
     */
    bool createFont(const std::string& name,
                    uint8_t*           source,
                    uint32_t           sourceLength,
                    FontStyle          style = FontStyle::NORMAL,
                    bool               hint  = true);

    /**
     * Looks for an existing font with the "name".
     *
     * @param name:		the name to which the font is registered
     */
    Font_t* getFont(const std::string& name);

    /**
     * Deletes the font and removes it from the font registry.
     *
     * @param font:		the font to destroy
     */
    void destroyFont(Font_t* font);

    /**
     * @return the freetype library handle
     */
    inline FT_Library getLibraryHandle() {
        return library;
    }
};

#endif
