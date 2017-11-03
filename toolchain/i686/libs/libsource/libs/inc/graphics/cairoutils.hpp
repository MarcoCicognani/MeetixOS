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

#ifndef MEETIX_LIBRARY_GRAPHICS_CAIROUTILS
#define MEETIX_LIBRARY_GRAPHICS_CAIROUTILS

#include <cairo/cairo.h>

/*
 * apply a radius blur to the provided surface
 */
void cairoBlurSurface(cairo_surface_t *surface, double radius);

/*
 * create a cairo rectangle with smussed angles
 */
void cairoRoundedRectangle(cairo_t *cr, double x, double y, double width, double height, double radius);

#endif
