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

#include <eva.h>
#include <fstream>
#include <utils/environment.hpp>
#include <utils/fparser.hpp>

using namespace std;

/**
 * get variable of associated key in environment variables
 */
string Environment::get(const string& key) {
    // open the environment file
    ifstream env("/cfg/environ/vars");

    // parse the content
    PropertyFileParser  parser(env);
    map<string, string> vars = parser.getProperties();

    // return the variable
    return vars[key];
}

/**
 *	set provided variable with provided key in environment variables loaded from mx shell
 */
void Environment::set(const string& key, const string& var) {
    // set string with comand + key + variable
    string request = "--setenv " + key + '=' + var;

    // exec shell
    SpawnStatus status = Spawn("/cmd/mx", request.c_str(), "/", SECURITY_LEVEL_APPLICATION);

    //// check exec state
    // if (status != SPAWN_STATUS_SUCCESSFUL) Utils::log("setEnvVar: failed to spawn mx process");
}

/**
 *	get logged user
 */
string Environment::getLoggedUser() {
    return Environment::get("USER");
}

/**
 *	get hostname
 */
string Environment::getHostname() {
    return Environment::get("HOSTNAME");
}

/**
 *	get theme
 */
string Environment::getTheme() {
    return Environment::get("THEME");
    ;
}
