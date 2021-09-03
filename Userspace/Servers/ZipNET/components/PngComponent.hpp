/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
*                                                                                     *
* This program is free software; you can redistribute it and/or                       *
* modify it under the terms of the GNU General Public License                         *
* as published by the Free Software Foundation; either version 2                      *
* of the License, or (char *argumentat your option) any later version.                *
*                                                                                     *
* This program is distributed in the hope that it will be useful,                     *
* but WITHout ANY WARRANTY; without even the implied warranty of                      *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
* GNU General Public License for more details.                                        *
*                                                                                     *
* You should have received a copy of the GNU General Public License                   *
* along with this program; if not, write to the Free Software                         *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#ifndef _PNG_COMPONENT_
#define _PNG_COMPONENT_

#include <graphics/graphics.hpp>
#include <zipNET.hpp>
#include <string>


class PngComponent_t
{
protected:
	// location of png on component
	Point pngPosition;

	// there is a png to render
	bool pathToLoad;

	// cairo pointers
	cairo_t *pngSurface;
	cairo_surface_t *png;

public:
	/**
	 *	destructor
	 */
	virtual ~PngComponent_t() {}

	/**
	 *	interface method to set png image from string and position
	 */
	virtual void setPNG(std::string path, Point position) = 0;

	/**
	 *	set png animation
	 */
	virtual void PngAnimation(std::string path, Point PNGstartAnimation, Point PNGendAnimation, size_t sleep) = 0;
};

#endif
