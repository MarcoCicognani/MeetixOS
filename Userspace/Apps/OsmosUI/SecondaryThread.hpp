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

#ifndef _SECONDARY_THREAD_
#define _SECONDARY_THREAD_

#include "OsmosUI.hpp"

/*
 * secondary thread for candyGUI
 */
class SecondaryThread {
public:
    /*
     *	update every minute the hour label
     */
    static void HourManagerThread(Label* hourLabel);

    /*
     *	update memory usage every minute
     */
    static void MemoryUsageThread(Label* memLabel);

    /*
     *	create a application dock
     */
    static void OsmosUIDockThread(const Dimension& resolution);

    /*
     * receive and show the notification from other user application
     */
    static void NotificationThread();
};

#endif
