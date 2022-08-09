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

#include <LibTC/CharTypes.hh>
#include <LibTC/Collection/String.hh>
#include <LibTC/Collection/StringBuilder.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Try.hh>

namespace TC::Collection {

auto String::construct_empty() -> String {
    return MUST(try_construct_empty());
}

auto String::construct_from_other(const String& rhs) -> String {
    return MUST(try_construct_from_other(rhs));
}

auto String::construct_from_view(StringView string_view) -> String {
    return MUST(try_construct_from_view(string_view));
}

auto String::try_construct_empty() -> ErrorOr<String> {
    return try_construct_from_view(""sv);
}

auto String::try_construct_from_other(const String& rhs) -> ErrorOr<String> {
    return try_construct_from_view(rhs.as_string_view());
}

auto String::try_construct_from_view(StringView string_view) -> ErrorOr<String> {
    return String{ TRY(StringStorage::try_construct_from_view(string_view)) };
}

auto String::clone() const -> String {
    return String{ m_string_storage_ref.clone() };
}

String::String(String&& rhs) noexcept
    : m_string_storage_ref{ Cxx::move(rhs.m_string_storage_ref) } {
}

auto String::operator=(String&& rhs) noexcept -> String& {
    String string{ Cxx::move(rhs) };
    swap(string);
    return *this;
}

auto String::swap(String& rhs) noexcept -> void {
    m_string_storage_ref.swap(rhs.m_string_storage_ref);
}

auto String::at(usize index) const -> char const& {
    return as_string_view().at(index);
}

auto String::operator[](usize index) const -> char const& {
    return at(index);
}

auto String::compare(StringView rhs) const -> int {
    return as_string_view().compare(rhs);
}

auto String::equals_ignore_case(StringView rhs) const -> bool {
    return as_string_view().equals_ignore_case(rhs);
}

auto String::sub_string(usize start) const -> String {
    return MUST(try_sub_string(start));
}

auto String::sub_string(usize start, usize count) const -> String {
    return MUST(try_sub_string(start, count));
}

auto String::try_sub_string(usize start) const -> ErrorOr<String> {
    return try_construct_from_view(sub_string_view(start));
}

auto String::try_sub_string(usize start, usize count) const -> ErrorOr<String> {
    return try_construct_from_view(sub_string_view(start, count));
}

auto String::sub_string_view(usize start) const -> StringView {
    return as_string_view().sub_string_view(start);
}

auto String::sub_string_view(usize start, usize count) const -> StringView {
    return as_string_view().sub_string_view(start, count);
}

auto String::trim(StringView chars, TrimMode trim_mode) const -> String {
    return MUST(try_trim(chars, trim_mode));
}

auto String::try_trim(StringView chars, TrimMode trim_mode) const -> ErrorOr<String> {
    return try_construct_from_view(trim_view(chars, trim_mode));
}

auto String::trim_whitespaces(TrimMode trim_mode) const -> String {
    return MUST(try_trim_whitespaces(trim_mode));
}

auto String::try_trim_whitespaces(TrimMode trim_mode) const -> ErrorOr<String> {
    return try_construct_from_view(trim_whitespaces_view(trim_mode));
}

auto String::trim_view(StringView chars, TrimMode trim_mode) const -> StringView {
    return as_string_view().trim(chars, trim_mode);
}

auto String::trim_whitespaces_view(TrimMode trim_mode) const -> StringView {
    return as_string_view().trim_whitespaces(trim_mode);
}

auto String::starts_with(StringView rhs, CaseSensitivity case_sensitivity) const -> bool {
    return as_string_view().starts_with(rhs, case_sensitivity);
}

auto String::starts_with(char rhs, CaseSensitivity case_sensitivity) const -> bool {
    return as_string_view().starts_with(rhs, case_sensitivity);
}

auto String::ends_with(StringView rhs, CaseSensitivity case_sensitivity) const -> bool {
    return as_string_view().ends_with(rhs, case_sensitivity);
}

auto String::ends_with(char rhs, CaseSensitivity case_sensitivity) const -> bool {
    return as_string_view().ends_with(rhs, case_sensitivity);
}

template<typename T>
auto String::as_int(TrimWhitespace trim_whitespace) const -> Option<T> {
    return as_string_view().as_int<T>(trim_whitespace);
}

template auto String::as_int(TrimWhitespace trim_whitespace) const -> Option<i8>;
template auto String::as_int(TrimWhitespace trim_whitespace) const -> Option<i16>;
template auto String::as_int(TrimWhitespace trim_whitespace) const -> Option<i32>;
template auto String::as_int(TrimWhitespace trim_whitespace) const -> Option<i64>;

template<typename T>
auto String::as_uint(TrimWhitespace trim_whitespace) const -> Option<T> {
    return as_string_view().as_uint<T>(trim_whitespace);
}

template auto String::as_uint(TrimWhitespace trim_whitespace) const -> Option<u8>;
template auto String::as_uint(TrimWhitespace trim_whitespace) const -> Option<u16>;
template auto String::as_uint(TrimWhitespace trim_whitespace) const -> Option<u32>;
template auto String::as_uint(TrimWhitespace trim_whitespace) const -> Option<u64>;

template<typename T>
auto String::as_uint_from_hex(TrimWhitespace trim_whitespace) const -> Option<T> {
    return as_string_view().as_uint_from_hex<T>(trim_whitespace);
}

template auto String::as_uint_from_hex(TrimWhitespace trim_whitespace) const -> Option<u8>;
template auto String::as_uint_from_hex(TrimWhitespace trim_whitespace) const -> Option<u16>;
template auto String::as_uint_from_hex(TrimWhitespace trim_whitespace) const -> Option<u32>;
template auto String::as_uint_from_hex(TrimWhitespace trim_whitespace) const -> Option<u64>;

template<typename T>
auto String::as_uint_from_octal(TrimWhitespace trim_whitespace) const -> Option<T> {
    return as_string_view().as_uint_from_octal<T>(trim_whitespace);
}

template auto String::as_uint_from_octal(TrimWhitespace trim_whitespace) const -> Option<u8>;
template auto String::as_uint_from_octal(TrimWhitespace trim_whitespace) const -> Option<u16>;
template auto String::as_uint_from_octal(TrimWhitespace trim_whitespace) const -> Option<u32>;
template auto String::as_uint_from_octal(TrimWhitespace trim_whitespace) const -> Option<u64>;

auto String::find(char needle, size_t start) const -> Option<usize> {
    return as_string_view().find(needle, start);
}

auto String::find(StringView needle, size_t start) const -> Option<usize> {
    return as_string_view().find(needle, start);
}

auto String::find_last(char needle) const -> Option<usize> {
    return as_string_view().find_last(needle);
}

auto String::find_all(StringView needle) const -> Vector<usize> {
    return as_string_view().find_all(needle);
}

auto String::try_find_all(StringView needle) const -> ErrorOr<Vector<usize>> {
    return as_string_view().try_find_all(needle);
}

auto String::to_lowercase() const -> String {
    return MUST(try_to_lowercase());
}

auto String::try_to_lowercase() const -> ErrorOr<String> {
    auto string_builder = StringBuilder::construct_with_capacity(len());
    for ( auto const c : *this ) {
        if ( is_ascii_uppercase_alpha(c) )
            TRY(string_builder.try_append(to_ascii_lowercase(c)));
        else
            TRY(string_builder.try_append(c));
    }
    return string_builder.try_to_string();
}

auto String::to_uppercase() const -> String {
    return MUST(try_to_uppercase());
}

auto String::try_to_uppercase() const -> ErrorOr<String> {
    auto string_builder = StringBuilder::construct_with_capacity(len());
    for ( auto const c : *this ) {
        if ( is_ascii_lowercase_alpha(c) )
            TRY(string_builder.try_append(to_ascii_uppercase(c)));
        else
            TRY(string_builder.try_append(c));
    }
    return string_builder.try_to_string();
}

auto String::operator==(const String& rhs) const -> bool {
    return as_string_view() == rhs.as_string_view();
}

auto String::operator==(const StringView& rhs) const -> bool {
    return as_string_view() == rhs;
}

auto String::operator!=(const String& rhs) const -> bool {
    return as_string_view() != rhs.as_string_view();
}

auto String::operator!=(const StringView& rhs) const -> bool {
    return as_string_view() != rhs;
}

auto String::operator<(const String& rhs) const -> bool {
    return as_string_view() < rhs.as_string_view();
}

auto String::operator<(const StringView& rhs) const -> bool {
    return as_string_view() < rhs;
}

auto String::operator<=(const String& rhs) const -> bool {
    return as_string_view() <= rhs.as_string_view();
}

auto String::operator<=(const StringView& rhs) const -> bool {
    return as_string_view() <= rhs;
}

auto String::operator>(const String& rhs) const -> bool {
    return as_string_view() > rhs.as_string_view();
}

auto String::operator>(const StringView& rhs) const -> bool {
    return as_string_view() > rhs;
}

auto String::operator>=(const String& rhs) const -> bool {
    return as_string_view() >= rhs.as_string_view();
}

auto String::operator>=(const StringView& rhs) const -> bool {
    return as_string_view() >= rhs;
}

auto String::contains(StringView rhs, CaseSensitivity case_sensitivity) const -> bool {
    return as_string_view().contains(rhs, case_sensitivity);
}

auto String::contains(char rhs, CaseSensitivity case_sensitivity) const -> bool {
    return as_string_view().contains(rhs, case_sensitivity);
}

auto String::begin() const -> String::ConstIterator {
    return as_string_view().begin();
}

auto String::end() const -> String::ConstIterator {
    return as_string_view().end();
}

auto String::as_cstr() const -> char const* {
    return m_string_storage_ref->data_storage();
}

auto String::len() const -> usize {
    return m_string_storage_ref->char_count();
}

auto String::is_empty() const -> bool {
    return m_string_storage_ref->is_empty();
}

auto String::as_string_view() const -> StringView {
    return StringView{ as_cstr(), len() };
}

String::String(NonNullRef<StringStorage> string_storage)
    : m_string_storage_ref{ Cxx::move(string_storage) } {
}

} /* namespace TC::Collection */
