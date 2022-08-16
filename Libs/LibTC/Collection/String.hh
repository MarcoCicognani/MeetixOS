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

#include <LibTC/Collection/Enums/CaseSensitivity.hh>
#include <LibTC/Collection/Enums/TrimMode.hh>
#include <LibTC/Collection/ReverseIteratorSupport.hh>
#include <LibTC/Collection/StringStorage.hh>
#include <LibTC/Collection/StringView.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Hashing.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/NonNullRef.hh>
#include <LibTC/TypeTraits.hh>

namespace TC {
namespace Collection {

class String {
    TC_DENY_COPY(String);

public:
    using ConstIterator               = StringView::ConstIterator;
    using ConstReverseIterator        = StringView::ConstReverseIterator;
    using ConstReverseIteratorWrapper = StringView::ConstReverseIteratorWrapper;

public:
    /**
     * @brief Non-error safe Factory functions
     */
    [[nodiscard]] static auto construct_empty() -> String;
    [[nodiscard]] static auto construct_from_other(String const& rhs) -> String;
    [[nodiscard]] static auto construct_from_view(StringView string_view) -> String;

    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]] static auto try_construct_empty() -> ErrorOr<String>;
    [[nodiscard]] static auto try_construct_from_other(String const& rhs) -> ErrorOr<String>;
    [[nodiscard]] static auto try_construct_from_view(StringView string_view) -> ErrorOr<String>;

    /**
     * @brief Move constructor and move assignment
     */
    String(String&& rhs) noexcept;
    auto operator=(String&& rhs) noexcept -> String&;

    ~String() = default;

    /**
     * @brief Cloning only increments strong references to the memory
     */
    [[nodiscard]] auto clone() const -> String;

    /**
     * @brief Swaps this string with another
     */
    auto swap(String& rhs) noexcept -> void;

    /**
     * @brief Access operators
     */
    [[nodiscard]] auto at(usize index) const -> char const&;
    [[nodiscard]] auto operator[](usize index) const -> char const&;

    /**
     * @brief Compares this string with another
     */
    [[nodiscard]] auto compare(StringView rhs) const -> int;
    [[nodiscard]] auto equals_ignore_case(StringView rhs) const -> bool;

    /**
     * @brief Returns a sub_string_view of this string
     */
    [[nodiscard]] auto sub_string(usize start) const -> String;
    [[nodiscard]] auto sub_string(usize start, usize count) const -> String;

    [[nodiscard]] auto try_sub_string(usize start) const -> ErrorOr<String>;
    [[nodiscard]] auto try_sub_string(usize start, usize count) const -> ErrorOr<String>;

    [[nodiscard]] auto sub_string_view(usize start) const -> StringView;
    [[nodiscard]] auto sub_string_view(usize start, usize count) const -> StringView;

    /**
     * @brief Returns a sub-StringView of this trimmed out of chars
     */
    [[nodiscard]] auto trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const -> String;
    [[nodiscard]] auto try_trim(StringView chars, TrimMode trim_mode = TrimMode::Both) const -> ErrorOr<String>;

    [[nodiscard]] auto trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const -> String;
    [[nodiscard]] auto try_trim_whitespaces(TrimMode trim_mode = TrimMode::Both) const -> ErrorOr<String>;

    [[nodiscard]] auto trim_view(StringView chars, TrimMode trim_mode = TrimMode::Both) const -> StringView;
    [[nodiscard]] auto trim_whitespaces_view(TrimMode trim_mode = TrimMode::Both) const -> StringView;

    /**
     * @brief Returns whether the given rhs is at the start of this string
     */
    [[nodiscard]] auto starts_with(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;
    [[nodiscard]] auto starts_with(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;

    [[nodiscard]] auto ends_with(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;
    [[nodiscard]] auto ends_with(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;

    /**
     * @brief Converts this String into an integer
     */
    template<typename T = i32>
    [[nodiscard]] auto as_int(IntBase int_base = IntBase::Decimal, ParseMode parse_mode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    template<typename T = u32>
    [[nodiscard]] auto as_uint(IntBase int_base = IntBase::Decimal, ParseMode parse_mode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    /**
     * @brief Returns the index of the given needle into the src StringView
     */
    [[nodiscard]] auto find(char needle, size_t start = 0) const -> Option<usize>;
    [[nodiscard]] auto find(StringView needle, size_t start = 0) const -> Option<usize>;

    /**
     * @brief Returns the last index of the given needle into the src StringView
     */
    [[nodiscard]] auto find_last(char needle) const -> Option<usize>;

    /**
     * @brief Returns all the indexes of needle inside the src StringView
     */
    [[nodiscard]] auto find_all(StringView needle) const -> Vector<usize>;
    [[nodiscard]] auto try_find_all(StringView needle) const -> ErrorOr<Vector<usize>>;

    /**
     * @brief Returns a new string with the lowercase of this one
     */
    [[nodiscard]] auto to_lowercase() const -> String;
    [[nodiscard]] auto try_to_lowercase() const -> ErrorOr<String>;

    /**
     * @brief Returns a new string with the lowercase of this one
     */
    [[nodiscard]] auto to_uppercase() const -> String;
    [[nodiscard]] auto try_to_uppercase() const -> ErrorOr<String>;

    /**
     * @brief Returns a new string with the reverse of this one
     */
    [[nodiscard]] auto to_reverse() const -> String;
    [[nodiscard]] auto try_to_reverse() const -> ErrorOr<String>;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] auto operator==(String const& rhs) const -> bool;
    [[nodiscard]] auto operator==(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator!=(String const& rhs) const -> bool;
    [[nodiscard]] auto operator!=(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator<(String const& rhs) const -> bool;
    [[nodiscard]] auto operator<(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator<=(String const& rhs) const -> bool;
    [[nodiscard]] auto operator<=(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator>(String const& rhs) const -> bool;
    [[nodiscard]] auto operator>(StringView const& rhs) const -> bool;
    [[nodiscard]] auto operator>=(String const& rhs) const -> bool;
    [[nodiscard]] auto operator>=(StringView const& rhs) const -> bool;

    /**
     * @brief Returns whether the given rhs is contained into this String
     */
    [[nodiscard]] auto contains(StringView rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;
    [[nodiscard]] auto contains(char rhs, CaseSensitivity case_sensitivity = CaseSensitivity::Sensitive) const -> bool;

    /**
     * @brief For-each support
     */
    auto begin() const -> ConstIterator;
    auto end() const -> ConstIterator;

    /**
     * @brief For-each support
     */
    auto rbegin() const -> ConstReverseIterator;
    auto rend() const -> ConstReverseIterator;
    auto reverse_iter() const -> ConstReverseIteratorWrapper;

    /**
     * @brief Getters
     */
    [[nodiscard]] auto as_cstr() const -> char const*;
    [[nodiscard]] auto len() const -> usize;
    [[nodiscard]] auto count() const -> usize;

    [[nodiscard]] auto is_empty() const -> bool;

    [[nodiscard]] auto as_string_view() const -> StringView;

private:
    explicit String(NonNullRef<StringStorage> string_storage);

private:
    NonNullRef<StringStorage> m_string_storage_ref;
};

} /* namespace Collection */

using Collection::String;

template<>
struct TypeTraits<String> : public Details::TypeTraits<String> {
    static auto hash(String const& value) -> usize {
        return Hashing::string_calculate_hash(value.as_cstr(), value.len());
    }

    static constexpr auto is_trivial() -> bool {
        return false;
    }
};

} /* namespace TC */
