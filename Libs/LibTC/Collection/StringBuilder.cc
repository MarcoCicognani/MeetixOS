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

#include <LibTC/Collection/StringBuilder.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Try.hh>

namespace TC::Collection {

auto StringBuilder::construct_empty() -> StringBuilder {
    return StringBuilder{};
}

auto StringBuilder::construct_with_capacity(usize capacity) -> StringBuilder {
    return MUST(try_construct_with_capacity(capacity));
}

auto StringBuilder::construct_from_other(const StringBuilder& rhs) -> StringBuilder {
    return MUST(try_construct_from_other(rhs));
}

auto StringBuilder::try_construct_with_capacity(usize capacity) -> ErrorOr<StringBuilder> {
    auto string_builder = construct_empty();
    TRY(string_builder.try_ensure_capacity(capacity));
    return string_builder;
}

auto StringBuilder::try_construct_from_other(const StringBuilder& rhs) -> ErrorOr<StringBuilder> {
    auto string_builder = TRY(try_construct_with_capacity(rhs.len()));
    for ( auto const& c : rhs.as_string_view() )
        TRY(string_builder.try_append(c));

    return string_builder;
}

auto StringBuilder::clone() const -> StringBuilder {
    return MUST(try_clone());
}

auto StringBuilder::try_clone() const -> ErrorOr<StringBuilder> {
    return StringBuilder::try_construct_from_other(*this);
}

auto StringBuilder::clear(KeepStorageCapacity keep_storage_capacity) {
    m_char_vector.clear(keep_storage_capacity);
}

auto StringBuilder::append(char c) {
    MUST(try_append(c));
}

auto StringBuilder::try_append(char c) -> ErrorOr<void> {
    return m_char_vector.try_append(c);
}

auto StringBuilder::append(StringView string_view) {
    MUST(try_append(string_view));
}

auto StringBuilder::try_append(StringView string_view) -> ErrorOr<void> {
    if ( string_view.is_null_or_empty() )
        return {};

    /* first ensure the right capacity then append unchecked for speed */
    TRY(try_ensure_capacity(len() + string_view.len()));
    for ( auto c : string_view )
        m_char_vector.append_unchecked(c);
    return {};
}

[[maybe_unused]] auto StringBuilder::ensure_capacity(usize capacity) {
    MUST(try_ensure_capacity(capacity));
}

auto StringBuilder::try_ensure_capacity(usize capacity) -> ErrorOr<void> {
    return m_char_vector.try_ensure_capacity(capacity);
}

auto StringBuilder::to_string() const -> String {
    return MUST(try_to_string());
}

auto StringBuilder::try_to_string() const -> ErrorOr<String> {
    return String::try_construct_from_view(as_string_view());
}

auto StringBuilder::len() const -> usize {
    return m_char_vector.count();
}

auto StringBuilder::is_empty() const -> bool {
    return len() == 0;
}

auto StringBuilder::as_string_view() const -> StringView {
    return StringView{ m_char_vector.raw_data(), len() };
}

constexpr StringBuilder::StringBuilder() noexcept
    : m_char_vector{ Vector<char>::construct_empty() } {
}

} /* namespace TC::Collection */