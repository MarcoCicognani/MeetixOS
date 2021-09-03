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

#ifndef MEETIX_LIBRARY_GRAPHICS_VBE
#define MEETIX_LIBRARY_GRAPHICS_VBE

#include <stdint.h>

#define VBE_DRIVER_IDENTIFIER			"vbe"

/**
 * struct that contains the info of the vbe mode initialized
 */
struct VbeModeInfo
{
public:
	uint16_t resX;       // x width
	uint16_t resY;       // y heigth
	uint16_t bpp;        // density per inch
	uint16_t bpsl;       // bytes per scanline
	uint32_t lfb;        // linear frame buffer pointer

	/**
	 * empty constructor
	 */
	VbeModeInfo() {}

	/**
	 * filled constructor
	 */
	VbeModeInfo(uint16_t w, uint16_t h, uint16_t bp, uint16_t bps, uint32_t fb) :
		resX(w), resY(h), bpp(bp), bpsl(bps), lfb(fb) {}
}__attribute__((packed));

/**
 * vbe command type
 */
typedef int VbeCommand;
#define VBE_COMMAND_SET_MODE	((VbeCommand) 0)

/**
 * vbe request command packet
 */
struct VbeRequestHeader
{
public:
	VbeCommand command;  // command for driver

	/**
	 * empty constructor
	 */
	VbeRequestHeader() {}

	/**
	 * filled constructor
	 */
	VbeRequestHeader(VbeCommand cmd) : command(cmd) {}
}__attribute__((packed));

/**
 * vbe request status type
 */
typedef int VbeSetModeStatus;
#define VBE_SET_MODE_STATUS_SUCCESS		((VbeSetModeStatus) 0)
#define VBE_SET_MODE_STATUS_FAILED		((VbeSetModeStatus) 1)

/**
 * vbe set mode request
 */
struct VbeSetModeRequest
{
public:
	VbeRequestHeader header;	// request header with the command for driver
	uint16_t width;				// the requested screen width
	uint16_t height;			// the requested height
	uint8_t bpp;				// bpp

	/**
	 * empty constructor
	 */
	VbeSetModeRequest() {}

	/**
	 * filled constructor
	 */
	VbeSetModeRequest(VbeCommand cmd, uint16_t w, uint16_t h, uint8_t bp) :
		header(cmd), width(w), height(h), bpp(bp) {}
}__attribute__((packed));

/**
 * response of the driver
 */
struct VbeSetModeResponse
{
	VbeSetModeStatus status;  // status code
	VbeModeInfo modeInfo;     // if successed, filled with the mode info setupped
}__attribute__((packed));

/**
 * static class to manage the default video driver
 */
class Vbe
{
public:
	/**
	 * set the video mode requested and fill the out mode info if operation success
	 *
	 * @param width:	the requested width
	 * @param height:	the requested height
	 * @param bpp:		the requested pixel color depth
	 * @param out:		filled with the info mode initialized
	 * @return true if operation success, false otherwise
	 */
	static bool setMode(uint16_t width, uint16_t height, uint8_t bpp, VbeModeInfo &out);
};

#endif
