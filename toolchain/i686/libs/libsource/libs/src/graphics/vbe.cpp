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

#include <eva.h>
#include <graphics/vbe.hpp>

/**
 * set the video mode requested and fill the out mode info if operation success
 *
 * @param width:	the requested width
 * @param height:	the requested height
 * @param bpp:		the requested pixel color depth
 * @param out:		filled with the info mode initialized
 * @return true if operation success, false otherwise
 */
bool Vbe::setMode(uint16_t width, uint16_t height, uint8_t bpp, VbeModeInfo &out)
{
	// identify vbe driver
	Tid driverTid = TaskGetID(VBE_DRIVER_IDENTIFIER);
	if (driverTid == -1) return false;

	// gte the transaction id
	MessageTransaction transaction = GetMessageTxId();

	// create mode-setting request
	VbeSetModeRequest request(VBE_COMMAND_SET_MODE, width, height, bpp);
	SendMessageT(driverTid, &request, sizeof(VbeSetModeRequest), transaction);

	// receive response
	size_t buflen = sizeof(MessageHeader) + sizeof(VbeSetModeResponse);
	uint8_t buf[buflen];
	auto status = ReceiveMessageT(buf, buflen, transaction);
	if (status == MESSAGE_RECEIVE_STATUS_SUCCESSFUL)
	{
		VbeSetModeResponse *response = (VbeSetModeResponse*) MESSAGE_CONTENT(buf);
		if (response->status == VBE_SET_MODE_STATUS_SUCCESS)
		{
			out = response->modeInfo;
			return true;
		}
	}

	return false;
}
