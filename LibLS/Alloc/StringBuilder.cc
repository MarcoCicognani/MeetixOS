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

#include <LibLS/Alloc/StringBuilder.hh>
#include <LibLS/Lang/Must.hh>
#include <LibLS/Lang/Try.hh>

auto StringBuilder::new_empty() -> StringBuilder {
    return StringBuilder{};
}

auto StringBuilder::new_with_capacity(usize capacity) -> StringBuilder {
    return must$(try_new_with_capacity(capacity));
}

auto StringBuilder::new_from_other(const StringBuilder& rhs) -> StringBuilder {
    return must$(try_new_from_other(rhs));
}

auto StringBuilder::try_new_with_capacity(usize capacity) -> ErrorOr<StringBuilder> {
    auto string_builder = new_empty();
    try$(string_builder.try_ensure_capacity(capacity));
    return string_builder;
}

auto StringBuilder::try_new_from_other(const StringBuilder& rhs) -> ErrorOr<StringBuilder> {
    auto string_builder = try$(try_new_with_capacity(rhs.len()));
    for ( auto const& c : rhs.as_string_view() )
        try$(string_builder.try_append(c));

    return string_builder;
}

auto StringBuilder::clone() const -> StringBuilder {
    return must$(try_clone());
}

auto StringBuilder::try_clone() const -> ErrorOr<StringBuilder> {
    return StringBuilder::try_new_from_other(*this);
}

auto StringBuilder::clear() -> void {
    m_char_vector.clear();
}

auto StringBuilder::clear_keep_capacity() -> void {
    m_char_vector.clear_keep_capacity();
}

auto StringBuilder::append(char c) -> void {
    must$(try_append(c));
}

auto StringBuilder::try_append(char c) -> ErrorOr<void> {
    return m_char_vector.try_append(c);
}

auto StringBuilder::append(StringView string_view) -> void {
    must$(try_append(string_view));
}

auto StringBuilder::try_append(StringView string_view) -> ErrorOr<void> {
    if ( string_view.is_null_or_empty() )
        return {};

    /* first ensure the right capacity then append unchecked for speed */
    try$(try_ensure_capacity(len() + string_view.len()));
    for ( auto const c : string_view )
        m_char_vector.append_unchecked(c);
    return {};
}

auto StringBuilder::ensure_capacity(usize capacity) {
    must$(try_ensure_capacity(capacity));
}

auto StringBuilder::try_ensure_capacity(usize capacity) -> ErrorOr<void> {
    return m_char_vector.try_ensure_capacity(capacity);
}

auto StringBuilder::to_string() const -> String {
    return must$(try_to_string());
}

auto StringBuilder::try_to_string() const -> ErrorOr<String> {
    return String::try_new_from_view(as_string_view());
}

auto StringBuilder::len() const -> usize {
    return m_char_vector.count();
}

auto StringBuilder::is_empty() const -> bool {
    return len() == 0;
}

auto StringBuilder::as_string_view() const -> StringView {
    return StringView::new_from_raw_parts(m_char_vector.raw_data(), len());
}
