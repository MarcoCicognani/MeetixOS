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

void StringBuilder::clear(KeepStorageCapacity keep_storage_capacity) {
    m_char_vector.clear(keep_storage_capacity);
}

void StringBuilder::append(char c) {
    MUST(try_append(c));
}

ErrorOr<void> StringBuilder::try_append(char c) {
    return m_char_vector.try_append(c);
}

void StringBuilder::append(StringView string_view) {
    MUST(try_append(string_view));
}

ErrorOr<void> StringBuilder::try_append(StringView string_view) {
    if ( string_view.is_null_or_empty() )
        return {};

    /* first ensure the right capacity then append unchecked for speed */
    TRY(m_char_vector.try_ensure_capacity(len() + string_view.len()));
    for ( auto c : string_view )
        m_char_vector.append_unchecked(c);
    return {};
}

String StringBuilder::to_string() const {
    return MUST(try_to_string());
}

ErrorOr<String> StringBuilder::try_to_string() const {
    return String::try_construct_from(as_string_view());
}

usize StringBuilder::len() const {
    return m_char_vector.count();
}

bool StringBuilder::is_empty() const {
    return len() == 0;
}

StringView StringBuilder::as_string_view() const {
    return StringView{ m_char_vector.data(), len() };
}

} /* namespace TC::Collection */