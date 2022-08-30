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

#include <LibC/errno.h>
#include <LibTC/SourceLocation.hh>

namespace TC {

/* TODO When refactor all to C++ modules use StringView instead of char const* */

class Error {
public:
    enum class FromSyscall {
        Yes,
        No
    };

public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto construct_from_errno(ErrnoCode      errno_code,
                                     FromSyscall    from_syscall   = FromSyscall::No,
                                     SourceLocation error_location = SourceLocation::construct_from_here()) -> Error;
    [[nodiscard]]
    static auto construct_from_literal(char const*    string_literal,
                                       FromSyscall    from_syscall   = FromSyscall::No,
                                       SourceLocation error_location = SourceLocation::construct_from_here()) -> Error;

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto errno_code() const -> ErrnoCode;
    [[nodiscard]]
    auto string_literal() const -> char const*;
    [[nodiscard]]
    auto is_from_syscall() const -> FromSyscall;
    [[nodiscard]]
    auto source_location() const -> SourceLocation;

    [[nodiscard]]
    auto operator==(ErrnoCode const& rhs) const -> bool;
    [[nodiscard]]
    auto operator==(char const* rhs) const -> bool;

private:
    explicit constexpr Error(ErrnoCode errno_code, char const* string_literal, FromSyscall from_syscall, SourceLocation error_location)
        : m_errno_code{ errno_code }
        , m_string_literal{ string_literal }
        , m_from_syscall{ from_syscall }
        , m_error_location{ error_location } {
    }

private:
    ErrnoCode      m_errno_code;
    char const*    m_string_literal;
    FromSyscall    m_from_syscall;
    SourceLocation m_error_location;
};

} /* namespace TC */