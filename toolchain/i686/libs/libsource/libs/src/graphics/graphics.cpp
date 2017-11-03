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

#include <graphics/graphics.hpp>
#include <string.h>
#include <malloc.h>

/**
 *
 */
Graphics::Graphics(uint16_t width, uint16_t height) : width(width), height(height)
{
	resize(width, height);
}

/**
 *
 */
void Graphics::resize(int newWidth, int newHeight)
{
	if (newWidth < 0 || newHeight < 0) return;

	// release old buffers
	if (surface) cairo_surface_destroy(surface);
	if (context) cairo_destroy(context);

	// set new values
	width = newWidth;
	height = newHeight;
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	context = cairo_create(surface);
}

/**
 *
 */
void Graphics::blitTo(Graphics *graphics, Rectangle absoluteClip, Point position)
{
	auto cr = graphics->context;
	cairo_save(cr);
	cairo_set_source_surface(cr, this->surface, position.x, position.y);
	cairo_rectangle(cr, absoluteClip.x, absoluteClip.y, absoluteClip.width, absoluteClip.height);
	cairo_clip(cr);
	cairo_paint(cr);
	cairo_restore(cr);
}
