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
    [[nodiscard]]
    static auto from_code(ErrorCode,
                          FromSyscall    = FromSyscall::No,
                          SourceLocation = SourceLocation::new_from_here()) -> Error;
    [[nodiscard]]
    static auto from_literal(StringView,
                             FromSyscall    = FromSyscall::No,
                             SourceLocation = SourceLocation::new_from_here()) -> Error;

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto code() const -> ErrorCode;
    [[nodiscard]]
    auto string_literal() const -> StringView;
    [[nodiscard]]
    auto is_from_syscall() const -> FromSyscall;
    [[nodiscard]]
    auto source_location() const -> SourceLocation;

    [[nodiscard]]
    auto operator==(ErrorCode const&) const -> bool;
    [[nodiscard]]
    auto operator==(StringView) const -> bool;

private:
    explicit Error(ErrorCode, StringView, FromSyscall, SourceLocation);

private:
    ErrorCode      m_error_code;
    StringView     m_string_literal;
    FromSyscall    m_from_syscall;
    SourceLocation m_error_location;
};
