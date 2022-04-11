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

SourceLocation SourceLocation::here(char const* file_path, char const* function, u32 line) {
    return SourceLocation{ file_path, function, line };
}

SourceLocation::SourceLocation(char const* file_path, char const* function, u32 line)
    : m_file_path{ file_path }
    , m_function{ function }
    , m_line{ line } {
}

char const* SourceLocation::file_path() const {
    return m_file_path;
}

char const* SourceLocation::function() const {
    return m_function;
}

u32 SourceLocation::line() const {
    return m_line;
}

} /* namespace TC */