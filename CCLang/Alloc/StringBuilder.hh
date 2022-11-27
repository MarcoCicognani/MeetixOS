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
    [[nodiscard]]
    static auto empty() -> StringBuilder;
    [[nodiscard]]
    static auto with_capacity(usize) -> StringBuilder;
    [[nodiscard]]
    static auto from_other(StringBuilder const&) -> StringBuilder;

    /**
     * @brief Error safe Factory functions
     */
    static auto try_with_capacity(usize) -> ErrorOr<StringBuilder>;
    static auto try_from_other(StringBuilder const&) -> ErrorOr<StringBuilder>;

    /**
     * @brief Move constructor and move assignment
     */
    StringBuilder(StringBuilder&&);
    auto operator=(StringBuilder&&) -> StringBuilder&;

    ~StringBuilder() = default;

    /**
     * @brief Swaps in O(1) the content of this StringBuilder with another
     */
    auto swap(StringBuilder&) -> void;

    /**
     * @brief Deep cloning
     */
    [[nodiscard]]
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
    auto append(StringView) -> void;
    auto try_append(StringView) -> ErrorOr<void>;

    /**
     * @brief Ensures that this StringBuilder could store at least <capacity>
     */
    auto ensure_capacity(usize);
    auto try_ensure_capacity(usize) -> ErrorOr<void>;

    /**
     * @brief Converts this StringBuilder to a String WITHOUT calling clear
     */
    [[nodiscard]]
    auto to_string() const -> String;
    auto try_to_string() const -> ErrorOr<String>;

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto len() const -> usize;
    [[nodiscard]]
    auto is_empty() const -> bool;

    [[nodiscard]]
    auto as_string_view() const -> StringView;

private:
    explicit constexpr StringBuilder() = default;

private:
    Vector<char> m_char_vector = Vector<char>::empty();
};


namespace Cxx {

constexpr auto swap(StringBuilder& lhs, StringBuilder& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */

