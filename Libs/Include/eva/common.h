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

#ifndef __EVA_COMMON__
#define __EVA_COMMON__

/**
 * C++ compilers must know that the declarations are C declarations
 */
#ifdef __cplusplus
#    define __BEGIN_C extern "C" {
#    define __END_C   }
#else
#    define __BEGIN_C
#    define __END_C
#endif

// standard declarations
#define __HAS_STDC11 (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)

// not implemented warning
#define __NOT_IMPLEMENTED(name)                                                                    \
    Log("'" #name "' is not implemented");                                                         \
    Exit(0);

// debug tracing
#define __DEBUG_TRACE_ENABLED 0
#if __DEBUG_TRACE_ENABLED
#    define __DEBUG_TRACE(function) Log(#    function);
#else
#    define __DEBUG_TRACE(function)
#endif

#endif
