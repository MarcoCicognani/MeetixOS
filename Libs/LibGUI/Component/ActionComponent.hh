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

#include <Api.h>
#include <cstdint>
#include <LibGUI/Listener/ActionListener.hh>
#include <LibGUI/Protocol.hh>

/**
 * Component that is capable of receiving action events
 */
class ActionComponent {
private:
    UiComponentID id;

protected:
    Component* self;

    ActionComponent(Component* self, UiComponentID id)
        : self(self)
        , id(id) {}

public:
    virtual ~ActionComponent() = default;

    /**
     *
     */
    bool set_action_listener(ActionListener* l);
};
