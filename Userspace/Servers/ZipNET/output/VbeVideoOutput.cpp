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

#include "VbeVideoOutput.hpp"

/**
 *
 */
bool VbeVideoOutput_t::initializeWithSettings(uint32_t width, uint32_t height, uint32_t bits) {
    return Vbe::setMode(width, height, bits, videoModeInformation);
}

/**
 *
 */
void VbeVideoOutput_t::blit(const Rectangle& invalid,
                            const Rectangle& sourceSize,
                            Color_t*         source) {
    uint16_t bpp = videoModeInformation.bpp;
    uint8_t* position
        = ((uint8_t*)videoModeInformation.lfb) + (invalid.y * videoModeInformation.bpsl);

    uint32_t right  = invalid.x + invalid.width;
    uint32_t bottom = invalid.y + invalid.height;

    if ( bpp == 32 ) {
        for ( int y = invalid.y; y < bottom; y++ ) {
            uint32_t* position4 = (uint32_t*)position;
            for ( int x = invalid.x; x < right; x++ )
                position4[x] = source[y * sourceSize.width + x];

            position = position + videoModeInformation.bpsl;
        }

    }

    else if ( bpp == 24 ) {
        for ( int y = invalid.y; y < bottom; y++ ) {
            for ( int x = invalid.x; x < right; x++ ) {
                Color_t color       = source[y * sourceSize.width + x];
                position[x * 3]     = color & 0xFF;
                position[x * 3 + 1] = (color >> 8) & 0xFF;
                position[x * 3 + 2] = (color >> 16) & 0xFF;
            }

            position = position + videoModeInformation.bpsl;
        }
    }
}

/**
 *
 */
Dimension VbeVideoOutput_t::getResolution() {
    return Dimension(videoModeInformation.resX, videoModeInformation.resY);
}
