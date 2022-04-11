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

#include <LibTC/IntTypes.hh>

namespace TC {

class SourceLocation {
public:
    /**
     * @brief Returns a SourceLocation from the current location
     */
    [[nodiscard]] static SourceLocation here(char const* file_path = __builtin_FILE(),
                                             char const* function  = __builtin_FUNCTION(),
                                             u32         line      = __builtin_LINE());

    /**
     * @brief Constructors
     */
    SourceLocation(SourceLocation const&) = default;
    SourceLocation(SourceLocation&&)      = default;

    SourceLocation& operator=(SourceLocation const&) = default;
    SourceLocation& operator=(SourceLocation&&) = default;

    /**
     * @brief Getters
     */
    [[nodiscard]] char const* file_path() const;
    [[nodiscard]] char const* function() const;
    [[nodiscard]] u32         line() const;

private:
    SourceLocation(char const* file_path, char const* function, u32 line);

private:
    char const* m_file_path;
    char const* m_function;
    u32         m_line;
};

} /* namespace TC */