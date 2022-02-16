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

#include <ctype.h>
#include <string.h>
#include <TC/Collection/String.hh>
#include <TC/Collection/StringBuilder.hh>
#include <TC/Collection/StringView.hh>
#include <TC/Cxx.hh>
#include <TC/Functional/Must.hh>
#include <TC/Functional/Try.hh>

namespace TC::Collection {

ErrorOr<String> String::try_from(char const* str) {
    return try_from(StringView{ str });
}

ErrorOr<String> String::try_from(char const* str, usize count) {
    return try_from(StringView{ str, count });
}

ErrorOr<String> String::try_from(StringView string_view) {
    return String{ TRY(StringStorage::try_from(string_view)) };
}

String::String()
    : String{ StringView{ "" } } {
}

String::String(char const* str)
    : String{ StringView{ str } } {
}

String::String(char const* str, usize count)
    : String{ StringView{ str, count } } {
}

String::String(StringView string_view)
    : String{ StringStorage::from(string_view) } {
}

String::String(String&& rhs) noexcept
    : m_string_storage{ move(rhs.m_string_storage) } {
}

String& String::operator=(char const* rhs) {
    String string{ rhs };
    swap(string);
    return *this;
}

String& String::operator=(StringView rhs) {
    String string{ rhs };
    swap(string);
    return *this;
}

String& String::operator=(String const& rhs) {
    String string{ rhs };
    swap(string);
    return *this;
}

String& String::operator=(String&& rhs) noexcept {
    String string{ move(rhs) };
    swap(string);
    return *this;
}

void String::swap(String& rhs) noexcept {
    m_string_storage.swap(rhs.m_string_storage);
}

char const& String::at(usize index) const {
    VERIFY_LESS(index, len());
    return m_string_storage->data()[index];
}

char const& String::operator[](usize index) const {
    return at(index);
}

int String::compare(StringView rhs) const {
    return as_view().compare(rhs);
}

bool String::equals_ignore_case(StringView rhs) const {
    return as_view().equals_ignore_case(rhs);
}

String String::sub_string(usize start) const {
    return MUST(try_sub_string(start));
}

String String::sub_string(usize start, usize count) const {
    return MUST(try_sub_string(start, count));
}

ErrorOr<String> String::try_sub_string(usize start) const {
    return try_from(sub_string_view(start));
}

ErrorOr<String> String::try_sub_string(usize start, usize count) const {
    return try_from(sub_string_view(start, count));
}

StringView String::sub_string_view(usize start) const {
    return as_view().sub_string(start);
}

StringView String::sub_string_view(usize start, usize count) const {
    return as_view().sub_string(start, count);
}

String String::trim(StringView chars, TrimMode trim_mode) const {
    return MUST(try_trim(chars, trim_mode));
}

ErrorOr<String> String::try_trim(StringView chars, TrimMode trim_mode) const {
    return try_from(trim_view(chars, trim_mode));
}

String String::trim_whitespaces(TrimMode trim_mode) const {
    return MUST(try_trim_whitespaces(trim_mode));
}

ErrorOr<String> String::try_trim_whitespaces(TrimMode trim_mode) const {
    return try_from(trim_whitespaces_view(trim_mode));
}

StringView String::trim_view(StringView chars, TrimMode trim_mode) const {
    return as_view().trim(chars, trim_mode);
}

StringView String::trim_whitespaces_view(TrimMode trim_mode) const {
    return as_view().trim_whitespaces(trim_mode);
}

bool String::starts_with(StringView rhs, CaseSensitivity case_sensitivity) const {
    return as_view().starts_with(rhs, case_sensitivity);
}

bool String::starts_with(char rhs, CaseSensitivity case_sensitivity) const {
    return as_view().starts_with(rhs, case_sensitivity);
}

bool String::ends_with(StringView rhs, CaseSensitivity case_sensitivity) const {
    return as_view().ends_with(rhs, case_sensitivity);
}

bool String::ends_with(char rhs, CaseSensitivity case_sensitivity) const {
    return as_view().ends_with(rhs, case_sensitivity);
}

template<typename T>
Option<T> String::as_int(TrimWhitespace trim_whitespace) const {
    return as_view().as_int<T>(trim_whitespace);
}

template Option<i8>  String::as_int(TrimWhitespace trim_whitespace) const;
template Option<i16> String::as_int(TrimWhitespace trim_whitespace) const;
template Option<i32> String::as_int(TrimWhitespace trim_whitespace) const;
template Option<i64> String::as_int(TrimWhitespace trim_whitespace) const;

template<typename T>
Option<T> String::as_uint(TrimWhitespace trim_whitespace) const {
    return as_view().as_uint<T>(trim_whitespace);
}

template Option<u8>  String::as_uint(TrimWhitespace trim_whitespace) const;
template Option<u16> String::as_uint(TrimWhitespace trim_whitespace) const;
template Option<u32> String::as_uint(TrimWhitespace trim_whitespace) const;
template Option<u64> String::as_uint(TrimWhitespace trim_whitespace) const;

template<typename T>
Option<T> String::as_uint_from_hex(TrimWhitespace trim_whitespace) const {
    return as_view().as_uint_from_hex<T>(trim_whitespace);
}

template Option<u8>  String::as_uint_from_hex(TrimWhitespace trim_whitespace) const;
template Option<u16> String::as_uint_from_hex(TrimWhitespace trim_whitespace) const;
template Option<u32> String::as_uint_from_hex(TrimWhitespace trim_whitespace) const;
template Option<u64> String::as_uint_from_hex(TrimWhitespace trim_whitespace) const;

template<typename T>
Option<T> String::as_uint_from_octal(TrimWhitespace trim_whitespace) const {
    return as_view().as_uint_from_octal<T>(trim_whitespace);
}

template Option<u8>  String::as_uint_from_octal(TrimWhitespace trim_whitespace) const;
template Option<u16> String::as_uint_from_octal(TrimWhitespace trim_whitespace) const;
template Option<u32> String::as_uint_from_octal(TrimWhitespace trim_whitespace) const;
template Option<u64> String::as_uint_from_octal(TrimWhitespace trim_whitespace) const;

Option<usize> String::find(char needle, size_t start) const {
    return as_view().find(needle, start);
}

Option<usize> String::find(StringView needle, size_t start) const {
    return as_view().find(needle, start);
}

Option<usize> String::find_last(char needle) const {
    return as_view().find_last(needle);
}

Vector<usize> String::find_all(StringView needle) const {
    return as_view().find_all(needle);
}

ErrorOr<Vector<usize>> String::try_find_all(StringView needle) const {
    return as_view().try_find_all(needle);
}

String String::to_lowercase() const {
    return MUST(try_to_lowercase());
}

ErrorOr<String> String::try_to_lowercase() const {
    StringBuilder string_builder{};
    for ( auto c : *this ) {
        if ( isupper(c) && isalpha(c) )
            TRY(string_builder.try_append(tolower(c)));
        else
            TRY(string_builder.try_append(c));
    }
    return string_builder.try_to_string();
}

String String::to_uppercase() const {
    return MUST(try_to_uppercase());
}

ErrorOr<String> String::try_to_uppercase() const {
    StringBuilder string_builder{};
    for ( auto c : *this ) {
        if ( islower(c) && isalpha(c) )
            TRY(string_builder.try_append(toupper(c)));
        else
            TRY(string_builder.try_append(c));
    }
    return string_builder.try_to_string();
}

bool String::operator==(const StringView& rhs) const {
    return as_view() == rhs;
}

bool String::operator!=(const StringView& rhs) const {
    return as_view() != rhs;
}

bool String::operator<(const StringView& rhs) const {
    return as_view() < rhs;
}

bool String::operator<=(const StringView& rhs) const {
    return as_view() <= rhs;
}

bool String::operator>(const StringView& rhs) const {
    return as_view() > rhs;
}

bool String::operator>=(const StringView& rhs) const {
    return as_view() >= rhs;
}

bool String::contains(StringView rhs, CaseSensitivity case_sensitivity) const {
    return as_view().contains(rhs, case_sensitivity);
}

bool String::contains(char rhs, CaseSensitivity case_sensitivity) const {
    return as_view().contains(rhs, case_sensitivity);
}

String::ConstIterator String::begin() const {
    return as_view().begin();
}

String::ConstIterator String::end() const {
    return as_view().end();
}

char const* String::as_cstr() const {
    return m_string_storage->data();
}

usize String::len() const {
    return m_string_storage->count();
}

bool String::is_empty() const {
    return m_string_storage->is_empty();
}

StringView String::as_view() const {
    return StringView{ as_cstr(), len() };
}

String::String(NonNullRef<StringStorage>&& string_storage)
    : m_string_storage{ move(string_storage) } {
}

} /* namespace TC::Collection */
