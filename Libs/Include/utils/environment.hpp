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

#ifndef MEETIX_LIBRARY_UTILS_ENVIRONMENT
#define MEETIX_LIBRARY_UTILS_ENVIRONMENT

#include <string>

/**
 *	static class to interact with the environment variables
 */
class Environment
{
private:
	/**
	 * remove constructor
	 */
	Environment() = delete;
public:
	/**
	 *	get provided variable with provided key in environment variables loaded from mx shell
	 */
	static std::string get(const std::string &var);

	/**
	 *	set provided variable with provided key in environment variables loaded from mx shell
	 */
	static void set(const std::string &key, const std::string &value);

	/**
	 *	get logged user
	 */
	static std::string getLoggedUser();

	/**
	 *	get hostname
	 */
	static std::string getHostname();

	/**
	 *	get theme
	 */
	static std::string getTheme();
};

#endif
