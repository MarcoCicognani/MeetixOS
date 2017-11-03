/*********************************************************************************
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
* 																			     *
* This program is free software; you can redistribute it and/or                  *
* modify it under the terms of the GNU General Public License                    *
* as published by the Free Software Foundation; either version 2				 *
* of the License, or (char *argumentat your option) any later version.			 *
*																				 *
* This program is distributed in the hope that it will be useful,				 *
* but WITHout ANY WARRANTY; without even the implied warranty of                 *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#ifndef MEETIX_LIBRARY_GRAPHICS_COLORARGB
#define MEETIX_LIBRARY_GRAPHICS_COLORARGB

#include <stdint.h>

/**
 * alpha gradients
 */
#define ALPHA_OPAQUE   0
#define ALPHA_MASK     1
#define ALPHA_EMBEDDED 2
#define ALPHA_INDEXED  3

// create the color type
typedef uint32_t Color_t;

/**
 * create an argb gradient from alpha/red/green/blue values
 *
 * @param alpha:    the alpha value of the gradient
 * @param red:      the red value of the gradient
 * @param green:    the green value of the gradient
 * @param blue:     the blue value of the gradient
 * @return the color value
 */
#define ARGB(alpha, red, green, blue) \
    ((alpha << 24) | (red << 16) | (green << 8) | (blue))

/**
 * create an rgb gradient from red/green/blue values
 *
 * @param red:      the red value of the gradient
 * @param green:    the green value of the gradient
 * @param blue:     the blue value of the gradient
 * @return the color value
 */
#define RGB(red, green, blue) \
    ARGB(0xFF, red, green, blue)

/**
 * get value of alpha from an argb gradient
 *
 * @param argb:     the gradient
 * @return alpha value
 */
#define _lalpha(argb)  \
    ((argb >> 24) & 0xFF)

/**
 * get value of red from an argb gradient
 *
 * @param argb:     the gradient
 * @return red value
 */
#define _lred(argb)  \
    ((argb >> 16) & 0xFF)

/**
 * get value of green from an argb gradient
 *
 * @param argb:     the gradient
 * @return green value
 */
#define _lgreen(argb)  \
    ((argb >> 8) & 0xFF)

/**
 * get value of blue from an argb gradient
 *
 * @param argb:     the gradient
 * @return blue value
 */
#define _lblue(argb)  \
    ((argb >> 0) & 0xFF)

/**
 * get value of alpha from an argb gradient
 *
 * @param argb:     the gradient
 * @return alpha value
 */
#define _alpha(argb)  \
    ((double) ((argb >> 24) & 0xFF) / 255)

/**
 * get value of red from an argb gradient
 *
 * @param argb:     the gradient
 * @return red value
 */
#define _red(argb)  \
    ((double) ((argb >> 16) & 0xFF) / 255)

/**
 * get value of green from an argb gradient
 *
 * @param argb:     the gradient
 * @return green value
 */
#define _green(argb)  \
    ((double) ((argb >> 8) & 0xFF) / 255)

/**
 * get value of blue from an argb gradient
 *
 * @param argb:     the gradient
 * @return blue value
 */
#define _blue(argb)  \
    ((double) ((argb >> 0) & 0xFF) / 255)

/**
 * fill multiple parameters from argb gradient
 */
#define LONG_ARGB_TO_CAIRO_PARAMS(argb)  \
    _lred(argb), _lgreen(argb), _lblue(argb), _lalpha(argb)

/**
 * fill multiple parameters from argb gradient
 */
#define ARGB_TO_CAIRO_PARAMS(argb)  \
    _red(argb), _green(argb), _blue(argb), _alpha(argb)

#endif
