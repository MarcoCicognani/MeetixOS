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

#include <GUI/Component/Component.hh>
#include <GUI/Component/ComponentRegistry.hh>
#include <ranges>
#include <Tasking/LockGuard.hh>

ComponentRegistry& ComponentRegistry::instance() {
    static ComponentRegistry* s_instance = nullptr;
    if ( !s_instance )
        s_instance = new ComponentRegistry{};
    return *s_instance;
}

/**
 * Add a component object to client registry
 */
void ComponentRegistry::add(Component* component) {
    Tasking::LockGuard guard{ m_lock };

    m_registry[component->id()] = component;
}

/*
 * Remove a component by m_command (remove only the reference on client map, not delete the object)
 */
void ComponentRegistry::remove(UiComponentID id) {
    Tasking::LockGuard guard{ m_lock };

    auto it = m_registry.find(id);
    if ( it != m_registry.end() )
        m_registry.erase(it);
}

/**
 * return the object by m_command
 */
Component* ComponentRegistry::get(UiComponentID id) {
    Tasking::LockGuard guard{ m_lock };

    auto it = m_registry.find(id);
    if ( it != m_registry.end() )
        return it->second;
    else
        return nullptr;
}

/**
 * parse all client registry and delete all the object
 * the deletion of client component object remove itself from client map and windowserver map
 */
void ComponentRegistry::destroy() {
    Tasking::LockGuard guard{ m_lock };

    // start from end
    for ( auto& rev_it : std::ranges::reverse_view(m_registry) )
        delete rev_it.second;
}
