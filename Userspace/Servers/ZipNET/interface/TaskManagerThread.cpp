/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * *
 * * * * * MeetiX OS By MeetiX OS Project [Marco Cicognani] *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or * modify it
 * under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2 * of the
 * License, or (char *argumentat your option) any later version. *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful, * but WITHout
 * ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the * GNU General
 * Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License * along
 * with this program; if not, write to the Free Software * Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * *
 * * * * */

#include <interface/ComponentRegistry.hpp>
#include <interface/TaskManagerThread.hpp>
#include <list>
#include <sstream>

/*
 *
 */
TaskManagerThread_t::TaskManagerThread_t(Label_t* where, const Rectangle& bounds)
    : internal(where), bounds(bounds) {
}

/*
 *
 */
void TaskManagerThread_t::run() {
    TaskRegisterID("taskManager");

    internal->setVisible(true);
    internal->setColor(0, RGB(255, 255, 255));

    while ( true ) {
        list<Window_t*> windows = ComponentRegistry::instance().getWindowsComponents();
        stringstream    titles;

        for ( Window_t* current : windows )
            titles << " | " << current->getTitle() << " | ";

        internal->setTitle(titles.str());
        Sleep(100);
    }
}
