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

#include <LibRT/ErrorCode.hh>

#include <CCLang/Forward.hh>

#include <CCLang/Core/SourceLocation.hh>
#include <CCLang/Lang/StringView.hh>

class Error final {
public:
    enum class FromSyscall : bool {
        Yes,
        No
    };

public:
    /**
     * @brief Error safe factory functions
     */
    static auto from_code(ErrorCode      error_code,
                          FromSyscall    from_syscall    = FromSyscall::No,
                          SourceLocation source_location = SourceLocation::from_here()) -> Error;
    static auto from_literal(StringView     string_literals,
                             FromSyscall    from_syscall    = FromSyscall::No,
                             SourceLocation source_location = SourceLocation::from_here()) -> Error;

    /**
     * @brief Getters
     */
    auto code() const -> ErrorCode;
    auto string_literal() const -> StringView;
    auto is_from_syscall() const -> FromSyscall;
    auto source_location() const -> SourceLocation;

    auto operator==(ErrorCode const& rhs) const -> bool;
    auto operator==(StringView rhs) const -> bool;

private:
    explicit Error(ErrorCode error_code, StringView string_literals, FromSyscall from_syscall, SourceLocation source_location);

private:
    ErrorCode      m_error_code;
    StringView     m_string_literal;
    FromSyscall    m_from_syscall;
    SourceLocation m_error_location;
};
