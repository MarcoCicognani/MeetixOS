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
#include <LibGraphics/Color.hh>
#include <LibGUI/Application.hh>
#include <LibGUI/Component/Component.hh>
#include <LibGUI/Protocol.hh>

/*
 *
 */
class ColoredComponent {
private:
    uint32_t id;

protected:
    ColoredComponent(uint32_t id)
        : id(id) {}

public:
    virtual bool set_color(Graphics::Color::ArgbGradient shapeColor, Graphics::Color::ArgbGradient titleColor);
};
