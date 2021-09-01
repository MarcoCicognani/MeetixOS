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

#ifndef MEETIX_LIBRARY_GRAPHICS_TEXT_FONTLOADER
#define MEETIX_LIBRARY_GRAPHICS_TEXT_FONTLOADER

#include <graphics/text/font.hpp>
#include <string>

/**
 * static class to load fonts and get instance of it
 */
class FontLoader
{
private:
	/**
	 * load a font file from an arbitrary path
	 *
	 * @param path:		the path of the font file
	 * @param name:		the name of the font
	 * @return the font object instance or nullptr
	 */
	static Font_t *getFontAtPath(const std::string &path, const std::string &name);

	/**
	 * load a font file from predefinited system path
	 *
	 * @param name:		the name of the font
	 * @return the font object instance or nullptr
	 */
	static Font_t *getSystemFont(const std::string &name);

public:
	/**
	 * get the font object from a name, if it doesn't exist
	 * check througt the filesystem
	 *
	 * @param name:		the name of the font
	 * @return the font instance or nullptr
	 */
	static Font_t *get(const std::string &name);

	/**
	 * @return the default font instance
	 */
	static Font_t *getDefault();
};

#endif
