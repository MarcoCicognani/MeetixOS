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
OSError Error::os_error() const {
    return m_os_error;
}

char const* Error::string_literal() const {
    return m_string_literal;
}

Error::FromSyscall Error::from_syscall() const {
    return m_from_syscall;
}

SourceLocation Error::source_location() const {
    return m_error_location;
}

bool Error::operator==(const OSError& rhs) const {
    return m_os_error == rhs;
}

bool Error::operator==(const char* rhs) const {
    return StringView{ m_string_literal } == StringView{ rhs };
}

} /* namespace TC */