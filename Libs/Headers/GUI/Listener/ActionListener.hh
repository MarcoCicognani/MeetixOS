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
#include "Listener.hh"

class Component;

/**
 *
 */
class ActionListener : public Listener {
public:
    ~ActionListener() override = default;

    void process(UiComponentEventHeader* header) override {
        handle_action();
    }

    virtual void handle_action() = 0;
};
