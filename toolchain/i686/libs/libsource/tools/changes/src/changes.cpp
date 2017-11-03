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

#include "changes.hpp"

#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>

/**
 *
 */
int main(int argc, char *argv[])
{
	// parse the arguments
	std::map<std::string,std::string> params = parseCommands(argc, argv);

	// copy the name of the tool
	char *self = argv[0];

	// create a default mode
	ChangesMode mode = ChangesMode::NONE;

	// create string to store file path and table
	std::string file;
	std::string tablepath = DEFAULT_TABLEPATH;

	// read parameters
	for (std::pair<std::string, std::string> entry : params)
	{
		// check the changes of a file
		if (entry.first == FLAG_CHECK || entry.first == FLAG_CHECK_L)
		{
			// mode is already assigned
			if (mode != ChangesMode::NONE) return usageMessage(self);
			mode = ChangesMode::CHECK;
			file = entry.second;
		}

		// store a file changes
		else if (entry.first == FLAG_STORE || entry.first == FLAG_STORE_L)
		{
			// mode is already assigned
			if (mode != ChangesMode::NONE) return usageMessage(self);
			mode = ChangesMode::STORE;
			file = entry.second;
		}

		// clear a file changes file
		else if (entry.first == FLAG_CLEAR)
		{
			// mode is already assigned
			if (mode != ChangesMode::NONE) return usageMessage(self);
			mode = ChangesMode::CLEAR;
		}

		// change the name of tablepath name
		else if (entry.first == FLAG_OUT || entry.first == FLAG_OUT_L) tablepath = entry.second;
		else if (entry.first == FLAG_HELP) return help(self);
		else if (entry.first == FLAG_VERSION) return version();
		else return unknownFlag(self, entry.first);
	}

	// check the chosed mode
	if (mode == ChangesMode::NONE)
	{
		return usageMessage(self);
	}

	// check if file exists
	std::string absolute = getAbsolutePath(file);
	if (mode == ChangesMode::CHECK || mode == ChangesMode::STORE)
	{
		if (access(absolute.c_str(), F_OK) == -1)
		{
			fprintf(stderr, "error: '%s' does not exist\n", absolute.c_str());
			return -1;
		}
	}

	// check the file changes
	if (mode == ChangesMode::CHECK) return check(absolute, tablepath);

	// store modify
	else if (mode == ChangesMode::STORE)
	{
		store(absolute, tablepath);
		return 0;
	}

	// clear file changes
	else if (mode == ChangesMode::CLEAR)
	{
		clear(tablepath);
		return 0;
	}
}

/**
 * parse the main arguments and return a map with parsed inputs
 *
 * @param argc:		the arguments count
 * @param argv:		the arguments provided
 * @return:			a map object with parsed arguments
 */
std::map<std::string, std::string> parseCommands(int argc, char *argv[])
{
	// create a local map to store parsed args
	std::map<std::string, std::string> values;

	// create variables to store flag commands
	std::string flag;
	bool atflag = true;

	// parse each args excepts the first, the filename
	for (int i = 1; i < argc; i++)
	{
		// copy to a string object the current arguments
		std::string param(argv[i]);

		// non valid parameter
		if (!param.size()) continue;

		// check recognized flag
		if (atflag)
		{
			// copy string
			flag = param;

			// ignored flags
			if (flag == FLAG_HELP || flag == FLAG_VERSION || flag == FLAG_CLEAR) values[flag] = "";
			else atflag = false;
		}

		// copy the entire parameter
		else
		{
			values[flag] = param;
			atflag = true;
		}
	}

	// close the last
	if (!atflag) values[flag] = "";

	return values;
}

/**
 * store on map the changes of a file
 *
 * @param path:			the path of the file
 * @param tablepath:	the changes table file
 */
void store(const std::string &path, const std::string &tablepath)
{
	// get the last modify
	long lastModify = getLastModifyDate(path);

	// read the changes table
	std::map<std::string, long> entries;
	readChangeTable(tablepath, entries);

	// save the changes
	entries[path] = lastModify;
	saveChangeTable(tablepath, entries);
}

/**
 * check if a file is changed
 *
 * @param path:			the path of the file
 * @param tablepath:	the changes table file
 */
int check(const std::string &path, const std::string &tablepath)
{
	// get last modify
	long lastModify = getLastModifyDate(path);

	// read changes table
	std::map<std::string, long> entries;
	readChangeTable(tablepath, entries);

	// check in each entry
	for (std::pair<std::string, long> entry : entries)
		if (path == entry.first) return (lastModify > entry.second) ? 1 : 0;
	return 1;
}

/**
 * clear the map of chenages of a directory
 *
 * @param tablepath:	the changes table to erase
 */
void clear(const std::string &tablepath)
{
	std::map<std::string, long> entries;
	readChangeTable(tablepath, entries);
	entries.clear();
	saveChangeTable(tablepath, entries);
}

