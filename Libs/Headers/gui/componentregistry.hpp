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
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef MEETIX_LIBRARY_UI_COMPONENT_REGISTRY
#define MEETIX_LIBRARY_UI_COMPONENT_REGISTRY

#include <gui/uispech.hpp>
#include <map>
#include <Tasking/Lock.hh>

// forward declaration
class Component;

/**
 * client method for gestion of component object interfaces
 */
class ComponentRegistry {
public:
    static ComponentRegistry& instance();

    void       add(Component* component);
    void       remove(UiComponentID id);
    Component* get(UiComponentID id);
    void       deleteRegistry();

private:
    ComponentRegistry() = default;

    Tasking::Lock                       m_lock{};
    std::map<UiComponentID, Component*> m_registry{};
};

#endif
