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

#ifndef _ENVIRONMENT_
#define _ENVIRONMENT_

#include <map>
#include <string>

/*
 * class thta provide and contain environment variables
 */
class Environment {
public:
    // constructor, espected path to environment file
    Environment(std::string path) {
        envPath = path;
    }

    /*
     * load the environment
     */
    void load();

    /*
     *	get value from provided key
     */
    std::string getVariable(const std::string& key);

    /*
     * get al variables from map
     */
    std::map<std::string, std::string> getAllVariables();

    /*
     *	set variable, add or modify
     */
    void setVariable(const std::string& key, const std::string& value);

private:
    // map with variables
    std::map<std::string, std::string> environmentVariables;

    // path to environment file
    std::string envPath;
};

#endif
