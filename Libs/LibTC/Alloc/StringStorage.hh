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

#include <LibTC/Alloc/NonNullRef.hh>
#include <LibTC/Core/ErrorOr.hh>
#include <LibTC/Lang/IntTypes.hh>

class StringStorage final {
    TCDenyCopy$(StringStorage);
    TCDenyMove$(StringStorage);
    TCNonNullRefConstructible$(StringStorage);

public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]]
    static auto try_new_from_view(StringView string_view) -> ErrorOr<NonNullRef<StringStorage>>;

    ~StringStorage();

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto storage_ptr() const -> char const*;
    [[nodiscard]]
    auto len() const -> usize;

    [[nodiscard]]
    auto is_empty() const -> bool;

private:
    explicit constexpr StringStorage(char const* storage_ptr, usize char_count)
        : m_storage_ptr{ storage_ptr }
        , m_char_count{ char_count } {
    }

private:
    char const* m_storage_ptr{ nullptr };
    usize       m_char_count{ 0 };
};
