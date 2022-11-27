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

#include <CCLang/Core/Error.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/StringView.hh>

auto Error::from_code(ErrorCode code, FromSyscall from_syscall, SourceLocation error_location) -> Error {
    return Error(code, {}, from_syscall, Cxx::move(error_location));
}

auto Error::from_literal(StringView string_literal, FromSyscall from_syscall, SourceLocation error_location) -> Error {
    return Error(ErrorCode::None, string_literal, from_syscall, Cxx::move(error_location));
}

auto Error::code() const -> ErrorCode {
    return m_error_code;
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

auto Error::operator==(ErrorCode const& rhs) const -> bool {
    return m_error_code == rhs;
}

auto Error::operator==(StringView rhs) const -> bool {
    return m_string_literal == rhs;
}

Error::Error(ErrorCode code, StringView string_literal, Error::FromSyscall from_syscall, SourceLocation error_location)
    : m_error_code(code)
    , m_string_literal(Cxx::move(string_literal))
    , m_from_syscall(from_syscall)
    , m_error_location(Cxx::move(error_location)) {
}
