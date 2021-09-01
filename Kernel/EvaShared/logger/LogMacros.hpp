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

#ifndef EVA_SHARED_LOGGER_LOGMACROS
#define EVA_SHARED_LOGGER_LOGMACROS

#include <BuildConfig.hpp>

/**
 * define a log macros for info logging
 */
#if LOG_LEVEL <= LOG_LEVEL_INFO
#define logInfo(msg...)					Logger::println(msg)
#define logInfon(msg...)				Logger::print(msg)
#define LOGGING_INFO					true
#else
#define logInfo(msg...)					{};
#define logInfon(msg...)				{};
#define LOGGING_INFO					false
#endif

/**
 * define a log macros for warning logging
 */
#if LOG_LEVEL <= LOG_LEVEL_WARN
#define logWarn(msg...)					Logger::println(msg)
#define logWarnn(msg...)				Logger::print(msg)
#define LOGGING_WARN					true
#else
#define logWarn(msg...)					{};
#define logWarnn(msg...)				{};
#define LOGGING_WARN					false
#endif

/**
 * define a log macros for debug logging
 */
#if LOG_LEVEL <= LOG_LEVEL_DEBUG
#define logDebug(msg...)				Logger::println(msg)
#define logDebugn(msg...)				Logger::print(msg)
#define LOGGING_DEBUG					true
#else
#define logDebug(msg...)				{};
#define logDebugn(msg...)				{};
#define LOGGING_DEBUG					false
#endif


#endif
