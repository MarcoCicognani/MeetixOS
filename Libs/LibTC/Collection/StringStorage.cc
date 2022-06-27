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

#include <LibTC/BitCast.hh>
#include <LibTC/Collection/StringStorage.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Memory/NonNullRef.hh>
#include <LibTC/Memory/Raw.hh>

namespace TC::Collection {

NonNullRef<StringStorage> StringStorage::construct_from(StringView string_view) {
    return MUST(try_construct_from(string_view));
}

ErrorOr<NonNullRef<StringStorage>> StringStorage::try_construct_from(StringView string_view) {
    using RCStringStorage = Memory::Details::RefCounted<StringStorage>;

    if ( string_view.is_null() )
        return Error{ EINVAL };

    auto ref_counted_string_storage = TRY(Memory::Raw::clean_alloc_object<void>(alloc_size(string_view.len())));
    auto ref_counted_string_ptr     = new (ref_counted_string_storage) RCStringStorage{ FromArgs, string_view };

    return NonNullRef<StringStorage>{ Adopt, ref_counted_string_ptr };
}

void StringStorage::operator delete(void* raw_string_storage) {
    if ( raw_string_storage == nullptr ) [[unlikely]]
        return;

    auto string_storage = bit_cast<Memory::Details::RefCounted<StringStorage>*>(raw_string_storage);
    Memory::Raw::free_sized(string_storage, alloc_size(string_storage->shared_object().m_char_count));
}

char const* StringStorage::data_storage() const {
    return m_inline_storage;
}

usize StringStorage::char_count() const {
    return m_char_count;
}

bool StringStorage::is_empty() const {
    return m_char_count == 0;
}

usize StringStorage::alloc_size(usize char_count) {
    return sizeof(Memory::Details::RefCounted<StringStorage>) + sizeof(char) * char_count;
}

StringStorage::StringStorage(StringView string_view)
    : m_char_count{ string_view.len() } {
    __builtin_memcpy(m_inline_storage, string_view.as_cstr(), sizeof(char) * m_char_count);
}

} /* namespace TC::Collection */
