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

#include <sstream>
#include <utils/arguments.hpp>

/**
 * return a list with non filtered options from main arguments
 */
llist<std::string> Arguments::getNonFiltered(int argc, const char* argv[]) {
    // create local list
    llist<std::string> nonfiltered;

    // parse all arguments
    for ( int arg = 0; arg < argc; arg++ )
        if ( argv[arg][0] != '-' )
            nonfiltered.add(std::string(argv[arg]));

    return nonfiltered;
}

/**
 * return a list with filtered options from main arguments
 */
llist<std::string> Arguments::getFiltered(int argc, const char* argv[]) {
    // create local list
    llist<std::string> filtered;

    // parse all arguments
    for ( int arg = 0; arg < argc; arg++ )
        if ( argv[arg][0] == '-' )
            filtered.add(std::string(argv[arg]));

    return filtered;
}

/**
 * split on out parameter the provided string from provided delimiter
 */
template<typename T>
void Arguments::split(const std::string& str, char delim, T result) {
    std::stringstream ss(str);
    std::string       item;
    while ( std::getline(ss, item, delim) )
        *(result++) = item;
}

/**
 * split the provided string with delimiters on vector
 */
std::vector<std::string> Arguments::split(const std::string& str, char delim) {
    std::vector<std::string> elems;
    split(str, delim, std::back_inserter(elems));
    return elems;
}
