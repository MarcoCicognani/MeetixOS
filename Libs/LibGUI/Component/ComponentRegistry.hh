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

#pragma once

#include <LibGUI/Protocol.hh>
#include <LibTasking/Lock.hh>
#include <map>

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
    void       destroy();

private:
    ComponentRegistry() = default;

    Tasking::Lock                       m_lock{};
    std::map<UiComponentID, Component*> m_registry{};
};
