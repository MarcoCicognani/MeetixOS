/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
*                                                                                     *
* This program is free software; you can redistribute it and/or                       *
* modify it under the terms of the GNU General Public License                         *
* as published by the Free Software Foundation; either version 2                      *
* of the License, or (char *argumentat your option) any later version.                *
*                                                                                     *
* This program is distributed in the hope that it will be useful,                     *
* but WITHout ANY WARRANTY; without even the implied warranty of                      *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
* GNU General Public License for more details.                                        *
*                                                                                     *
* You should have received a copy of the GNU General Public License                   *
* along with this program; if not, write to the Free Software                         *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include "environment.hpp"
#include <fstream>
#include <libgen.h>
#include <utils/fparser.hpp>

using namespace std;

/*
 * load the environment file from path provided by constructor
 */
void Environment::load()
{
	// open environment file
	ifstream file(envPath);

	// parse it and get map with variables
	PropertyFileParser parser(file);
	map<string, string> variables = parser.getProperties();

	// copy into local map variables from file
	for (pair<string, string> cop : variables)
		environmentVariables[cop.first] = variables[cop.first];

	// closing file
	file.close();
}


/*
 *	get value from provided key
 */
string Environment::getVariable(const string &key)
{
	return environmentVariables[key];
}


/*
 * get al variables from map
 */
map<string, string> Environment::getAllVariables()
{
	return environmentVariables;
}

/*
 * set or add variable
 */
void Environment::setVariable(const string &key, const string &value)
{
	// set on local variables
	environmentVariables[key] = value;

	// write on environment file
	ofstream envFile(envPath);
	for (std::pair<string, string> variable : environmentVariables)
	{
		envFile << variable.first + '=' + variable.second + '\n';
	}
	envFile.close();
}
