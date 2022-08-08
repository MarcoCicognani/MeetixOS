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

#include <LibC/string.h>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Error.hh>

namespace TC {

Error::Error(OSError os_error, FromSyscall from_syscall, SourceLocation error_location)
    : m_os_error{ os_error }
    , m_string_literal{ strerror(os_error) }
    , m_from_syscall{ from_syscall }
    , m_error_location{ error_location } {
}

Error::Error(char const* string_literal, FromSyscall from_syscall, SourceLocation error_location)
    : m_os_error{ ECUSTOM }
    , m_string_literal{ string_literal }
    , m_from_syscall{ from_syscall }
    , m_error_location{ error_location } {
}

Error::Error(OSError os_error, char const* string_literal, FromSyscall from_syscall, SourceLocation error_location)
    : m_os_error{ os_error }
    , m_string_literal{ string_literal }
    , m_from_syscall{ from_syscall }
    , m_error_location{ error_location } {
}
auto Error::os_error() const -> OSError {
    return m_os_error;
}

auto Error::string_literal() const -> char const* {
    return m_string_literal;
}

auto Error::from_syscall() const -> Error::FromSyscall {
    return m_from_syscall;
}

auto Error::source_location() const -> SourceLocation {
    return m_error_location;
}

auto Error::operator==(const OSError& rhs) const -> bool {
    return m_os_error == rhs;
}

auto Error::operator==(const char* rhs) const -> bool {
    /* TODO - find a better way */
    return StringView{ m_string_literal, __builtin_strlen(m_string_literal) } == StringView{ rhs, __builtin_strlen(rhs) };
}

} /* namespace TC */