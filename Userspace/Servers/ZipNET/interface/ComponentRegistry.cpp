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
    UiComponentID id                 = nextID++;
    m_registry[id]                   = component;
    componentsByProcess[process][id] = component;
    component->id                    = id;
    return id;
}

/**
 *
 */
Component_t* ComponentRegistry::get(UiComponentID id) {
    if ( m_registry.count(id) > 0 )
        return m_registry[id];
    return 0;
}

/**
 *
 */
map<UiComponentID, Component_t*>* ComponentRegistry::getProcessMap(Pid pid) {
    if ( componentsByProcess.count(pid) > 0 )
        return &componentsByProcess[pid];
    return nullptr;
}

/**
 *
 */
bool ComponentRegistry::removeComponent(Pid pid, UiComponentID id) {
    if ( m_registry.count(id) > 0 ) {
        // get components and set unvisible
        Component_t* component = m_registry[id];
        component->setVisible(false);

        // remove from process map
        if ( componentsByProcess.count(pid) > 0 )
            componentsByProcess[pid].erase(componentsByProcess[pid].find(id));

        // remove from global components
        m_registry.erase(m_registry.find(id));

        return true;
    }

    return false;
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

/**
 *
 */
void ComponentRegistry::removeProcessMap(Pid pid) {
    componentsByProcess.erase(pid);
}
