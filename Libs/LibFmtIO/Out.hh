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

#include <LibFmtIO/VOut.hh>

namespace FmtIO {

template<typename... TArgs>
auto out(StringView format_view, TArgs&&... args) -> ErrorOr<void> {
    return vout(stdout, format_view, Cxx::forward<TArgs>(args)...);
}

template<typename... TArgs>
auto outln(StringView format_view, TArgs&&... args) -> ErrorOr<void> {
    return voutln(stdout, format_view, Cxx::forward<TArgs>(args)...);
}

} /* namespace FmtIO */