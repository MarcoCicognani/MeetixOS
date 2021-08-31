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
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * */

#ifndef __EVA_SYS_TYPES__
#define __EVA_SYS_TYPES__

#include "eva/common.h"
#include "eva/stdint.h"

__BEGIN_C

// address types
typedef uint32_t Address;
typedef Address PhysicalAddress;
typedef Address VirtualAddress;
typedef uint32_t FarPointer;

// extracts parts from far pointers
#define FP_SEG(fp)        				(((FarPointer) fp) >> 16)
#define FP_OFF(fp)        				(((FarPointer) fp) & 0xFFFF)

// far pointer conversion macros
#define SEGOFF_TO_LINEAR(seg, off)		((void*) (((seg & 0xFFFF) << 4) + (off & 0xFFFF)))
#define FP_TO_LINEAR(fp)				SEGOFF_TO_LINEAR(FP_SEG(fp), FP_OFF(fp))

#define SEGOFF_TO_FP(seg, off)			((FarPointer) (((seg & 0xFFFF) << 16) | (off & 0xFFFF)))
#define LINEAR_TO_FP(linear)			((linear > 0x100000) ? 0 : ((((linear >> 4) & 0xFFFF) << 16) + (linear & 0xFL)))

__END_C

#endif
