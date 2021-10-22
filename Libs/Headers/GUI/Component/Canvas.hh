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
#include "../../Graphics/Context.hh"
#include "../Listener/CanvasBufferListener.hh"
#include "Component.hh"

/**
 *
 */
struct CanvasBufferInfo {
    Graphics::Color::ArgbGradient* buffer;
    uint16_t                       width;
    uint16_t                       height;
};

/**
 *
 */
class Canvas : public Component {
protected:
    Graphics::Context* graphics;
    Address            currentBuffer;
    Address            nextBuffer;

    /**
     * Listener only for user purpose, so a client gets an event once the
     * buffer was changed.
     */
    CanvasBufferListener* userListener;

    /*
     *
     */
    Canvas(uint32_t id)
        : graphics(0), Component(id, UI_COMPONENT_TYPE_CANVAS), currentBuffer(0), nextBuffer(0),
          userListener(0) {
    }

public:
    virtual ~Canvas();

    /*
     *
     */
    static Canvas* create();

    /*
     *
     */
    void acknowledge_new_buffer(Address address);

    /*
     *
     */
    void blit(Graphics::Metrics::Rectangle rect);

    /*
     *
     */
    CanvasBufferInfo buffer_info();

    /*
     *
     */
    void set_buffer_listener(CanvasBufferListener* l) {
        userListener = l;
    }
};
