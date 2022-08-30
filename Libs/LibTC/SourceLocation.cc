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

#include <LibTC/SourceLocation.hh>

namespace TC {

auto SourceLocation::construct_from_here(const char* file_path, const char* function, u32 line) -> SourceLocation {
    return SourceLocation{ file_path, function, line };
}

auto SourceLocation::file_path() const -> char const* {
    return m_file_path;
}

auto SourceLocation::function() const -> char const* {
    return m_function;
}

auto SourceLocation::line() const -> u32 {
    return m_line;
}

} /* namespace TC */