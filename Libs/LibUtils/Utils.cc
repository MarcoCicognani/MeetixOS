/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#include <Api.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwctype>
#include <LibUtils/Utils.hh>

namespace Utils {

std::string trim(const std::string& str) {
    if ( !str.empty() ) {
        auto first = str.find_first_not_of(" \f\n\r\t\v");
        auto last  = str.find_last_not_of(" \f\n\r\t\v");
        if ( first == std::string::npos || last == std::string::npos )
            return str;

        auto res = str.substr(first, last - first + 1);
        return res;
    }
    return str;
}

void log(const std::string& message, ...) {
    va_list l;
    va_start(l, message);
    log(message.c_str(), l);
    va_end(l);
}

void log(const char* message, ...) {
    va_list l;
    va_start(l, message);
    log(message, l);
    va_end(l);
}

void log(const char* message, va_list l) {
    usize message_len = strlen(message);
    usize buffer_len  = message_len * 2;
    char* buffer      = new char[buffer_len];

    vsnprintf(buffer, buffer_len, message, l);
    s_log(buffer);
    delete[] buffer;
}

namespace PortIO {

u8 read_u8(u16 port) {
    u8 value;
    asm volatile("inb %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

u16 read_u16(u16 port) {
    u16 value;
    asm volatile("inw %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

u32 read_u32(u16 port) {
    u32 value;
    asm volatile("inl %1, %0" : "=a"(value) : "dN"(port));
    return value;
}

void write_u8(u16 port, u8 value) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

void write_u16(u16 port, u16 value) {
    asm volatile("outw %1, %0" : : "dN"(port), "a"(value));
}

void write_u32(u16 port, u32 value) {
    asm volatile("outl %1, %0" : : "dN"(port), "a"(value));
}

} /* namespace PortIO */
} /* namespace Utils */
