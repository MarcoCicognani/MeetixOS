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

#ifndef MEETIX_LIBRARY_UTILS_ARGUMENTS
#define MEETIX_LIBRARY_UTILS_ARGUMENTS

#include <eva/utils/llist.hpp>
#include <string>
#include <vector>

/**
 * static class to manage arguments
 */
class Arguments {
private:
    /**
     * remove constructor
     */
    Arguments() = delete;

public:
    /**
     * return a list with non filtered options from main arguments
     */
    static llist<std::string> getNonFiltered(int argc, const char* argv[]);

    /**
     * return a list with filtered options from main arguments
     */
    static llist<std::string> getFiltered(int argc, const char* argv[]);

    /**
     * split on out parameter the provided string from provided delimiter
     */
    template<typename T>
    static void split(const std::string& str, char delim, T result);

    /**
     * split the provided string with delimiters on vector
     */
    static std::vector<std::string> split(const std::string& str, char delim);
};

#endif
