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
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef MEETIX_LIBRARY_GRAPHICS_GRAPHICS
#define MEETIX_LIBRARY_GRAPHICS_GRAPHICS

#include <cairo/cairo.h>
#include <graphics/metrics/dimension.hpp>
#include <graphics/metrics/point.hpp>
#include <graphics/metrics/rectangle.hpp>
#include <stdint.h>
#include <string>

/**
 *
 */
class Graphics {
private:
    int              width;
    int              height;
    cairo_t*         context = 0;
    cairo_surface_t* surface = 0;

public:
    /**
     * Creates a graphics object. This is a class that holds a surface.
     * If an <code>externalBuffer</code> is provided, no internal buffer will be
     * automatically created.
     *
     * @param width of the externalBuffer
     * @param height of the externalBuffer
     */
    Graphics(uint16_t width = 0, uint16_t height = 0);

    /**
     *
     */
    void resize(int width, int height);

    /**
     *
     */
    cairo_t* getContext() {
        return context;
    }

    /**
     *
     */
    cairo_surface_t* getSurface() {
        return surface;
    }

    /**
     *
     */
    int getWidth() {
        return width;
    }

    /**
     *
     */
    int getHeight() {
        return height;
    }

    /**
     *
     */
    void blitTo(Graphics* graphics, Rectangle absoluteClip, Point position);
};

#endif
