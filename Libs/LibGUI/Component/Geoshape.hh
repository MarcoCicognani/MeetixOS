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
#include <LibGUI/Application.hh>
#include <LibGUI/Component/ActionComponent.hh>
#include <LibGUI/Component/ColoredComponent.hh>
#include <LibGUI/Component/Component.hh>
#include <LibGUI/Component/ImageComponent.hh>
#include <LibGUI/Component/TitledComponent.hh>
#include <LibGUI/Properties.hh>
#include <LibGUI/Protocol.hh>

class Geoshape : public Component,
                 public TitledComponent,
                 public ImageComponent,
                 public ColoredComponent {
protected:
    /*
     *
     */
    Geoshape(uint32_t id)
        : Component(id, UI_COMPONENT_TYPE_GEOSHAPE)
        , TitledComponent(id)
        , ImageComponent(id)
        , ColoredComponent(id) {}

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
