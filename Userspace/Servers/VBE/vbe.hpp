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

#ifndef VBE_DRIVER
#define VBE_DRIVER

#include <Api.h>
#include <cstdint>

/**
 *
 */
#define VBE_INFO_BLOCK_SIZE 512
struct VbeInfoBlock {
    // Signature should be "VESA" (0x56455341)
    uint8_t signature[4];
    // For example 0x300 for VESA 3.0
    uint16_t version;

    // OEM string pointer
    FarPtr oemStringFarPtr;
    // Capability information
    uint8_t capabilities[4];
    // Video modes
    FarPtr videoModeFarPtr;

    // Size of video memory in 64KiB blocks
    uint16_t memoryBlockCount;

    // OEM information
    uint16_t oemSoftwareRevision;
    FarPtr   oemVendorNameStringFarPtr;
    FarPtr   oemProductNameStringFarPtr;
    FarPtr   oemProductRevisionFarPtr;
} A_PACKED;

/**
 *
 */
#define VBE_MODE_INFO_BLOCK_SIZE 256
struct ModeInfoBlock {
    // 0
    uint16_t modeAttributes;
    uint8_t  windowAttributesA;
    uint8_t  windowAttributesB;
    uint16_t granularityKb;
    uint16_t windowSizeKb;
    uint16_t segmentA;
    uint16_t segmentB;
    FarPtr   windowFunctionFarPtr;

    // 16
    uint16_t bytesPerScanline;
    uint16_t resolutionX;
    uint16_t resolutionY;
    uint8_t  charSizeX;
    uint8_t  charSizeY;
    uint8_t  planes;
    uint8_t  bpp;
    uint8_t  banks;
    uint8_t  memoryModel;
    uint8_t  bankSizeKb;
    uint8_t  imagePages;
    uint8_t  reserved0;

    // 31
    uint8_t redMaskSize;
    uint8_t redFieldPosition;
    uint8_t greenMaskSize;
    uint8_t greenFieldPosition;
    uint8_t blueMaskSize;
    uint8_t blueFieldPosition;
    uint8_t rsvdMaskSize;
    uint8_t rsvdFieldPosition;
    uint8_t directColorModeInfo;

    // 40
    uint32_t lfbPhysicalBase;
    uint32_t offScreenMemOffset;
    uint16_t offScreenMemSizeKb;
    uint16_t linBytesPerScanline;
    uint8_t  bnkNumberOfImagePages;
    uint8_t  linNumberOfImagePages;
    uint8_t  linRedMaskSize;
    uint8_t  linRedFieldPosition;
    uint8_t  linGreenMaskSize;
    uint8_t  linGreenFieldPosition;
    uint8_t  linBlueMaskSize;
    uint8_t  linkBlueFieldPosition;
    uint8_t  linReservedMaskSize;
    uint8_t  linReservedFieldPosition;
    uint32_t maxPixelClock;
} A_PACKED;

/**
 * Info about the enabled video mode
 */
struct VesaVideoInfo {
    uint32_t resolutionX;
    uint32_t resolutionY;
    uint8_t  bpp;
    uint32_t bytesPerScanline;
    void*    lfb;
};

#endif
