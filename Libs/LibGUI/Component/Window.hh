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
#include <LibGUI/Component/ImageComponent.hh>
#include <LibGUI/Component/TitledComponent.hh>
#include <list>

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
        : Component(id, UI_COMPONENT_TYPE_WINDOW)
        , TitledComponent(id)
        , ImageComponent(id)
        , ColoredComponent(id) {}

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
