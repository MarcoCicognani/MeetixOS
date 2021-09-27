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

#include <gui/component.hpp>
#include <gui/componentregistry.hpp>

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
    m_lock.lock();
    m_registry[component->getID()] = component;
    m_lock.unlock();
}

/*
 * Remove a component by id (remove only the reference on client map, not delete the object)
 */
void ComponentRegistry::remove(UiComponentID id) {
    if ( m_registry.count(id) > 0 ) {
        m_lock.lock();
        m_registry.erase(id);
        m_lock.unlock();
    }
}

/**
 * return the object by id
 */
Component* ComponentRegistry::get(UiComponentID id) {
    if ( m_registry.count(id) > 0 ) {
        m_lock.lock();
        auto component = m_registry[id];
        m_lock.unlock();

        return component;
    }
    return nullptr;
}

/**
 * parse all client registry and delete all the object
 * the deletion of client component object remove itself from client map and windowserver map
 */
void ComponentRegistry::deleteRegistry() {
    m_lock.lock();

    // start from end
    for ( auto rev_it = m_registry.rbegin(); rev_it != m_registry.rend(); ++rev_it )
        delete rev_it->second;

    m_lock.unlock();
}
