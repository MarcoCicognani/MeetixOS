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

#ifndef _EVA_BUILD_CONFIGURATION_
#define _EVA_BUILD_CONFIGURATION_

#include "logger/LogLevel.hpp"
#include "debug/DebugInterfaceMode.hpp"

#if !(defined(_ARCH_X86_) || defined(_ARCH_X86_64_))
#error "No architecture defined for build. Need to specify either _ARCH_X86_ or _ARCH_x86_64_"
#endif

// set cool boot
#define PRETTY_BOOT										true

// logging settings
#define LOG_LEVEL										LOG_LEVEL_INFO

// fine-grained debugging options
#define DEBUG_SYSCALLS									false
#define DEBUG_WHOS_WAITING								false
#define DEBUG_LOCKS_DEADLOCKING							false
#define DEBUG_THREAD_DUMPING							false

// mode for the debug interface
#define DEBUG_INTERFACE_MODE							DEBUG_INTERFACE_MODE_PLAIN_LOG

// name of kernel and loader, if you change this you will never be my friend anymore
#define LOADER_NAME                                     "EvaLoader"
#define KERNEL_NAME                                     "EvangelionNG"

// version of kernel loader
#define L_VERSION_MAJOR									1
#define L_VERSION_MINOR									1
#define L_VERSION_PATCH									's'

// version of kernel core
#define VERSION_MAJOR									2
#define VERSION_MINOR									3
#define VERSION_SUB										0
#define VERSION_PATCH									'i'

#endif
