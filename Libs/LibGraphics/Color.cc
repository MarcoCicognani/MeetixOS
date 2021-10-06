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

#include <Graphics/Color.hh>

namespace Graphics::Color {

ArgbGradient as_rgb(u8 red, u8 green, u8 blue) {
    return as_argb(0xff, red, green, blue);
}

ArgbGradient as_argb(u8 alpha, u8 red, u8 green, u8 blue) {
    return static_cast<ArgbGradient>((alpha << 24) | (red << 16) | (green << 8) | blue);
}

double alpha(ArgbGradient argb_gradient) {
    return static_cast<double>((argb_gradient >> 24) & 0xFF) / 255;
}

double red(ArgbGradient argb_gradient) {
    return static_cast<double>((argb_gradient >> 16) & 0xFF) / 255;
}

double green(ArgbGradient argb_gradient) {
    return static_cast<double>((argb_gradient >> 8) & 0xFF) / 255;
}

double blue(ArgbGradient argb_gradient) {
    return static_cast<double>((argb_gradient >> 0) & 0xFF) / 255;
}

} /* namespace Graphics::Color */
