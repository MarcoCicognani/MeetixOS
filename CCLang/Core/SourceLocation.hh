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

#pragma once

#include <CCLang/Forward.hh>

#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/StringView.hh>

class SourceLocation final {
public:
    /**
     * @brief Error safe factory function
     */
    static auto from_here(char const* file_path = __builtin_FILE(), char const* function = __builtin_FUNCTION(), u32 line = __builtin_LINE()) -> SourceLocation;

    /**
     * @brief Getters
     */
    auto file_path() const -> StringView;
    auto function() const -> StringView;
    auto line() const -> u32;

private:
    explicit SourceLocation(char const* file_path, char const* function, u32 line);

private:
    StringView m_file_path;
    StringView m_function;
    u32        m_line;
};
