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

#include <Api.h>
#include <cstdint>
#include <LibGUI/Protocol.hh>

/**
 *
 */
class TitledComponent {
private:
    uint32_t id;

protected:
    TitledComponent(uint32_t id)
        : id(id) {}

public:
    /**
     *
     */
    virtual ~TitledComponent() = default;

    /**
     *
     */
    virtual bool set_title(std::string title);

    /**
     *
     */
    virtual bool set_ghost_title(std::string text);

    /**
     *
     */
    virtual bool set_font(std::string fontName);

    /**
     *
     */
    virtual bool set_font_size(int size);

    /**
     *
     */
    virtual bool set_title_alignment(Graphics::Text::Alignment alignment);

    /**
     *
     */
    virtual std::string title();
};
