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

#include <CCLang/Alloc/StringStorage.hh>

#include <CCLang/Alloc/New.hh>
#include <CCLang/Alloc/NonNullRef.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/StringView.hh>
#include <CCLang/Lang/Try.hh>

auto StringStorage::try_from_view(StringView string_view) -> ErrorOr<NonNullRef<StringStorage>> {
    if ( string_view.is_null() )
        return Error::from_code(ErrorCode::EmptyData);

    auto storage_box = try$(Box<char[]>::try_from_len(string_view.len()));
    return NonNullRef<StringStorage>::try_from_adopt(new (nothrow) StringStorage(Cxx::move(storage_box), string_view));
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

StringStorage::StringStorage(Box<char[]> storage_box, StringView string_view)
    : m_storage_box(Cxx::move(storage_box))
    , m_char_count(string_view.len()) {
    Cxx::memcpy(m_storage_box.as_ptr(), string_view.as_cstr(), string_view.len() * sizeof(char));
}
