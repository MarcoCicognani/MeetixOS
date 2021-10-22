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
#include "../../../../Toolchain/Local/i686-pc-meetix/include/c++/11.2.0/list"
#include "ColoredComponent.hh"
#include "Component.hh"
#include "ImageComponent.hh"
#include "TitledComponent.hh"

/**
 *
 */
class Window : public Component,
               public TitledComponent,
               public ImageComponent,
               public ColoredComponent {
protected:
    /*
     *
     */
    Window(uint32_t id)
        : Component(id, UI_COMPONENT_TYPE_WINDOW), TitledComponent(id), ImageComponent(id),
          ColoredComponent(id) {
    }

public:
    ~Window() override = default;

    /*
     *
     */
    static Window* create();

    /*
     *
     */
    bool is_resizable();

    /*
     *
     */
    void set_resizable(bool resizable);

    /*
     *
     */
    bool on_close(void (*func)());
};
