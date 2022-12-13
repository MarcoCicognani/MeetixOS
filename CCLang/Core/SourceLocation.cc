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

#include <CCLang/Core/SourceLocation.hh>

auto SourceLocation::from_here(const char* file_path, const char* function, u32 line) -> SourceLocation {
    return SourceLocation(file_path, function, line);
}

auto SourceLocation::file_path() const -> StringView {
    return m_file_path;
}

auto SourceLocation::function() const -> StringView {
    return m_function;
}

auto SourceLocation::line() const -> u32 {
    return m_line;
}

SourceLocation::SourceLocation(const char* file_path, const char* function, u32 line)
    : m_file_path(StringView::from_cstr(file_path))
    , m_function(StringView::from_cstr(function))
    , m_line(line) {
}
