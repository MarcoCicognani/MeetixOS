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
#include <LibGUI/Listener/BoundsListener.hh>
#include <LibGUI/Protocol.hh>

/**
 * Component that is capable of receiving bounds events
 */
class BoundsEventComponent {
private:
    UiComponentID id;

protected:
    Component* self;

    BoundsEventComponent(Component* self, UiComponentID id)
        : self(self)
        , id(id) {}

public:
    virtual ~BoundsEventComponent() = default;

    /**
     *
     */
    bool set_bounds_listener(BoundsListener* l);
};
