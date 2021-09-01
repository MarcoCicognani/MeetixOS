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

#ifndef MEETIX_LIBRARY_UTILS_PROPERTYFILEPARSER
#define MEETIX_LIBRARY_UTILS_PROPERTYFILEPARSER

#include <string>
#include <vector>
#include <map>

/**
 * class to parse and get properties of property files
 * file must be organized as:
 * key[-:=]value
 * parser return a map filled with key and values
 */
class PropertyFileParser
{
public:
	/**
	 * filled constructor
	 *
	 * @param t:	the filestream opened to the property file
	 */
	PropertyFileParser(std::ifstream &t);

	/**
	 * filled constructor
	 *
	 * @param content:	the content of the file
	 */
	PropertyFileParser(const std::string &_content);

	/**
	 * @return the parsed properties map of the file provided
	 */
	std::map<std::string, std::string> getProperties();

private:
	/**
	 * internal data
	 */
	char current;			// current readed character
	uint32_t position;		// position into the file
	std::string content;

	/**
	 * initializes the parser from a file content
	 *
	 * @param content:		the content of the file
	 */
	void initialize(const std::string &_content);

	/**
	 * fill the strings provided with the current line values
	 *
	 * @param key:		the first string where store to
	 * @param value:	the second string where store to
	 * @return whether the operation success
	 */
	bool propertyEntry(std::string &key, std::string &value);

	/**
	 * parse the line and store to out the
	 * key part of the property
	 *
	 * @param out:		the string where store the property readed
	 * @return whether the operation success
	 */
	bool key(std::string &out);

	/**
	 * parse the line and store to out the
	 * value part of the property
	 *
	 * @param out:		the string where store the property readed
	 * @return whether the operation success
	 */
	bool value(std::string &out);

	/**
	 * read the next character and assigns it to <current>
	 */
	void next();
};

#endif
