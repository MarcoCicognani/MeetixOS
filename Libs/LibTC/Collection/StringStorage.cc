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

#include <LibTC/Collection/StringStorage.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Memory/Raw.hh>

namespace TC::Collection {

NonNullRef<StringStorage> StringStorage::from(StringView string_view) {
    return MUST(try_from(string_view));
}

ErrorOr<NonNullRef<StringStorage>> StringStorage::try_from(StringView string_view) {
    if ( string_view.is_null() )
        return EINVAL;

    auto string_storage_size = alloc_size(string_view.len());
    auto string_storage_slot = TRY(Memory::Raw::clean_alloc_object<StringStorage>(string_storage_size));
    auto string_storage_ptr  = new (string_storage_slot) StringStorage{ string_view };

    return NonNullRef<StringStorage>{ NonNullRef<StringStorage>::Adopt, *string_storage_ptr };
}

void StringStorage::operator delete(void* raw_string_storage) {
    if ( raw_string_storage == nullptr ) [[unlikely]]
        return;

    auto string_storage = reinterpret_cast<StringStorage*>(raw_string_storage);
    Memory::Raw::free_sized(string_storage, alloc_size(string_storage->m_char_count));
}

char const* StringStorage::data() const {
    return m_inline_storage;
}

usize StringStorage::count() const {
    return m_char_count;
}

bool StringStorage::is_empty() const {
    return count() == 0;
}

usize StringStorage::alloc_size(usize char_count) {
    return sizeof(StringStorage) + sizeof(char) * (char_count + 1);
}

StringStorage::StringStorage(StringView string_view)
    : m_char_count{ string_view.len() } {
    __builtin_memcpy(m_inline_storage, string_view.as_cstr(), m_char_count);
}

} /* namespace TC::Collection */
