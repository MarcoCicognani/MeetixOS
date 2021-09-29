/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani & D. Morandi]                  *
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
#include <io/files/futils.hpp>
#include <sstream>
#include <utils/fparser.hpp>
#include <utils/utils.hpp>

/**
 * filled constructor
 *
 * @param t:	the filestream opened to the property file
 */
PropertyFileParser::PropertyFileParser(std::ifstream& t) {
    // get all the file content
    std::stringstream buffer;
    buffer << t.rdbuf();

    // close the file
    t.close();

    // initialize the parser
    initialize(buffer.str());
}

/**
 * filled constructor
 *
 * @param content:	the content of the file
 */
PropertyFileParser::PropertyFileParser(const std::string& _content) {
    initialize(_content);
}

/**
 * @return the parsed properties map of the file provided
 */
std::map<std::string, std::string> PropertyFileParser::getProperties() {
    // create a local map to store properties
    std::map<std::string, std::string> properties;

    // parse each properties
    while ( true ) {
        std::string a, b;
        if ( propertyEntry(a, b) )
            properties.insert(std::make_pair(a, b));
        else
            break;
    }

    return properties;
}

/**
 * initializes the parser from a file content
 *
 * @param content:		the content of the file
 */
void PropertyFileParser::initialize(const std::string& _content) {
    m_current_c    = -1;
    m_buf_position = 0;
    m_content      = _content;

    next();
}

/**
 * fill the strings provided with the current line values
 *
 * @param key:		the first string where store to
 * @param value:	the second string where store to
 * @return whether the operation success
 */
bool PropertyFileParser::propertyEntry(std::string& k, std::string& v) {
    // Key
    if ( !key(k) )
        return false;

    // = or :
    next();

    // Value
    if ( !value(v) )
        return false;

    return true;
}

/**
 * parse the line and store to out the
 * key part of the property
 *
 * @param out:		the string where store the property readed
 * @return whether the operation success
 */
bool PropertyFileParser::key(std::string& out) {
    auto len_1           = out.length();
    auto is_empty_1      = out.empty();
    auto is_empty_size_1 = out.size() == 0;
    auto is_not_empty_1  = !out.empty();
    out.clear();
    auto len_2           = out.length();
    auto is_empty_2      = out.empty();
    auto is_empty_size_2 = out.size() == 0;
    auto is_not_empty_2  = !out.empty();

    while ( m_current_c != -1 && m_current_c != '=' && m_current_c != ':' ) {
        out += m_current_c;
        next();
    }

    auto len_3           = out.length();
    auto is_empty_3      = out.empty();
    auto is_empty_size_3 = out.size() == 0;
    auto is_not_empty_3  = !out.empty();

    out = Utils::trim(out);

    auto len_4           = out.length();
    auto is_empty_4      = out.empty();
    auto is_empty_size_4 = out.size() == 0;
    auto is_not_empty_4  = !out.empty();

    return is_not_empty_4;
}

/**
 * parse the line and store to out the
 * value part of the property
 *
 * @param out:		the string where store the property readed
 * @return whether the operation success
 */
bool PropertyFileParser::value(std::string& out) {
    out.clear();
    while ( m_current_c != -1 && m_current_c != '\n' ) {
        out += m_current_c;
        next();
    }

    out = Utils::trim(out);

    if ( out.length() > 1 && out[0] == '"' && out[out.length() - 1] == '"' )
        out = out.substr(1, out.length() - 2);

    return !out.empty();
}

/**
 * read the next character and assigns it to <current>
 */
void PropertyFileParser::next() {
    if ( m_buf_position >= m_content.length() ) {
        m_current_c = -1;
        return;
    }

    m_current_c = m_content[m_buf_position++];
}
