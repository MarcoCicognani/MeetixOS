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

#include "OsmosUI.hpp"
#include "SecondaryThread.hpp"

#include <gui/btnlist.hpp>
#include <utils/environment.hpp>

using namespace std;

/**
 *	Paint dock on screen
 */
void SecondaryThread::OsmosUIDockThread(const Dimension& resolution) {
    // creating button list
    ButtonList* dockButtons;

    // create dock geoshape
    Geoshape* dock = Geoshape::create();
    dock->setBounds(Rectangle(resolution.width / 2 - 197, resolution.height - 64, 394, 64));
    dock->setColor(ARGB(120, 0, 0, 0), ARGB(255, 0, 0, 0));

    // creating button and add to list
    dockButtons = new ButtonList();
    dockButtons->add("shell", [] {
        Spawn("/app/CandyShell/bin/CandyShell", "", "/app/CandyShell/", SECURITY_LEVEL_APPLICATION);
    });
    dockButtons->configure("shell",
                           Rectangle(30, 7, 48, 48),
                           "",
                           "/app/CandyShell/deps/icon/ico.png",
                           Point(0, 0),
                           ARGB(0, 0, 0, 0),
                           ARGB(255, 0, 0, 0));

    // creating button and add to list
    dockButtons->add("calculator", [] {
        Spawn("/app/calculator/bin/calculator", "", "/app/calculator/", SECURITY_LEVEL_APPLICATION);
    });
    dockButtons->configure("calculator",
                           Rectangle(125, 7, 48, 48),
                           "",
                           "/app/calculator/deps/icon/ico.png",
                           Point(0, 0),
                           ARGB(0, 0, 0, 0),
                           ARGB(255, 0, 0, 0));

    // creating button and add to list
    dockButtons->add("editor", [] {
        Spawn("/app/CandyNote/bin/CandyNote", "", "/app/CandyNote/", SECURITY_LEVEL_APPLICATION);
    });
    dockButtons->configure("editor",
                           Rectangle(220, 7, 48, 48),
                           "",
                           "/app/CandyNote/deps/icon/ico.png",
                           Point(0, 0),
                           ARGB(0, 0, 0, 0),
                           ARGB(255, 0, 0, 0));

    // creating button and add to list
    dockButtons->add("shutdowns", meetiXOSLogout);
    dockButtons->configure("shutdowns",
                           Rectangle(315, 7, 48, 48),
                           "",
                           "/app/OsmosUI/deps/icon/shut.png",
                           Point(0, 0),
                           ARGB(0, 0, 0, 0),
                           ARGB(255, 0, 0, 0));

    // add all buttons to dock
    dockButtons->show(dock);

    // lock thread
    uint8_t blocker = true;
    AtomicBlock(&blocker);

    // remove screen objects
    delete dockButtons;
    delete dock;
}
