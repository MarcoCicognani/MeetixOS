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

#ifndef __CONFIGURATION_BASED_VIDEO_OUTPUT__
#define __CONFIGURATION_BASED_VIDEO_OUTPUT__

#include "VideoOutput.hpp"

/**
 * A configuration based video output reads a configuration file first to
 * determine what video settings shall be used.
 */
class ConfigurationBasedVideoOutput_t: public VideoOutput_t
{
public:
	virtual ~ConfigurationBasedVideoOutput_t() {}

	/**
	 * Reads the configuration file and then calls the initialization function with
	 * the correct parameters.
	 *
	 * @return whether initialization was successful
	 */
	virtual bool initialize();

	/**
	 * Initializes the video mode with the given settings.
	 *
	 * @param width
	 * 		screen width
	 * @param height
	 * 		screen height
	 * @param bits
	 * 		bit depth
	 *
	 * @return whether initialization was successful
	 */
	virtual bool initializeWithSettings(uint32_t width, uint32_t height, uint32_t bits) = 0;

	/**
	 *
	 */
	virtual void blit(const Rectangle &invalid, const Rectangle &sourceSize, Color_t *source) = 0;

};

#endif
