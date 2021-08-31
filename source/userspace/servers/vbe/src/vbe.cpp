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

#include <vbe.hpp>
#include <graphics/vbe.hpp>
#include <utils/utils.hpp>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sstream>

/**
 *
 */
bool loadVbeInfo(VbeInfoBlock *target)
{
	Vm86Registers out;
	Vm86Registers in;

	FarPointer vbeInfoBlockFp = LINEAR_TO_FP((uint32_t ) target);

	in.ax = 0x4F00;
	in.bx = 0;
	in.cx = 0;
	in.dx = 0;
	in.es = FP_SEG(vbeInfoBlockFp);
	in.di = FP_OFF(vbeInfoBlockFp);
	in.ds = 0;
	in.si = 0;

	CallVm86(0x10, &in, &out);

	return (out.ax == 0x4F);
}

/**
 *
 */
bool loadModeInfo(uint16_t mode, ModeInfoBlock *target)
{
	Vm86Registers out;
	Vm86Registers regs;

	FarPointer modeInfoBlockFp = LINEAR_TO_FP((uint32_t ) target);

	regs.ax = 0x4F01;
	regs.cx = mode;
	regs.es = FP_SEG(modeInfoBlockFp);
	regs.di = FP_OFF(modeInfoBlockFp);

	CallVm86(0x10, &regs, &out);

	return (out.ax == 0x4F);
}

/**
 *
 */
bool setVideoMode(uint32_t mode, bool flatFrameBuffer)
{
	Vm86Registers out;
	Vm86Registers regs;

	regs.ax = 0x4F02;
	regs.bx = mode;

	if (flatFrameBuffer) regs.bx |= 0x4000;

	CallVm86(0x10, &regs, &out);

	return (out.ax == 0x4F);
}

/**
 *
 */
bool setVideoMode(uint32_t wantedWidth, uint32_t wantedHeight, uint32_t wantedBpp, VesaVideoInfo &result)
{
	bool successful = false;
	bool debugOutput = false;

	// Get VBE mode info
	VbeInfoBlock *vbeInfoBlock = (VbeInfoBlock*) LowerMalloc(VBE_INFO_BLOCK_SIZE);

	if (!loadVbeInfo(vbeInfoBlock)) Utils::log("could not load VBE info block");
	else
	{
		Utils::log("loaded vbe info, version %i", vbeInfoBlock->version);

		// Load modes
		ModeInfoBlock *modeInfoBlock = (ModeInfoBlock*) LowerMalloc(VBE_MODE_INFO_BLOCK_SIZE);

		uint32_t bestMatchingMode = 0;
		uint32_t bestFoundDepthDiff = -1;
		uint32_t bestFoundResolutionDiff = -1;
		uint32_t wantedResolution = wantedWidth * wantedHeight;

		Utils::log("farptr: %d, seg: %d, off: %d, linear: %d", vbeInfoBlock->videoModeFarPtr, FP_SEG(vbeInfoBlock->videoModeFarPtr), FP_OFF(vbeInfoBlock->videoModeFarPtr), FP_TO_LINEAR(vbeInfoBlock->videoModeFarPtr));

		uint16_t *modes = (uint16_t*) FP_TO_LINEAR(vbeInfoBlock->videoModeFarPtr);
		for (uint32_t i = 0;; ++i)
		{
			uint16_t mode = modes[i];

			if (mode == 0xFFFF) break;

			if (!loadModeInfo(mode, modeInfoBlock))
			{
				Utils::log("mode %i: could not load mode info block, skipping", mode);
				continue;
			}

			// mode output
			if (debugOutput) {
				Utils::log("mode: %i, attr: %i, mmo: %i, %ix%ix%i lfb: %d", mode, modeInfoBlock->modeAttributes, modeInfoBlock->memoryModel, modeInfoBlock->resolutionX, modeInfoBlock->resolutionY, modeInfoBlock->bpp, modeInfoBlock->lfbPhysicalBase);
			}

			// Must be supported by hardware
			if ((modeInfoBlock->modeAttributes & 0x1) != 0x1) continue;

			// Need LFB support
			if ((modeInfoBlock->modeAttributes & 0x90) != 0x90) continue;

			// Need direct color mode
			if (modeInfoBlock->memoryModel != 6) continue;

			// Check if it's matching better
			uint32_t resolution = modeInfoBlock->resolutionX * modeInfoBlock->resolutionY;
			uint32_t resolutionDiff = (resolution > wantedResolution) ? (resolution - wantedResolution) : (wantedResolution - resolution);
			uint32_t depthDiff = (modeInfoBlock->bpp > wantedBpp) ? (modeInfoBlock->bpp - wantedBpp) : (wantedBpp - modeInfoBlock->bpp);

			if (resolutionDiff < bestFoundResolutionDiff || (resolutionDiff == bestFoundResolutionDiff && depthDiff < bestFoundDepthDiff))
			{
				bestMatchingMode = mode;
				bestFoundDepthDiff = depthDiff;
				bestFoundResolutionDiff = resolutionDiff;

				Utils::log("vbe: updated best matching mode to %ix%ix%i", modeInfoBlock->resolutionX, modeInfoBlock->resolutionY, modeInfoBlock->bpp);

				// Break on perfect match
				if (depthDiff == 0 && resolutionDiff == 0) break;
			}
		}

		// If a matching mode was found
		if (bestMatchingMode != 0)
		{
			// Enable the best matching mode
			Utils::log("performing mode switch to %i", bestMatchingMode);
			if (setVideoMode(bestMatchingMode, true))
			{
				// Reload mode info into buffer
				bool couldReloadModeInfo = loadModeInfo(bestMatchingMode, modeInfoBlock);

				// Reloading successful?
				if (couldReloadModeInfo)
				{
					// Create MMIO mapping
					void *area = MapMmio((void*) modeInfoBlock->lfbPhysicalBase, modeInfoBlock->linBytesPerScanline * modeInfoBlock->resolutionY);

					// Write out
					result.resolutionX = modeInfoBlock->resolutionX;
					result.resolutionY = modeInfoBlock->resolutionY;
					result.bpp = modeInfoBlock->bpp;
					result.bytesPerScanline = modeInfoBlock->linBytesPerScanline;
					result.lfb = area;
					successful = true;
				}
			}
		}

		LowerFree(modeInfoBlock);
	}

	LowerFree(vbeInfoBlock);

	return successful;
}

