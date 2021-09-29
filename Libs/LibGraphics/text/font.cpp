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

#include <graphics/text/font.hpp>
#include <graphics/text/fontmgr.hpp>
#include <io/files/futils.hpp>
#include <string.h>
#include <Utils/Utils.hh>

/**
 * Creates an empty font with the "name". The "source" data
 * is copied to an internal buffer.
 *
 * @param name:				font lookup name
 * @param source:			font data
 * @param sourceLength:		font data length
 * @param style:			font style
 * @param hint:				whether to render with hinting (antialias) or not
 */
Font_t::Font_t(const std::string& name,
               uint8_t*           source,
               uint32_t           sourceLength,
               FontStyle          style,
               bool               hint)
    : name(name), data(nullptr), face(nullptr), okay(false), style(style), activeSize(0),
      hint(hint) {
    data = new uint8_t[sourceLength];
    memcpy(data, source, sourceLength);

    // Check data
    if ( !data ) {
        Utils::log("failed to allocate memory buffer for font '" + name + "'");
        return;
    }

    // Load font
    FT_Error error = FT_New_Memory_Face(FontManager::getInstance()->getLibraryHandle(),
                                        data,
                                        sourceLength,
                                        0,
                                        &face);
    if ( error ) {
        Utils::log("freetype2 failed at FT_New_Memory_Face with error code %i", error);

        delete data;
        return;
    }

    // create cairo face
    cairoFace = cairo_ft_font_face_create_for_ft_face(face, 0);
    okay      = true;
}

/**
 * Destroys the font, deleting all associated Glyph
 * objects and freeing the freetype face.
 */
Font_t::~Font_t() {
    if ( okay ) {
        // destroy cairo face
        cairo_font_face_destroy(cairoFace);

        // destroy font
        FT_Done_Face(face);

        // destroy data
        delete data;
    }
}

/**
 * Loads a font from the file "in". If there is already a font with
 * the "name", that font is returned.
 *
 * @param in:		the font file
 * @param name:		the name to register the font to
 * @return either the font, or nullptr
 */
Font_t* Font_t::fromFile(FILE* in, const std::string& name) {
    // first check if is already a loaded font
    Font_t* existing = FontManager::getInstance()->getFont(name);
    if ( existing )
        return existing;

    // get the length of the file
    int64_t length = s_length(fileno(in));
    if ( length == -1 )
        return nullptr;

    // read the file
    auto fileContent = new uint8_t[length];
    if ( !FileUtils::tryReadBytes(in, 0, fileContent, length) ) {
        delete[] fileContent;
        return nullptr;
    }

    // create the new font
    if ( !FontManager::getInstance()->createFont(name, fileContent, length) ) {
        delete[] fileContent;
        return nullptr;
    }

    // return the new font instance
    return FontManager::getInstance()->getFont(name);
}
