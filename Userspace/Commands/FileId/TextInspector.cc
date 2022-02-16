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

#include "TextInspector.hh"

#include <algorithm>
#include <sstream>

std::string TextInspector::inspection() {
    std::stringstream ss{};
    ss << m_stream.rdbuf();
    auto str = ss.str();

    auto utf8_check = [](int c) { return c == 0x09 || c == 0x0A || c == 0x0D || (0x20 <= c && c <= 0x7E); };

    if ( std::ranges::any_of(str, utf8_check) )
        return { "UTF-8 Encoded text file\n" };
    else
        return { "ASCII text file\n" };
}
