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

#include <CCLang/Alloc/String.hh>
#include <CCLang/Alloc/StringBuilder.hh>
#include <CCLang/Core/CharTypes.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/StringView.hh>
#include <CCLang/Lang/Try.hh>

auto String::new_empty() -> String {
    return must$(try_new_empty());
}

auto String::new_from_other(const String& rhs) -> String {
    return must$(try_new_from_other(rhs));
}

auto String::new_from_view(StringView string_view) -> String {
    return must$(try_new_from_view(string_view));
}

auto String::try_new_empty() -> ErrorOr<String> {
    return try_new_from_view(""sv);
}

auto String::try_new_from_other(const String& rhs) -> ErrorOr<String> {
    return try_new_from_view(rhs.as_string_view());
}

auto String::try_new_from_view(StringView string_view) -> ErrorOr<String> {
    return String{ try$(StringStorage::try_new_from_view(string_view)) };
}

auto String::clone() const -> String {
    return String{ m_string_storage_ref.clone() };
}

String::String(String&& rhs)
    : m_string_storage_ref{ Cxx::move(rhs.m_string_storage_ref) } {
}

auto String::operator=(String&& rhs) -> String& {
    String string{ Cxx::move(rhs) };
    swap(string);
    return *this;
}

auto String::swap(String& rhs) -> void {
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
    return must$(try_sub_string(start));
}

auto String::sub_string(usize start, usize count) const -> String {
    return must$(try_sub_string(start, count));
}

auto String::try_sub_string(usize start) const -> ErrorOr<String> {
    return try_new_from_view(sub_string_view(start));
}

auto String::try_sub_string(usize start, usize count) const -> ErrorOr<String> {
    return try_new_from_view(sub_string_view(start, count));
}

auto String::sub_string_view(usize start) const -> StringView {
    return as_string_view().sub_string_view(start);
}

auto String::sub_string_view(usize start, usize count) const -> StringView {
    return as_string_view().sub_string_view(start, count);
}

auto String::trim(StringView chars, TrimMode trim_mode) const -> String {
    return must$(try_trim(chars, trim_mode));
}

auto String::try_trim(StringView chars, TrimMode trim_mode) const -> ErrorOr<String> {
    return try_new_from_view(trim_view(chars, trim_mode));
}

auto String::trim_whitespaces(TrimMode trim_mode) const -> String {
    return must$(try_trim_whitespaces(trim_mode));
}

auto String::try_trim_whitespaces(TrimMode trim_mode) const -> ErrorOr<String> {
    return try_new_from_view(trim_whitespaces_view(trim_mode));
}

auto String::trim_view(StringView chars, TrimMode trim_mode) const -> StringView {
    return as_string_view().trim(chars, trim_mode);
}

auto String::trim_whitespaces_view(TrimMode trim_mode) const -> StringView {
    return as_string_view().trim_whitespaces(trim_mode);
}

auto String::starts_with(StringView rhs, CaseSensible case_sensible) const -> bool {
    return as_string_view().starts_with(rhs, case_sensible);
}

auto String::starts_with(char rhs, CaseSensible case_sensible) const -> bool {
    return as_string_view().starts_with(rhs, case_sensible);
}

auto String::ends_with(StringView rhs, CaseSensible case_sensible) const -> bool {
    return as_string_view().ends_with(rhs, case_sensible);
}

auto String::ends_with(char rhs, CaseSensible case_sensible) const -> bool {
    return as_string_view().ends_with(rhs, case_sensible);
}

template<typename T>
auto String::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<T> {
    return as_string_view().as_int<T>(int_base, parse_mode);
}

template auto String::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<i8>;
template auto String::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<i16>;
template auto String::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<i32>;
template auto String::as_int(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<i64>;

template<typename T>
auto String::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<T> {
    return as_string_view().as_uint<T>(int_base, parse_mode);
}

template auto String::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<u8>;
template auto String::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<u16>;
template auto String::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<u32>;
template auto String::as_uint(IntBase int_base, ParseMode parse_mode) const -> ErrorOr<u64>;

auto String::find(char needle, usize start) const -> Option<usize> {
    return as_string_view().find(needle, start);
}

auto String::find(StringView needle, usize start) const -> Option<usize> {
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
    return must$(try_to_lowercase());
}

auto String::try_to_lowercase() const -> ErrorOr<String> {
    auto string_builder = try$(StringBuilder::try_new_with_capacity(len()));
    for ( auto const c : *this ) {
        if ( is_ascii_uppercase_alpha(c) )
            try$(string_builder.try_append(to_ascii_lowercase(c)));
        else
            try$(string_builder.try_append(c));
    }
    return string_builder.try_to_string();
}

auto String::to_uppercase() const -> String {
    return must$(try_to_uppercase());
}

auto String::try_to_uppercase() const -> ErrorOr<String> {
    auto string_builder = try$(StringBuilder::try_new_with_capacity(len()));
    for ( auto const c : *this ) {
        if ( is_ascii_lowercase_alpha(c) )
            try$(string_builder.try_append(to_ascii_uppercase(c)));
        else
            try$(string_builder.try_append(c));
    }
    return string_builder.try_to_string();
}

auto String::to_reverse() const -> String {
    return must$(try_to_reverse());
}

auto String::try_to_reverse() const -> ErrorOr<String> {
    auto string_builder = try$(StringBuilder::try_new_with_capacity(len()));
    for ( auto const c : reverse_iter() )
        try$(string_builder.try_append(c));

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

auto String::contains(StringView rhs, CaseSensible case_sensible) const -> bool {
    return as_string_view().contains(rhs, case_sensible);
}

auto String::contains(char rhs, CaseSensible case_sensible) const -> bool {
    return as_string_view().contains(rhs, case_sensible);
}

auto String::begin() const -> ConstIterator {
    return as_string_view().begin();
}

auto String::end() const -> ConstIterator {
    return as_string_view().end();
}

auto String::rbegin() const -> ConstReverseIterator {
    return as_string_view().rbegin();
}

auto String::rend() const -> ConstReverseIterator {
    return as_string_view().rend();
}

auto String::reverse_iter() const -> ConstReverseIteratorWrapper {
    return as_string_view().reverse_iter();
}

auto String::as_cstr() const -> char const* {
    return m_string_storage_ref->storage_ptr();
}

auto String::len() const -> usize {
    return m_string_storage_ref->len();
}

auto String::count() const -> usize {
    return m_string_storage_ref->len();
}

auto String::is_empty() const -> bool {
    return m_string_storage_ref->is_empty();
}

auto String::as_string_view() const -> StringView {
    return StringView::new_from_raw_parts(as_cstr(), len());
}

String::String(NonNullRef<StringStorage> string_storage)
    : m_string_storage_ref{ Cxx::move(string_storage) } {
}
