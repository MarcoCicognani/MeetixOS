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
     * @brief Constructors
     */
    explicit Error(OSError os_error, FromSyscall from_syscall = FromSyscall::No, SourceLocation error_location = SourceLocation::here());
    explicit Error(char const* string_literal, FromSyscall from_syscall = FromSyscall::No, SourceLocation error_location = SourceLocation::here());
    explicit Error(OSError        os_error,
                   char const*    string_literal,
                   FromSyscall    from_syscall   = FromSyscall::No,
                   SourceLocation error_location = SourceLocation::here());

    /**
     * @brief Getters
     */
    [[nodiscard]] auto os_error() const -> OSError;
    [[nodiscard]] auto string_literal() const -> char const*;
    [[nodiscard]] auto from_syscall() const -> FromSyscall;
    [[nodiscard]] auto source_location() const -> SourceLocation;

    [[nodiscard]] auto operator==(OSError const& rhs) const -> bool;
    [[nodiscard]] auto operator==(char const* rhs) const -> bool;

private:
    OSError        m_os_error;
    char const*    m_string_literal;
    FromSyscall    m_from_syscall;
    SourceLocation m_error_location;
};

} /* namespace TC */