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

auto Error::construct_from_errno(ErrnoCode errno_code, FromSyscall from_syscall, SourceLocation error_location) -> Error {
    return Error{ errno_code, nullptr, from_syscall, error_location };
}

auto Error::construct_from_literal(const char* string_literal, FromSyscall from_syscall, SourceLocation error_location) -> Error {
    return Error{ ECUSTOM, string_literal, from_syscall, error_location };
}

auto Error::errno_code() const -> ErrnoCode {
    return m_errno_code;
}

auto Error::string_literal() const -> char const* {
    return m_string_literal;
}

auto Error::is_from_syscall() const -> FromSyscall {
    return m_from_syscall;
}

auto Error::source_location() const -> SourceLocation {
    return m_error_location;
}

auto Error::operator==(const ErrnoCode& rhs) const -> bool {
    return m_errno_code == rhs;
}

auto Error::operator==(const char* rhs) const -> bool {
    /* TODO - find a better way */
    return StringView{ m_string_literal, __builtin_strlen(m_string_literal) } == StringView{ rhs, __builtin_strlen(rhs) };
}

} /* namespace TC */