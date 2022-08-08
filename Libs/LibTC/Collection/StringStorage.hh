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

#pragma once

#include <LibTC/Forward.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/NonNullRef.hh>

namespace TC {
namespace Collection {

class StringView;

class StringStorage {
    TC_DENY_COPY(StringStorage);
    TC_DENY_MOVE(StringStorage);

public:
    /**
     * @brief Error safe Factory functions
     */
    static auto try_construct_from_view(StringView string_view) -> ErrorOr<NonNullRef<StringStorage>>;

    explicit StringStorage(StringView string_view);
    ~StringStorage() = default;

    /**
     * @brief Custom delete operator
     */
    auto operator delete(void* raw_string_storage) -> void;

    /**
     * @brief Getters
     */
    [[nodiscard]] auto data_storage() const -> char const*;
    [[nodiscard]] auto char_count() const -> usize;

    [[nodiscard]] auto is_empty() const -> bool;

    /**
     * @brief Since the StringStorage uses inline storage the requested size to the heap allocator is the size of the
     * class plus the capacity for the string storage
     */
    static auto alloc_size(usize char_count) -> usize;

private:
    usize m_char_count{ 0 };
    char  m_inline_storage[0];
};

} /* namespace Collection */

using Collection::StringStorage;

} /* namespace TC */