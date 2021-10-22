/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include "../../../../Toolchain/Local/i686-pc-meetix/include/c++/11.2.0/map"
#include "../../Tasking/Lock.hh"
#include "../Protocol.hh"

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
