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

#include <LibTC/Alloc/Details.hh>
#include <LibTC/Alloc/NonNullRef.hh>
#include <LibTC/Alloc/StringStorage.hh>
#include <LibTC/Lang/Cxx.hh>
#include <LibTC/Lang/StringView.hh>
#include <LibTC/Lang/Try.hh>

auto StringStorage::try_new_from_view(StringView string_view) -> ErrorOr<NonNullRef<StringStorage>> {
    if ( string_view.is_null() )
        return Error::new_from_code(EINVAL);

    auto chars_storage_ptr = try$(Details::__heap_plug_alloc_mem<char>(AllocLayout::new_for_array_of<char>(string_view.len())));
    __builtin_memcpy(chars_storage_ptr, string_view.as_cstr(), sizeof(char) * string_view.len());

    return NonNullRef<StringStorage>::try_new_from_emplace(chars_storage_ptr, string_view.len());
}

StringStorage::~StringStorage() {
    delete m_storage_ptr;
}

auto StringStorage::storage_ptr() const -> char const* {
    return m_storage_ptr;
}

auto StringStorage::len() const -> usize {
    return m_char_count;
}

auto StringStorage::is_empty() const -> bool {
    return m_char_count == 0;
}
