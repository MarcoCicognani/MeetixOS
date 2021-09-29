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

#include <Utils/Time.hh>

using namespace std;

/**
 *	Display hour to hour label
 */
void SecondaryThread::HourManagerThread(Label* hourLabel) {
    // infinite iteration
    while ( true ) {
        // fill time struct
        Utils::Time::Current time;
        Utils::Time::current(time);

        // write data to stringstream
        stringstream hourStr;
        hourStr << time.m_hour << ':' << time.m_minute << ':' << time.m_second;

        // and display on screen label
        hourLabel->setTitle(hourStr.str());

        // wait 1 second
        s_sleep(1000);
    }
}
