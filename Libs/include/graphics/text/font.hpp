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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#ifndef MEETIX_LIBRARY_GRAPHICS_TEXT_FONT
#define MEETIX_LIBRARY_GRAPHICS_TEXT_FONT

#include <graphics/text/freetype.hpp>
#include <string>
#include <map>
#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

/**
 * the styles of fonts
 */
enum class FontStyle : uint8_t
{
	NORMAL,
	ITALIC,
	BOLD
};

/**
 * class to manage cairo fonts
 */
class Font_t
{
private:
	/**
	 * internal data
	 */
	uint8_t *data;		// data buffer
	std::string name;	// name of the font
	FontStyle style;	// font style
	bool hint;			// aliasing active flag

	/**
	 * backend
	 */
	FT_Face face;		// freetype object
	cairo_font_face_t *cairoFace;	// cairo font object

	/**
	 * flags
	 */
	bool okay;			// ready flag
	int activeSize;		// current size of the font

public:

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
	Font_t(const std::string &name, uint8_t *source, uint32_t sourceLength, FontStyle style, bool hint = true);

	/**
	 * Destroys the font, deleting all associated Glyph
	 * objects and freeing the freetype face.
	 */
	~Font_t();

	/**
	 * Loads a font from the file "in". If there is already a font with
	 * the "name", that font is returned.
	 *
	 * @param in:		the font file
	 * @param name:		the name to register the font to
	 * @return either the font, or nullptr
	 */
	static Font_t *fromFile(FILE *in, const std::string &name);

	/**
	 * @return whether the font was successfully initialized.
	 */
	inline bool isOkay() { return okay; }

	/**
	 * @return the name of the font
	 */
	inline std::string getName() { return name; }

	/**
	 * @return the font cairo surface
	 */
	inline cairo_font_face_t *getFace() { return cairoFace; }

};

#endif
