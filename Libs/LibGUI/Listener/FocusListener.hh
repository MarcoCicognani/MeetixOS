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

class Component;

/**
 *
 */
class FocusListener : public Listener {
public:
    ~FocusListener() override = default;

    void process(UiComponentEventHeader* header) override {
        handle_focus_changed(reinterpret_cast<UiComponentFocusEvent*>(header)->nowFocused);
    }

    virtual void handle_focus_changed(bool is_this_focused) = 0;
};
