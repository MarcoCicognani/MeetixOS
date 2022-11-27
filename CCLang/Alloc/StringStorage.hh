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

#include <CCLang/Forward.hh>

#include <CCLang/Alloc/Box.hh>
#include <CCLang/Alloc/NonNullRef.hh>
#include <CCLang/Alloc/RefCounted.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/IntTypes.hh>

class StringStorage final : public DenyCopy, public DenyMove, public RefCounted {
public:
    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]]
    static auto try_from_view(StringView) -> ErrorOr<NonNullRef<StringStorage>>;

    ~StringStorage() = default;

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
    explicit StringStorage(Box<char[]>, StringView);

private:
    Box<char[]> m_storage_box;
    usize       m_char_count = 0;
};
