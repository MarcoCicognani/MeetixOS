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

#include "../../../../Toolchain/Local/i686-pc-meetix/include/c++/11.2.0/cstdint"
#include "../../Api.h"
#include "../Listener/ActionListener.hh"
#include "../Protocol.hh"

/**
 * Component that is capable of receiving action events
 */
class ActionComponent {
private:
    UiComponentID id;

protected:
    Component* self;

    ActionComponent(Component* self, UiComponentID id) : self(self), id(id) {
    }

public:
    virtual ~ActionComponent() = default;

    /**
     *
     */
    bool set_action_listener(ActionListener* l);
};
