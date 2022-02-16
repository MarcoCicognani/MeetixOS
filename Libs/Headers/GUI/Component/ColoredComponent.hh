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
#include "../../Api.h"
#include "../Application.hh"
#include "../Protocol.hh"
#include "Component.hh"

#include <LibGraphics/Color.hh>

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
