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
#include <LibTC/Functional/Try.hh>
#include <LibTC/Memory/NonNullRef.hh>
#include <LibTC/Memory/Raw.hh>

namespace TC::Collection {

using RCStringStorage = Memory::Details::RefCounted<StringStorage>;

auto StringStorage::try_construct_from_view(StringView string_view) -> ErrorOr<NonNullRef<StringStorage>> {
    if ( string_view.is_null() )
        return Error::construct_from_errno(EINVAL);

    auto ref_counted_string_storage = TRY(Memory::Raw::clean_alloc_object<void>(alloc_size(string_view.len())));
    auto ref_counted_string_ptr     = new (ref_counted_string_storage) RCStringStorage{ Memory::Details::FromArgsTag::FromArgs, string_view };

    return NonNullRef<StringStorage>::construct_from_adopt(*ref_counted_string_ptr);
}

StringStorage::StringStorage(StringView string_view)
    : m_char_count{ string_view.len() } {
    __builtin_memcpy(m_inline_storage, string_view.as_cstr(), sizeof(char) * m_char_count);
}

auto StringStorage::operator delete(void* raw_string_storage) -> void {
    if ( raw_string_storage == nullptr ) [[unlikely]]
        return;

    auto string_storage = bit_cast<RCStringStorage*>(raw_string_storage);
    Memory::Raw::free_sized(string_storage, alloc_size(string_storage->shared_object().m_char_count));
}

auto StringStorage::data_storage() const -> char const* {
    return m_inline_storage;
}

auto StringStorage::char_count() const -> usize {
    return m_char_count;
}

auto StringStorage::is_empty() const -> bool {
    return m_char_count == 0;
}

auto StringStorage::alloc_size(usize char_count) -> usize {
    return sizeof(RCStringStorage) + sizeof(char) * char_count;
}

} /* namespace TC::Collection */
