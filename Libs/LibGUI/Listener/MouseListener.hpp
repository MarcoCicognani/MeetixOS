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
#include <LibGUI/Listener/Listener.hh>
#include <LibGUI/Protocol.hh>

/**
 *
 */
class MouseListener : public Listener {
public:
    ~MouseListener() override = default;

    void process(UiComponentEventHeader* header) override {
        handle_mouse_event(reinterpret_cast<UiComponentMouseEvent*>(header));
    }

    virtual void handle_mouse_event(UiComponentMouseEvent* e) = 0;
};
