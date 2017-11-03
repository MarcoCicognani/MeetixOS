/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 *                 Changes, a file change monitoring util                    *
 *                   Copyright (C) 2014, Max Schluessel                      *
 *                                                                           *
 *                                                                           *
 *  This program is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation, either version 3 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  This program is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __CHANGES__
#define __CHANGES__

#include "system.hpp"

#include <map>
#include <string>

// file created by tools to store the changes infos
#define DEFAULT_TABLEPATH		".changes"

/**
 * avaible commands
 */
#define FLAG_CHECK		"-c"
#define FLAG_CHECK_L	"--check"
#define FLAG_STORE		"-s"
#define FLAG_STORE_L	"--store"
#define FLAG_OUT		"-o"
#define FLAG_OUT_L		"--output"
#define FLAG_HELP		"--help"
#define FLAG_VERSION	"--version"
#define FLAG_CLEAR		"--clear"

/**
 * mode changes
 */
enum class ChangesMode : uint8_t
{
	NONE,
	STORE,
	CHECK,
	CLEAR
};

/**
 * print the title of the tool
 */
void title();

/**
 * show the help of the application
 *
 * @param self:		the name of the tool
 */
int help(char *self);

/**
 * show the version of the tool
 */
int version();

/**
 * show the error abount a non recognised parameter flag
 *
 * @param self:		the name of the tool
 * @param flag:		the non recognized flag
 * @return:			the value of the usage message
 */
int unknownFlag(char *self, const std::string &flag);

/**
 * show the usage message of the tool
 *
 * @param self:		the name of the tool
 * @return:			-1
 */
int usageMessage(char *self);

/**
 * parse the main arguments and return a map with parsed inputs
 *
 * @param argc:		the arguments count
 * @param argv:		the arguments provided
 * @return:			a map object with parsed arguments
 */
std::map<std::string, std::string> parseCommands(int argc, char *argv[]);

/**
 * store on map the changes of a file
 *
 * @param path:			the path of the file
 * @param tablepath:	the changes table file
 */
void store(const std::string &path, const std::string &tablepath);

/**
 * check if a file is changed
 *
 * @param path:			the path of the file
 * @param tablepath:	the changes table file
 */
int check(const std::string &path, const std::string &tablepath);

/**
 * clear the map of chenages of a directory
 *
 * @param tablepath:	the changes table to erase
 */
void clear(const std::string &tablepath);

/**
 * read the changes table of a path and return a filled map
 *
 * @param path:		the file path to read
 * @param target:	reference where the method store the readed changes
 */
void readChangeTable(const std::string &path, std::map<std::string, long> &target);

/**
 * save the changes table of a path and return a filled map
 *
 * @param path:		the file path to read
 * @param target:	reference where the method store the readed changes
 */
void saveChangeTable(const std::string &path, std::map<std::string, long> &target);

/**
 * absolutize the path
 *
 * @param path:		the relative path
 * @return:			the absolute path
 */
std::string getAbsolutePath(const std::string &path);

/**
 * get the last modify value of a file
 *
 * @param path:		the file path to check
 * @return:			the last modify value
 */
long getLastModifyDate(const std::string &path);

#endif
