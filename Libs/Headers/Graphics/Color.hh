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

/**
 * @brief Shortcut to give to cairo argb params
 */
#define ARGB_TO_CAIRO_PARAMS(argb)                                                                 \
    Graphics::Color::red(argb), Graphics::Color::green(argb), Graphics::Color::blue(argb),         \
        Graphics::Color::alpha(argb)

namespace Graphics::Color {

/* ------------------------------------ C++ Type Definitions ------------------------------------ */

/**
 * @brief The Graphics::Color::as_argb color gradient
 */
using ArgbGradient = usize;

/**
 * @brief Alpha channel gradients
 */
enum AlphaGradient
{
    ALPHA_OPAQUE,
    ALPHA_MASK,
    ALPHA_EMBEDDED,
    ALPHA_INDEXED,
};

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Creates an ArgbGradient from the given values
 */
ArgbGradient as_rgb(u8 red, u8 green, u8 blue);
ArgbGradient as_argb(u8 alpha, u8 red, u8 green, u8 blue);

/**
 * @brief Extracts the sub-color gradients from the given ArgbGradient
 */
double alpha(ArgbGradient argb_gradient);
double red(ArgbGradient argb_gradient);
double green(ArgbGradient argb_gradient);
double blue(ArgbGradient argb_gradient);

} /* namespace Graphics::Color */