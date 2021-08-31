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

#include "ComponentRegistry.hpp"
#include <map>
#include <unordered_map>

using namespace std;

static map<UiComponentID, Component_t*> components;
static map<Pid, map<UiComponentID, Component_t*>> componentsByProcess;
static UiComponentID nextID = 1;

/**
 *
 */
UiComponentID ComponentRegistry_t::add(Pid process, Component_t *component)
{
	UiComponentID id = nextID++;
	components[id] = component;
	componentsByProcess[process][id] = component;
	component->id = id;
	return id;
}

/**
 *
 */
Component_t *ComponentRegistry_t::get(UiComponentID id)
{
	if (components.count(id) > 0) return components[id];
	return 0;
}

/**
 *
 */
map<UiComponentID, Component_t*> *ComponentRegistry_t::getProcessMap(Pid pid)
{
	if (componentsByProcess.count(pid) > 0) return &componentsByProcess[pid];
	return nullptr;
}

/**
 *
 */
bool ComponentRegistry_t::removeComponent(Pid pid, UiComponentID id)
{
	if (components.count(id) > 0)
	{
		// get components and set unvisible
		Component_t *component = components[id];
		component->setVisible(false);

		// remove from process map
		if (componentsByProcess.count(pid) > 0) componentsByProcess[pid].erase(componentsByProcess[pid].find(id));

		// remove from global components
		components.erase(components.find(id));

		return true;
	}

	return false;
}

/**
 *
 */
std::list<Window_t*> ComponentRegistry_t::getWindowsComponents()
{
	// list where copy objects
	std::list<Window_t*> listOfWindows;

	// parse all regex
	for (std::pair<UiComponentID, Component_t*> current : components)
	{
		Component_t *component = current.second;

		if (component->getComponentType() == UI_COMPONENT_TYPE_WINDOW)
		{
			// cast component
			Window_t *casted = reinterpret_cast<Window_t*>(component);

			//and if component is template type provided add to list
			if (casted) listOfWindows.push_back(casted);
		}
	}

	return listOfWindows;
}

/**
 *
 */
void ComponentRegistry_t::removeProcessMap(Pid pid)
{
	componentsByProcess.erase(pid);
}
