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

#include "TextInspector.hh"

#include <sstream>

std::string TextInspector::inspection() {
    std::stringstream ss{};
    ss << m_stream.rdbuf();
    auto str = ss.str();

    if ( std::any_of(str.begin(), str.end(), [](int c) {
             return c == 0x09 || c == 0x0A || c == 0x0D || (0x20 <= c && c <= 0x7E);
         }) )
        return { "UTF-8 Encoded text file\n" };
    else
        return { "ASCII text file\n" };
}
