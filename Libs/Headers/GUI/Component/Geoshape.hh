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
#include "../Application.hh"
#include "../Properties.hh"
#include "../Protocol.hh"
#include "ActionComponent.hh"
#include "ColoredComponent.hh"
#include "Component.hh"
#include "ImageComponent.hh"
#include "TitledComponent.hh"

class Geoshape : public Component,
                 public TitledComponent,
                 public ImageComponent,
                 public ColoredComponent {
protected:
    /*
     *
     */
    Geoshape(uint32_t id)
        : Component(id, UI_COMPONENT_TYPE_GEOSHAPE), TitledComponent(id), ImageComponent(id),
          ColoredComponent(id) {
    }

public:
    ~Geoshape() override = default;

    /**
     *
     */
    static Geoshape* create();

    /**
     *
     */
    void set_resizable(bool resizable);
};
