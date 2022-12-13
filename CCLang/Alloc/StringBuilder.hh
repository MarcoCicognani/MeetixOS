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

#include <CCLang/Alloc/String.hh>
#include <CCLang/Alloc/Vector.hh>
#include <CCLang/Core/CharTypes.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/DenyMove.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/StringView.hh>

class StringBuilder final : public DenyCopy {
public:
    /**
     * @brief Non-Error safe factory functions
     */
    static auto empty() -> StringBuilder;
    static auto with_capacity(usize capacity) -> StringBuilder;
    static auto from_other(StringBuilder const& rhs) -> StringBuilder;

    /**
     * @brief Error safe Factory functions
     */
    static auto try_with_capacity(usize capacity) -> ErrorOr<StringBuilder>;
    static auto try_from_other(StringBuilder const& rhs) -> ErrorOr<StringBuilder>;

    /**
     * @brief Move constructor and move assignment
     */
    StringBuilder(StringBuilder&& rhs);
    auto operator=(StringBuilder&& rhs) -> StringBuilder&;

    ~StringBuilder() = default;

    /**
     * @brief Swaps in O(1) the content of this StringBuilder with another
     */
    auto swap(StringBuilder& rhs) -> void;

    /**
     * @brief Deep cloning
     */
    auto clone() const -> StringBuilder;
    auto try_clone() const -> ErrorOr<StringBuilder>;

    /**
     * @brief Clear this StringBuilder
     */
    auto clear() -> void;
    auto clear_keep_capacity() -> void;

    /**
     * @brief Appends a character to the builder
     */
    auto append(char c) -> void;
    auto try_append(char c) -> ErrorOr<void>;

    /**
     * @brief Appends a string to the builder
     */
    auto append(StringView string_view) -> void;
    auto try_append(StringView string_view) -> ErrorOr<void>;

    /**
     * @brief Appends a rune to the builder
     */
    auto append(u32 rune) -> void;
    auto try_append(u32 rune) -> ErrorOr<void>;

    /**
     * @brief Ensures that this StringBuilder could store at least <capacity>
     */
    auto ensure_capacity(usize capacity);
    auto try_ensure_capacity(usize capacity) -> ErrorOr<void>;

    /**
     * @brief Converts this StringBuilder to a String WITHOUT calling clear
     */
    auto to_string() const -> String;
    auto try_to_string() const -> ErrorOr<String>;

    /**
     * @brief Getters
     */
    auto len() const -> usize;
    auto is_empty() const -> bool;

    auto as_string_view() const -> StringView;

private:
    explicit constexpr StringBuilder() = default;

private:
    Vector<char> m_char_vector = Vector<char>::empty();
};


namespace Cxx {

auto swap(StringBuilder& lhs, StringBuilder& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */

