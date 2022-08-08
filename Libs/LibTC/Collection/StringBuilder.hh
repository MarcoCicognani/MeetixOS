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

#include <LibTC/Collection/Enums/KeepStorageCapacity.hh>
#include <LibTC/Collection/String.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/Collection/Vector.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/DenyMove.hh>
#include <LibTC/Forward.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Collection {

class StringBuilder {
    TC_DENY_COPY(StringBuilder);

public:
    /**
     * @brief Non-error safe Factory functions
     */
    static auto construct_empty() -> StringBuilder;
    static auto construct_with_capacity(usize capacity) -> StringBuilder;
    static auto construct_from_other(StringBuilder const& rhs) -> StringBuilder;

    /**
     * @brief Error safe Factory functions
     */
    static auto try_construct_with_capacity(usize capacity) -> ErrorOr<StringBuilder>;
    static auto try_construct_from_other(StringBuilder const& rhs) -> ErrorOr<StringBuilder>;

    /**
     * @brief Move constructor and move assignment
     */
    StringBuilder(StringBuilder&& rhs) noexcept                    = default;
    auto operator=(StringBuilder&& rhs) noexcept -> StringBuilder& = default;

    ~StringBuilder() = default;

    /**
     * @brief Deep cloning
     */
    [[nodiscard]] auto clone() const -> StringBuilder;
    [[nodiscard]] auto try_clone() const -> ErrorOr<StringBuilder>;

    /**
     * @brief Clear this StringBuilder
     */
    auto clear(KeepStorageCapacity keep_storage_capacity = KeepStorageCapacity::Yes);

    /**
     * @brief Appends a character to the builder
     */
    auto append(char c);
    auto try_append(char c) -> ErrorOr<void>;

    /**
     * @brief Appends a string to the builder
     */
    auto append(StringView string_view);
    auto try_append(StringView string_view) -> ErrorOr<void>;

    /**
     * @brief Ensures that this StringBuilder could store at least <capacity>
     */
    [[maybe_unused]] auto ensure_capacity(usize capacity);
    auto                  try_ensure_capacity(usize capacity) -> ErrorOr<void>;

    /**
     * @brief Converts this StringBuilder to a String WITHOUT calling clear
     */
    [[nodiscard]] auto to_string() const -> String;
    [[nodiscard]] auto try_to_string() const -> ErrorOr<String>;

    /**
     * @brief Getters
     */
    [[nodiscard]] auto len() const -> usize;
    [[nodiscard]] auto is_empty() const -> bool;

    [[nodiscard]] auto as_string_view() const -> StringView;

private:
    explicit constexpr StringBuilder() noexcept;

private:
    Vector<char> m_char_vector;
};

} /* namespace Collection */

using Collection::StringBuilder;

} /* namespace TC */