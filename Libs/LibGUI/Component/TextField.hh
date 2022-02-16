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

#include <cstdint>
#include <LibGUI/Component/ColoredComponent.hh>
#include <LibGUI/Component/Component.hh>
#include <LibGUI/Component/TitledComponent.hh>

/**
 *
 */
class Textfield : public Component,
                  public TitledComponent,
                  public ColoredComponent {
protected:
    /*
     *
     */
    Textfield(uint32_t id)
        : Component(id, UI_COMPONENT_TYPE_TEXTFIELD)
        , TitledComponent(id)
        , ColoredComponent(id) {}

public:
    ~Textfield() override = default;

    /*
     *
     */
    static Textfield* create();

    /*
     *
     */
    void setSecure(bool secure);

    /*
     *
     */
    bool isSecure();
};
