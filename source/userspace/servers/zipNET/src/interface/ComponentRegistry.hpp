/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
*                                                                                     *
*         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
*         This software is derived from the Ghost operating system project,           *
*         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
*         https://ghostkernel.org/                                                    *
*         https://github.com/maxdev1/ghost                                            *
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

#ifndef __COMPONENT_REGISTRY__
#define __COMPONENT_REGISTRY__

#include <tasking/thread.hpp>
#include <components/component.hpp>
#include <components/window.hpp>
#include <gui/uispech.hpp>
#include <list>

using namespace std;

class Window_t;

/**
 *
 */
class ComponentRegistry_t
{
public:
	static UiComponentID add(Pid process, Component_t *component);
	static Component_t *get(UiComponentID id);
	static std::list<Window_t*> getWindowsComponents();
	static map<UiComponentID, Component_t*> *getProcessMap(Pid pid);

	static bool removeComponent(Pid pid, UiComponentID id);
	static void removeProcessMap(Pid pid);
};

#endif
