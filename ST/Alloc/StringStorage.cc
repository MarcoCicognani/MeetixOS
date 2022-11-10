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

#include <ST/Alloc/Details.hh>
#include <ST/Alloc/NonNullRef.hh>
#include <ST/Alloc/StringStorage.hh>
#include <ST/Lang/Cxx.hh>
#include <ST/Lang/StringView.hh>
#include <ST/Lang/Try.hh>

auto StringStorage::try_new_from_view(StringView string_view) -> ErrorOr<NonNullRef<StringStorage>> {
    if ( string_view.is_null() )
        return Error::new_from_code(ErrorCode::Invalid);

    auto chars_storage_box = try$(Box<char[]>::try_new_from_len(string_view.len()));
    __builtin_memcpy(chars_storage_box.as_ptr(), string_view.as_cstr(), sizeof(char) * string_view.len());

    return NonNullRef<StringStorage>::try_new_from_emplace(Cxx::move(chars_storage_box), string_view.len());
}

auto StringStorage::storage_ptr() const -> char const* {
    return m_storage_box.as_ptr();
}

auto StringStorage::len() const -> usize {
    return m_char_count;
}

auto StringStorage::is_empty() const -> bool {
    return m_char_count == 0;
}

StringStorage::StringStorage(Box<char[]> storage_ptr, usize char_count)
    : m_storage_box{ Cxx::move(storage_ptr) }
    , m_char_count{ char_count } {
}