/**
 * print the title of the tool
 */
void title()
{
	printf("CHANGES, a file change monitoring tool\n\n");
}

/**
 * show the help of the application
 *
 * @param self:		the name of the tool
 */
int help(char *self)
{
	title();

	printf("This program can be used to check if a file has changed. The following commands are available:\n");
	printf("\n");
	printf("   %s, %s\n", FLAG_STORE, FLAG_STORE_L);

	// -----################################################################################----- 80 chars
	printf("       saves the current modify date of the given file\n");
	printf("   %s, %s\n", FLAG_CHECK, FLAG_CHECK_L);
	printf("       returns 1 if the specified file has changed since the last use of store,\n");
	printf("       returns 0 if it did not change\n");
	printf("   %s\n", FLAG_CLEAR);
	printf("       removes all entries from the change table\n");
	printf("   %s, %s\n", FLAG_OUT, FLAG_OUT_L);
	printf("       sets the location of the used change table\n");
	printf("   %s\n", FLAG_HELP);
	printf("       displays this help message\n");
	printf("   %s\n", FLAG_VERSION);
	printf("       displays the current version\n");
	printf("\n");
	printf("Modify dates are saved in a file named '%s' by default. To change the path for this file, use '%s alternatename'.\n", DEFAULT_TABLEPATH, FLAG_OUT);
	printf("\n");
	printf("Examples:\n");
	printf("   %s %s test.txt\n", self, FLAG_STORE);
	printf("       stores the current modify date for the file\n");
	printf("   %s %s test.txt\n", self, FLAG_CHECK);
	printf("       returns whether the file was modified since the last store\n");
	printf("   %s %s test.txt %s .mytable\n", self, FLAG_STORE, FLAG_OUT);
	printf("       stores the current modify date in the specified table file\n");
	printf("\n");
	printf("  - Max Schluessel");
	printf("\n");

	return 0;
}

/**
 * show the version of the tool
 */
int version()
{
	title();
	printf("version 0.2\n");
	return 0;
}

/**
 * show the usage message of the tool
 *
 * @param self:		the name of the tool
 * @return:			-1
 */
int usageMessage(char *self)
{
	std::stringstream f;
	f << "[" << FLAG_CHECK << "," << FLAG_STORE << "]";
	fprintf(stderr, "usage:\n\t%s %s file\n", self, f.str().c_str());
	fprintf(stderr, "use '%s' for more information", FLAG_HELP);
	return -1;
}

/**
 * show the error abount a non recognised parameter flag
 *
 * @param self:		the name of the tool
 * @param flag:		the non recognized flag
 * @return:			the value of the usage message
 */
int unknownFlag(char *self, const std::string &flag)
{
	fprintf(stderr, "error: unknown flag '%s'\n", flag.c_str());
	return usageMessage(self);
}

/**
 * absolutize the path
 *
 * @param path:		the relative path
 * @return:			the absolute path
 */
std::string getAbsolutePath(const std::string &path)
{
	char *buf = new char[PATH_MAX];

#if WINDOWS
	GetFullPathName(path.c_str(), PATH_MAX, buf, 0);
#elif MAC_OS or LINUX
	realpath(path.c_str(), buf);
#endif

	std::string absolute = buf;
	delete buf;
	return absolute;
}

/**
 * get the last modify value of a file
 *
 * @param path:		the file path to check
 * @return:			the last modify value
 */
long getLastModifyDate(const std::string &path)
{
	struct stat attrib;
	stat(path.c_str(), &attrib);
	tm modifyTime = *gmtime(&(attrib.st_mtime));
	return mktime(&modifyTime);
}

/**
 * read the changes table of a path and return a filled map
 *
 * @param path:		the file path to read
 * @param target:	reference where the method store the readed changes
 */
void readChangeTable(const std::string &path, std::map<std::string, long> &target)
{
	// open the file for reading
	std::ifstream in(path);

	// check stat
	if (!in.is_open()) return;

	// read the file
	std::string line;
	while (getline(in, line))
	{
		int eqpos = line.find_last_of("=");
		if (eqpos != -1)
		{
			std::string filepath = line.substr(0, eqpos);
			std::string timestamps = line.substr(eqpos + 1);

			std::stringstream s;
			s << timestamps;
			long timestamp;
			s >> timestamp;

			if (!filepath.empty() && !timestamps.empty())
				target[filepath] = timestamp;
		}
	}

	// close the file
	in.close();
}

/**
 * save the changes table of a path and return a filled map
 *
 * @param path:		the file path to read
 * @param target:	reference where the method store the readed changes
 */
void saveChangeTable(const std::string &path, std::map<std::string, long> &target)
{
	// open the file for writing
	std::ofstream out(path);

	// check open status
	if (!out.is_open()) return;

	// write each map node
	for (std::pair<std::string, long> entry : target)
		out << entry.first << "=" << entry.second << "\n";

	// close the file
	out.close();
}
