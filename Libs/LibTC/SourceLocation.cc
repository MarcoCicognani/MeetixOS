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

auto SourceLocation::here(char const* file_path, char const* function, u32 line) -> SourceLocation {
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

constexpr SourceLocation::SourceLocation(char const* file_path, char const* function, u32 line) noexcept
    : m_file_path{ file_path }
    , m_function{ function }
    , m_line{ line } {
}

} /* namespace TC */