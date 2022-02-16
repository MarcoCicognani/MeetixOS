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

#include <cstdint>
#include <LibGUI/Protocol.hh>

/**
 *
 */
class Listener {
public:
    virtual ~Listener() = default;

    /**
     * Processes the incoming raw event and translates it to a g_event.
     */
    virtual void process(UiComponentEventHeader* header) = 0;
};
