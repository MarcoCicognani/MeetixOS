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

#include <LibFmtIO/VOut.hh>

namespace FmtIO {

auto foreground(Color foreground) -> StringView {
    switch ( foreground ) {
        case Color::Black:
            return "\e[30m"sv;
        case Color::Red:
            return "\e[31m"sv;
        case Color::Green:
            return "\e[32m"sv;
        case Color::Yellow:
            return "\e[33m"sv;
        case Color::Blue:
            return "\e[34m"sv;
        case Color::Magenta:
            return "\e[35m"sv;
        case Color::Cyan:
            return "\e[36m"sv;
        case Color::White:
            return "\e[37m"sv;
        case Color::LGreen:
            return "\e[38m"sv;
    }
    return reset();
}

auto background(Color background) -> StringView {
    switch ( background ) {
        case Color::Black:
            return "\e[40m"sv;
        case Color::Red:
            return "\e[41m"sv;
        case Color::Green:
            return "\e[42m"sv;
        case Color::Yellow:
            return "\e[43m"sv;
        case Color::Blue:
            return "\e[44m"sv;
        case Color::Magenta:
            return "\e[45m"sv;
        case Color::Cyan:
            return "\e[46m"sv;
        case Color::White:
            return "\e[47m"sv;
        case Color::LGreen:
            return "\e[48m"sv;
    }
    return reset();
}

auto reset() -> StringView {
    return "\e[0m"sv;
}

} /* namespace FmtIO */