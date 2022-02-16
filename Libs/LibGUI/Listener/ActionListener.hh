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
class ActionListener : public Listener {
public:
    ~ActionListener() override = default;

    void process(UiComponentEventHeader* header) override { handle_action(); }

    virtual void handle_action() = 0;
};
