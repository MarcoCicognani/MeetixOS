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

#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/NonNullRef.hh>
#include <LibTC/Memory/Shareable.hh>

namespace TC {
namespace Collection {

class StringView;

class StringStorage : public Shareable {
    TC_DENY_COPY(StringStorage);
    TC_DENY_MOVE(StringStorage);

public:
    /**
     * @brief Constructors
     */
    static NonNullRef<StringStorage>          from(StringView string_view);
    static ErrorOr<NonNullRef<StringStorage>> try_from(StringView string_view);

    ~StringStorage() override = default;

    /**
     * @brief Custom delete operator
     */
    void operator delete(void* raw_string_storage);

    /**
     * @brief Getters
     */
    [[nodiscard]] char const* data() const;
    [[nodiscard]] usize       count() const;

    [[nodiscard]] bool is_empty() const;

    /**
     * @brief Since the StringStorage uses inline storage the requested size to the heap allocator is the size of the
     * class plus the capacity for the string storage
     */
    static usize alloc_size(usize char_count);

private:
    explicit StringStorage(StringView string_view);

private:
    usize m_char_count{ 0 };
    char  m_inline_storage[0];
};

} /* namespace Collection */

using Collection::StringStorage;

} /* namespace TC */