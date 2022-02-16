/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <LibGUI/Component/Component.hh>
#include <LibGUI/Component/ComponentRegistry.hh>
#include <LibTasking/LockGuard.hh>
#include <ranges>

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
