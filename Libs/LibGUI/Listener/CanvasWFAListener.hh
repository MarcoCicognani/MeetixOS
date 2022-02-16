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
class CanvasWfaListener : public Listener {
public:
    Canvas* m_canvas;

    CanvasWfaListener(Canvas* canvas)
        : m_canvas{ canvas } {}

    ~CanvasWfaListener() override = default;

    void process(UiComponentEventHeader* header) override {
        if ( header->type == UI_COMPONENT_EVENT_TYPE_CANVAS_WFA ) {
            m_canvas->acknowledge_new_buffer(reinterpret_cast<UiComponentCanvasWfaEvent*>(header)->newBufferAddress);
        }
    }
};
