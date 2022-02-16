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
#include <LibTasking/LockGuard.hh>

using namespace std;

ComponentRegistry& ComponentRegistry::instance() {
    static ComponentRegistry* s_instance = nullptr;
    if ( !s_instance ) {
        s_instance = new ComponentRegistry{};
    }

    return *s_instance;
}

/**
 *
 */
UiComponentID ComponentRegistry::add(Pid process, Component_t* component) {
    Tasking::LockGuard lock_guard{ m_lock };

    UiComponentID id                     = m_next_id++;
    m_registry[id]                       = component;
    m_components_by_process[process][id] = component;
    component->id                        = id;
    return id;
}

/**
 *
 */
Component_t* ComponentRegistry::get(UiComponentID id) {
    Tasking::LockGuard lock_guard{ m_lock };

    auto it = m_registry.find(id);
    if ( it != m_registry.end() )
        return it->second;
    else
        return nullptr;
}

/**
 *
 */
void ComponentRegistry::removeComponent(Pid pid, UiComponentID id) {
    Tasking::LockGuard lock_guard{ m_lock };

    if ( m_registry.count(id) > 0 ) {
        if ( m_components_by_process.count(pid) > 0 )
            m_components_by_process[pid].erase(m_components_by_process[pid].find(id));

        m_registry.erase(m_registry.find(id));
    }
}

void ComponentRegistry::cleanup_process(Pid pid) {
    map<UiComponentID, Component_t*>* components;
    {
        Tasking::LockGuard lock_guard{ m_lock };
        components = &m_components_by_process[pid];
    }

    if ( components ) {
        std::list<Component_t*> components_list{};

        for ( auto& entry : *components ) {
            auto component = entry.second;
            if ( component
                 && std::find(components_list.begin(), components_list.end(), component) == components_list.end() ) {
                components_list.push_back(component);
            }
        }

        std::list<Component_t*> removed_components;

        while ( !components_list.empty() ) {
            auto component = components_list.back();
            components_list.pop_back();

            if ( component->isWindow() ) {
                remove_process_components(pid, component, removed_components);

                for ( auto removed : removed_components )
                    components_list.remove(removed);
            }
        }

        {
            Tasking::LockGuard lock_guard{ m_lock };
            m_components_by_process.erase(m_components_by_process.find(pid));
        }
    }
}

void ComponentRegistry::remove_process_components(Pid                      pid,
                                                  Component_t*             component,
                                                  std::list<Component_t*>& removed_components) {
    // Never remove twice
    if ( std::find(removed_components.begin(), removed_components.end(), component) != removed_components.end() ) {
        return;
    }

    // Hide it
    component->setVisible(false);

    // Remove this component
    removed_components.push_back(component);

    // Recursively remove all child elements first
    for ( auto child_ref : component->getChildren() )
        remove_process_components(pid, child_ref.m_component, removed_components);

    // Remove from registry
    removeComponent(pid, component->id);

    // Remove from parent
    auto parent       = component->getParent();
    bool canBeDeleted = true;
    if ( parent ) {
        // Only allow to really "delete" children that are default referenced
        ChildComponentRef childReference{};
        if ( parent->getChildReference(component, childReference) ) {
            canBeDeleted = (childReference.m_ref_type == ChildComponentRefType::Default);
        }

        // Remove from parent
        parent->removeChild(component);
    }

    // Finally, delete it
    if ( canBeDeleted ) {
        delete component;
    }
}

/**
 *
 */
std::list<Window_t*> ComponentRegistry::getWindowsComponents() {
    // list where copy objects
    std::list<Window_t*> listOfWindows;

    // parse all regex
    for ( std::pair<UiComponentID, Component_t*> current : m_registry ) {
        Component_t* component = current.second;

        if ( component->getComponentType() == UI_COMPONENT_TYPE_WINDOW ) {
            // cast component
            Window_t* casted = reinterpret_cast<Window_t*>(component);

            // and if component is template type provided add to list
            if ( casted )
                listOfWindows.push_back(casted);
        }
    }

    return listOfWindows;
}
