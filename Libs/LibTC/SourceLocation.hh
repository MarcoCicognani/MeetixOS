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

#include <LibTC/Forward.hh>

#include <LibTC/IntTypes.hh>

namespace TC {

/* TODO When refactor all to C++ modules use StringView instead of char const* */

class SourceLocation {
public:
    /**
     * @brief Returns a SourceLocation from the current location
     */
    [[nodiscard]] static auto here(char const* file_path = __builtin_FILE(),
                                   char const* function  = __builtin_FUNCTION(),
                                   u32         line      = __builtin_LINE()) -> SourceLocation;

    /**
     * @brief Constructors
     */
    SourceLocation(SourceLocation const&) = default;
    SourceLocation(SourceLocation&&)      = default;

    auto operator=(SourceLocation const&) -> SourceLocation& = default;
    auto operator=(SourceLocation&&) -> SourceLocation&      = default;

    /**
     * @brief Getters
     */
    [[nodiscard]] auto file_path() const -> char const*;
    [[nodiscard]] auto function() const -> char const*;
    [[nodiscard]] auto line() const -> u32;

private:
    explicit constexpr SourceLocation(char const* file_path, char const* function, u32 line) noexcept;

private:
    char const* m_file_path;
    char const* m_function;
    u32         m_line;
};

} /* namespace TC */