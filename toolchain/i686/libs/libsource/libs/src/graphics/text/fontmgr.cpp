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

#include <graphics/text/fontmgr.hpp>
#include <utils/utils.hpp>

/**
 * singletone instance of the class
 */
static FontManager *instance = nullptr;

/**
 * @return the instance of the font manager singleton
 */
FontManager *FontManager::getInstance()
{
	if (!instance) instance = new FontManager();
	return instance;
}

/**
 * constructor
 */
FontManager::FontManager()
{
	initializeEngine();
}

/**
 * destructor
 */
FontManager::~FontManager()
{
	destroyEngine();
}

/**
 * Initializes a freetype library instance
 */
void FontManager::initializeEngine()
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error) Utils::log("freetype2 failed at FT_Init_FreeType with error code %i", error);
}

/**
 * Destroys the freetype library instance
 */
void FontManager::destroyEngine()
{
	FT_Error error = FT_Done_Library(library);
	if (error) Utils::log("freetype2 failed at FT_Done_Library with error code %i", error);
}

/**
 * Looks for an existing font with the "name".
 *
 * @param name:		the name to which the font is registered
 */
Font_t *FontManager::getFont(const std::string &name)
{
	if (fontRegistry.count(name) > 0) return fontRegistry[name];
	return 0;
}

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
bool FontManager::createFont(const std::string &name, uint8_t *source, uint32_t sourceLength, FontStyle style, bool hint)
{
	// check if font already exists
	if (fontRegistry.count(name) > 0)
	{
		Utils::log("tried to create font '" + name + "' that already exists");
		return false;
	}

	// Create font object
	Font_t *font = new Font_t(name, source, sourceLength, style, hint);
	if (!font->isOkay()) delete font;

	// Register font
	fontRegistry[name] = font;

	Utils::log("created font '" + name + "'");
	return true;
}

/**
 * Deletes the font and removes it from the font registry.
 *
 * @param font:		the font to destroy
 */
void FontManager::destroyFont(Font_t *font)
{
	fontRegistry.erase(font->getName());
	delete font;
}
