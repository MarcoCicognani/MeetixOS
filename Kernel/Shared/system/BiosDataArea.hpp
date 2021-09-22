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

#ifndef EVA_SHARED_SYSTEM_BIOSDATAAREA
#define EVA_SHARED_SYSTEM_BIOSDATAAREA

#include "Api/StdInt.h"

/**
 * COM port information within the BIOS data area
 */
struct ComPortInformation {
    uint16_t com1;
    uint16_t com2;
    uint16_t com3;
    uint16_t com4;
} A_PACKED;

/**
 * LPT information
 */
struct LptInformation {
    uint16_t lpt1;
    uint16_t lpt2;
    uint16_t lpt3;
} A_PACKED;

/**
 * Bios data area structure
 */
struct BiosDataArea {
    ComPortInformation comPortInfo;
    LptInformation     lptInfo;

    uint16_t ebdaShiftedAddr; // must be left-shifted by 4

    // Other data is currently not used
} A_PACKED;

/**
 * Pointer to the bios data area
 */
extern BiosDataArea* biosDataArea;

#endif
