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

#include <LibTC/Core/Error.hh>
#include <LibTC/Lang/Cxx.hh>
#include <LibTC/Lang/StringView.hh>

auto Error::construct_from_code(u32 code, FromSyscall from_syscall, SourceLocation error_location) -> Error {
    return Error{ code, ""sv, from_syscall, Cxx::move(error_location) };
}

auto Error::construct_from_literal(StringView string_literal, FromSyscall from_syscall, SourceLocation error_location) -> Error {
    return Error{ 0, string_literal, from_syscall, Cxx::move(error_location) };
}

auto Error::code() const -> u32 {
    return m_code;
}

auto Error::string_literal() const -> StringView {
    return m_string_literal;
}

auto Error::is_from_syscall() const -> FromSyscall {
    return m_from_syscall;
}

auto Error::source_location() const -> SourceLocation {
    return m_error_location;
}

auto Error::operator==(u32 const& rhs) const -> bool {
    return m_code == rhs;
}

auto Error::operator==(StringView rhs) const -> bool {
    return m_string_literal == rhs;
}

Error::Error(u32 code, StringView string_literal, Error::FromSyscall from_syscall, SourceLocation error_location)
    : m_code{ code }
    , m_string_literal{ Cxx::move(string_literal) }
    , m_from_syscall{ from_syscall }
    , m_error_location{ Cxx::move(error_location) } {
}
