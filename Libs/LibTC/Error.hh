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

#include <LibC/errno.h>
#include <LibTC/SourceLocation.hh>

namespace TC {

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
    explicit Error(char const*    string_literal,
                   FromSyscall    from_syscall   = FromSyscall::No,
                   SourceLocation error_location = SourceLocation::here());
    explicit Error(OSError        os_error,
                   char const*    string_literal,
                   FromSyscall    from_syscall   = FromSyscall::No,
                   SourceLocation error_location = SourceLocation::here());

    /**
     * @brief Getters
     */
    [[nodiscard]] OSError        os_error() const;
    [[nodiscard]] char const*    string_literal() const;
    [[nodiscard]] FromSyscall    from_syscall() const;
    [[nodiscard]] SourceLocation source_location() const;

    [[nodiscard]] bool operator==(OSError const& rhs) const;
    [[nodiscard]] bool operator==(char const* rhs) const;

private:
    OSError        m_os_error;
    char const*    m_string_literal;
    FromSyscall    m_from_syscall;
    SourceLocation m_error_location;
};

} /* namespace TC */