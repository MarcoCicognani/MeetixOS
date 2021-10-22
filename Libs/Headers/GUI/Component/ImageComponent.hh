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
#include "../../Graphics/Color.hh"
#include "../Application.hh"
#include "../Protocol.hh"
#include "Component.hh"

/*
 *
 */
class ImageComponent {
private:
    /**
     *
     */
    uint32_t id;

protected:
    /**
     *
     */
    ImageComponent(uint32_t id) : id(id) {
    }

public:
    /**
     *
     */
    virtual bool set_image(std::string path, Graphics::Metrics::Point pngPosition);
};
