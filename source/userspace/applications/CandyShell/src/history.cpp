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

#include <history.hpp>

/*
 *
 */
History::History() : howPrevious(0), numOfHistoredCommands(0) {}

/**
 * Function to initialize history
 */
void History::initHistory()
{
	container.clear();
}

/**
 * Function to add command to history
 */
void History::addHistory(string newCommand)
{
	container.push_back(newCommand);
	it = container.end();
	howPrevious = 1;
	numOfHistoredCommands++;
}

/**
 * Function to get command from history
 */
string History::getHistory()
{
	if (howPrevious <= numOfHistoredCommands)
	{
		previousCommand = prev(it, howPrevious);
		howPrevious++;

		return *previousCommand;
	}

	else return string("");
}
