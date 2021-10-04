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

#ifndef _TERMINAL_SCREEN_STREAM_STATUS_
#define _TERMINAL_SCREEN_STREAM_STATUS_

#include <IO/Shell.hh>

/**
 * Determines the state of a stream to the terminal.
 */
typedef int                TerminalStreamStatus;
const TerminalStreamStatus TERMINAL_STREAM_STATUS_TEXT         = 0;
const TerminalStreamStatus TERMINAL_STREAM_STATUS_LAST_WAS_ESC = 1;
const TerminalStreamStatus TERMINAL_STREAM_STATUS_WITHIN_VT100 = 2;
const TerminalStreamStatus TERMINAL_STREAM_STATUS_WITHIN_TERM  = 3;

/**
 * Used to remember the status of a stream, for example whether a control
 * sequence is currently being sent.
 */
typedef struct {
    TerminalStreamStatus status         = TERMINAL_STREAM_STATUS_TEXT;
    int                  parameterCount = 0;
    int                  parameters[SHELL_STREAM_CONTROL_MAX_PARAMETERS];
    char                 controlCharacter;
} StreamControlStatus;

#endif
