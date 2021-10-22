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
#include "../Listener/BoundsListener.hh"
#include "../Protocol.hh"

/**
 * Component that is capable of receiving bounds events
 */
class BoundsEventComponent {
private:
    UiComponentID id;

protected:
    Component* self;

    BoundsEventComponent(Component* self, UiComponentID id) : self(self), id(id) {
    }

public:
    virtual ~BoundsEventComponent() = default;

    /**
     *
     */
    bool set_bounds_listener(BoundsListener* l);
};
