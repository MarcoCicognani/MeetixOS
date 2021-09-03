/*********************************************************************************
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
* 																			     *
* This program is free software; you can redistribute it and/or                  *
* modify it under the terms of the GNU General Public License                    *
* as published by the Free Software Foundation; either version 2				 *
* of the License, or (char *argumentat your option) any later version.			 *
*																				 *
* This program is distributed in the hope that it will be useful,				 *
* but WITHout ANY WARRANTY; without even the implied warranty of                 *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#include <gui/componentregistry.hpp>
#include <gui/component.hpp>
#include <tasking/lock.hpp>

static Lock componentsLock;
static std::map<UiComponentID, Component*> components;

/**
 * Add a component object to client registry
 */
void ComponentRegistry::add(Component *component)
{
	componentsLock.lock();
	components[component->getID()] = component;
	componentsLock.unlock();
}

/*
 * Remove a component by id (remove only the reference on client map, not delete the object)
 */
void ComponentRegistry::remove(UiComponentID id)
{
	if (components.count(id) > 0)
	{
		componentsLock.lock();
		components.erase(id);
		componentsLock.unlock();
	}
}

/**
 * return the object by id
 */
Component *ComponentRegistry::get(UiComponentID id)
{
	if (components.count(id) > 0)
	{
		componentsLock.lock();
		Component *component = components[id];
		componentsLock.unlock();

		return component;
	}
	return 0;
}

/**
 * parse all client registry and delete all the object
 * the deletion of client component object remove itself from client map and windowserver map
 */
void ComponentRegistry::deleteRegistry()
{
	componentsLock.lock();

	// start from end
	for (std::map<UiComponentID, Component*>::reverse_iterator rit = components.rbegin(); rit != components.rend(); ++rit)
		delete rit->second;

	componentsLock.unlock();
}
