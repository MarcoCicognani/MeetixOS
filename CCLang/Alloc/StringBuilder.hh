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

class StringBuilder final {
    TCDenyCopy$(StringBuilder);

public:
    /**
     * @brief Non-Error safe factory functions
     */
    [[nodiscard]]
    static auto new_empty() -> StringBuilder;
    [[nodiscard]]
    static auto new_with_capacity(usize capacity) -> StringBuilder;
    [[nodiscard]]
    static auto new_from_other(StringBuilder const& rhs) -> StringBuilder;

    /**
     * @brief Error safe Factory functions
     */
    static auto try_new_with_capacity(usize capacity) -> ErrorOr<StringBuilder>;
    static auto try_new_from_other(StringBuilder const& rhs) -> ErrorOr<StringBuilder>;

    /**
     * @brief Move constructor and move assignment
     */
    StringBuilder(StringBuilder&& rhs)                    = default;
    auto operator=(StringBuilder&& rhs) -> StringBuilder& = default;

    ~StringBuilder() = default;

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
    auto append(StringView string_view) -> void;
    auto try_append(StringView string_view) -> ErrorOr<void>;

    /**
     * @brief Ensures that this StringBuilder could store at least <capacity>
     */
    auto ensure_capacity(usize capacity);
    auto try_ensure_capacity(usize capacity) -> ErrorOr<void>;

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
    Vector<char> m_char_vector{ Vector<char>::new_empty() };
};
