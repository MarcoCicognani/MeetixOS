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

#include "SecondaryThread.hpp"

using namespace std;

/**
 *	display memory usage in CandyGUI menu
 */
void SecondaryThread::MemoryUsageThread(Label *memLabel)
{
	// infinite iteration
	while (true)
	{
		// stringstream convertor
		stringstream usage;

		// call sysinfo
		EvaSysInfo memUsage;
		Sysinfo(&memUsage);

		// fill stringstream with data
		usage << (memUsage.totRam - memUsage.freeRam) / 1024 << "/"  << memUsage.totRam / 1024  << "MB";

		// and display in screen label
		memLabel->setTitle(usage.str());

		// wait 50 milliseconds
		Sleep(50);
	}
}
