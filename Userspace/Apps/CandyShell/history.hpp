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

#ifndef _HISTORY_
#define _HISTORY_

#include "login.hpp"
#include "standardIO.hpp"

#include <Api.h>
#include <iterator>
#include <vector>

using namespace std;

class History {
private:
    vector<string>                                        container;
    vector<string>::iterator                              it;
    __gnu_cxx::__normal_iterator<string*, vector<string>> previousCommand;
    int                                                   howPrevious;
    int                                                   numOfHistoredCommands;

public:
    History();

    /**
     * Function to initialize history
     */
    void initHistory();

    /**
     * Function to add command to history
     */
    void addHistory(string newCommand);

    /**
     * Function to get command from history
     */
    string getHistory();
};

#endif