/**
 *
 */
int main()
{
	if (!RegisterAsServer(VBE_DRIVER_IDENTIFIER, SECURITY_LEVEL_DRIVER))
	{
		klog("video driver: could not register with task identifier: %s", VBE_DRIVER_IDENTIFIER);
		return -1;
	}

	Utils::log("vesa initialized");

	size_t buflen = sizeof(MessageHeader) + sizeof(VbeSetModeRequest);
	uint8_t buf[buflen];

	while (true)
	{
		// wait for incoming request
		auto status = ReceiveMessage(buf, buflen);
		if (status != MESSAGE_RECEIVE_STATUS_SUCCESSFUL) continue;

		MessageHeader *header = (MessageHeader*) buf;
		VbeRequestHeader *vbeheader = (VbeRequestHeader*) MESSAGE_CONTENT(buf);
		uint32_t requester = header->sender;

		// handle command
		if (vbeheader->command == VBE_COMMAND_SET_MODE)
		{
			VbeSetModeRequest *request = (VbeSetModeRequest*) MESSAGE_CONTENT(buf);

			// create response
			VbeSetModeResponse response;

			// switch video mode
			VesaVideoInfo result;
			uint16_t resX = request->width;
			uint16_t resY = request->height;
			uint8_t bpp = request->bpp;

			Utils::log("attempting to set video mode");
			if (setVideoMode(resX, resY, bpp, result))
			{
				Utils::log("changed video mode to " + resX + 'x' + resY + 'x' + bpp);
				uint32_t lfbSize = result.bytesPerScanline * result.resolutionY;
				void *addressInRequestersSpace = ShareMem(result.lfb, lfbSize, requester);

				response.status = VBE_SET_MODE_STATUS_SUCCESS;
				response.modeInfo.lfb = (uint32_t) addressInRequestersSpace;
				response.modeInfo.resX = result.resolutionX;
				response.modeInfo.resY = result.resolutionY;
				response.modeInfo.bpp = (uint8_t) result.bpp;
				response.modeInfo.bpsl = (uint16_t) result.bytesPerScanline;

			}

			else
			{
				Utils::log("unable to switch to video resolution " + resX + 'x' + resY + 'x' + bpp);
				response.status = VBE_SET_MODE_STATUS_FAILED;
			}

			// send response
			SendMessageT(header->sender, &response, sizeof(VbeSetModeResponse), header->transaction);
		}

		else
		{
			std::stringstream ukn;
			ukn << "received unknown command " << vbeheader->command << " from task " << header->sender;
			Utils::log(ukn.str());
		}
	}

	return 0;
}
