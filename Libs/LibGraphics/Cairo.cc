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

#include <cmath>
#include <cstdint>
#include <LibGraphics/Cairo.hh>

namespace Graphics::Cairo {

void blur_surface(cairo_surface_t* surface, double radius) {
    // declare a temporary variable
    cairo_surface_t* tmp;
    int              width, height;
    int              srcStride, dstStride;
    int              x, y, z, w;
    u8 *             src, *dst;
    usize *          s, *d, a, p;
    int              i, j, k;
    u8               kernel[17];
    const int        size = 17;
    const int        half = size / 2;

    // get status
    if ( cairo_surface_status(surface) )
        return;

    // get sizes
    width  = cairo_image_surface_get_width(surface);
    height = cairo_image_surface_get_height(surface);

    // get format
    switch ( cairo_image_surface_get_format(surface) ) {
        case CAIRO_FORMAT_A1:

        default:
            // Don't even think about it!
            return;

        case CAIRO_FORMAT_A8:
            // Handle a8 surfaces by effectively unrolling the loops by a
            // factor of 4 - this is safe since we know that stride has to be a
            // multiple of usize.
            width = width / 4;
            break;

        case CAIRO_FORMAT_RGB24:
        case CAIRO_FORMAT_ARGB32:
            break;
    }

    // creating a temporary surface and get it status
    tmp = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    if ( cairo_surface_status(tmp) )
        return;

    // get data on two copies
    src       = cairo_image_surface_get_data(surface);
    srcStride = cairo_image_surface_get_stride(surface);

    // get data on two copies
    dst       = cairo_image_surface_get_data(tmp);
    dstStride = cairo_image_surface_get_stride(tmp);

    a = 0;
    for ( i = 0; i < size; i++ ) {
        double f       = i - half;
        a += kernel[i] = exp(-f * f / 30.0) * 80;
    }

    // Horizontally blur from surface -> tmp
    for ( i = 0; i < height; i++ ) {
        s = (usize*)(src + i * srcStride);
        d = (usize*)(dst + i * dstStride);
        for ( j = 0; j < width; j++ ) {
            if ( radius < j && j < width - radius ) {
                d[j] = s[j];
                continue;
            }

            x = y = z = w = 0;
            for ( k = 0; k < size; k++ ) {
                if ( j - half + k < 0 || j - half + k >= width )
                    continue;

                p = s[j - half + k];

                x += ((p >> 24) & 0xff) * kernel[k];
                y += ((p >> 16) & 0xff) * kernel[k];
                z += ((p >> 8) & 0xff) * kernel[k];
                w += ((p >> 0) & 0xff) * kernel[k];
            }
            d[j] = (x / a << 24) | (y / a << 16) | (z / a << 8) | w / a;
        }
    }

    // Then vertically blur from tmp -> surface
    for ( i = 0; i < height; i++ ) {
        s = (usize*)(dst + i * dstStride);
        d = (usize*)(src + i * srcStride);
        for ( j = 0; j < width; j++ ) {
            if ( radius <= i && i < height - radius ) {
                d[j] = s[j];
                continue;
            }

            x = y = z = w = 0;
            for ( k = 0; k < size; k++ ) {
                if ( i - half + k < 0 || i - half + k >= height )
                    continue;

                s = (usize*)(dst + (i - half + k) * dstStride);
                p = s[j];

                x += ((p >> 24) & 0xff) * kernel[k];
                y += ((p >> 16) & 0xff) * kernel[k];
                z += ((p >> 8) & 0xff) * kernel[k];
                w += ((p >> 0) & 0xff) * kernel[k];
            }
            d[j] = (x / a << 24) | (y / a << 16) | (z / a << 8) | w / a;
        }
    }

    cairo_surface_destroy(tmp);
}

/*
 * create a cairo rectangle with smussed angles
 */
void rounded_rectangle(cairo_t* cr, double x, double y, double width, double height, double radius) {
    double degrees = M_PI / 180.0;
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
    cairo_arc(cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
    cairo_arc(cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
    cairo_arc(cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
    cairo_close_path(cr);
}

} /* namespace Graphics::Cairo */