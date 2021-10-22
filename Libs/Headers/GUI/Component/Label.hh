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
#include "ColoredComponent.hh"
#include "Component.hh"
#include "TitledComponent.hh"

/**
 *
 */
class Label : public Component,
              public TitledComponent,
              public ColoredComponent {
protected:
    /*
     *
     */
    Label(uint32_t id)
        : Component(id, UI_COMPONENT_TYPE_LABEL), TitledComponent(id), ColoredComponent(id) {
    }

public:
    ~Label() override = default;

    /*
     *
     */
    static Label* create();
};
