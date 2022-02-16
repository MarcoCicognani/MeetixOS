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

#include <cairo/cairo.h>

namespace Graphics::Cairo {

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Blurs the given surface for the amount of radius given
 */
void blur_surface(cairo_surface_t* surface, double radius);

/**
 * @brief Draws on the given cairo context a rectangle with rounded angles
 */
void rounded_rectangle(cairo_t* cr, double x, double y, double width, double height, double radius);

} /* namespace Graphics::Cairo */