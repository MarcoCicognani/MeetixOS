/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
* MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
*                                                                                     *
*         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
*         This software is derived from the Ghost operating system project,           *
*         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
*         https://ghostkernel.org/                                                    *
*         https://github.com/maxdev1/ghost                                            *
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

#ifndef __VIDEO_OUTPUT__
#define __VIDEO_OUTPUT__

#include <graphics/metrics/rectangle.hpp>
#include <graphics/color.hpp>
#include <graphics/metrics/dimension.hpp>

/**
 * The video output is the abstract interface that the window server uses to
 * initialize a video mode and put output on the screen.
 */
class VideoOutput_t
{
public:
	virtual ~VideoOutput_t()
	{
	}

	/**
	 * Initializes the video mode implementation.
	 *
	 * @return whether initialization was successful
	 */
	virtual bool initialize() = 0;

	/**
	 * Writes the invalid rectangle within the source image to the screen.
	 *
	 * @param invalid
	 * 		rectangle that is invalid
	 * @param sourceSize
	 * 		absolute size of the source
	 * @param source
	 * 		source buffer
	 */
	virtual void blit(const Rectangle &invalid, const Rectangle &sourceSize, Color_t *source) = 0;

	/**
	 * Returns the initialized resolution.
	 */
	virtual Dimension getResolution() = 0;

};

#endif
