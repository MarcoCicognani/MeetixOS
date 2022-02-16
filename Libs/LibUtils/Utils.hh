/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <cstdarg>
#include <string>

namespace Utils {

/* ----------------------------------- C++ function prototypes ---------------------------------- */

/**
 * @brief Trims the whitespaces of the given string
 */
std::string trim(const std::string& str);

/**
 * @brief Prints a formatted log using the kernel s_log() interface
 */
void log(const std::string& message, ...);
void log(const char* message, ...) A_PRINTF(1, 2);
void log(const char* message, va_list l);

namespace PortIO {

/**
 * @brief Read from the given x86 I/O <port>
 */
u8  read_u8(u16 port);
u16 read_u16(u16 port);
u32 read_u32(u16 port);

/**
 * @brief Writes the given <value> into the given x86 I/O <port>
 */
void write_u8(u16 port, u8 value);
void write_u16(u16 port, u16 value);
void write_u32(u16 port, u32 value);

} /* namespace PortIO */
} /* namespace Utils */