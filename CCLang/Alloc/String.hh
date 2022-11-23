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

#include <CCLang/Alloc/NonNullRef.hh>
#include <CCLang/Alloc/StringStorage.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Core/Hashing.hh>
#include <CCLang/Core/TypeTraits.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/ReverseIteratorSupport.hh>
#include <CCLang/Lang/StringView.hh>

class String final : public DenyCopy {
public:
    using ConstIterator               = StringView::ConstIterator;
    using ConstReverseIterator        = StringView::ConstReverseIterator;
    using ConstReverseIteratorWrapper = StringView::ConstReverseIteratorWrapper;

    using CaseSensible = StringView::CaseSensible;
    using KeepEmpty    = StringView::KeepEmpty;
    using IntBase      = StringView::IntBase;
    using ParseMode    = StringView::ParseMode;
    using TrimMode     = StringView::TrimMode;

public:
    /**
     * @brief Non-Error safe factory functions
     */
    [[nodiscard]]
    static auto new_empty() -> String;
    [[nodiscard]]
    static auto new_from_other(String const&) -> String;
    [[nodiscard]]
    static auto new_from_view(StringView) -> String;

    /**
     * @brief Error safe Factory functions
     */
    static auto try_new_empty() -> ErrorOr<String>;
    static auto try_new_from_other(String const&) -> ErrorOr<String>;
    static auto try_new_from_view(StringView) -> ErrorOr<String>;

    /**
     * @brief Move constructor and move assignment
     */
    String(String&&);
    auto operator=(String&&) -> String&;

    ~String() = default;

    /**
     * @brief Cloning only increments strong references to the memory
     */
    [[nodiscard]]
    auto clone() const -> String;

    /**
     * @brief Swaps this string with another
     */
    auto swap(String&) -> void;

    /**
     * @brief Access operators
     */
    [[nodiscard]]
    auto at(usize index) const -> char const&;
    [[nodiscard]]
    auto operator[](usize index) const -> char const&;

    /**
     * @brief Compares this string with another
     */
    [[nodiscard]]
    auto compare(StringView) const -> Order;
    [[nodiscard]]
    auto equals_ignore_case(StringView) const -> bool;

    /**
     * @brief Returns a sub_string_view of this string
     */
    [[nodiscard]]
    auto sub_string(usize start) const -> String;
    [[nodiscard]]
    auto sub_string(usize start, usize count) const -> String;

    auto try_sub_string(usize start) const -> ErrorOr<String>;
    auto try_sub_string(usize start, usize count) const -> ErrorOr<String>;

    [[nodiscard]]
    auto sub_string_view(usize start) const -> StringView;
    [[nodiscard]]
    auto sub_string_view(usize start, usize count) const -> StringView;

    /**
     * @brief Returns a sub-StringView of this trimmed out of chars
     */
    [[nodiscard]]
    auto trim(StringView chars, TrimMode = TrimMode::Both) const -> String;
    auto try_trim(StringView chars, TrimMode = TrimMode::Both) const -> ErrorOr<String>;

    [[nodiscard]]
    auto trim_whitespaces(TrimMode = TrimMode::Both) const -> String;
    auto try_trim_whitespaces(TrimMode = TrimMode::Both) const -> ErrorOr<String>;

    [[nodiscard]]
    auto trim_view(StringView chars, TrimMode = TrimMode::Both) const -> StringView;
    [[nodiscard]]
    auto trim_whitespaces_view(TrimMode = TrimMode::Both) const -> StringView;

    /**
     * @brief Returns whether the given rhs is at the start of this string
     */
    [[nodiscard]]
    auto starts_with(StringView rhs, CaseSensible = CaseSensible::Yes) const -> bool;
    [[nodiscard]]
    auto starts_with(char rhs, CaseSensible = CaseSensible::Yes) const -> bool;

    [[nodiscard]]
    auto ends_with(StringView rhs, CaseSensible = CaseSensible::Yes) const -> bool;
    [[nodiscard]]
    auto ends_with(char rhs, CaseSensible = CaseSensible::Yes) const -> bool;

    /**
     * @brief Converts this String into an integer
     */
    template<typename T = i32>
    auto as_int(IntBase = IntBase::Decimal, ParseMode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    template<typename T = u32>
    auto as_uint(IntBase = IntBase::Decimal, ParseMode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    /**
     * @brief Returns the index of the given needle into the src StringView
     */
    auto find(char, usize start = 0) const -> Option<usize>;
    auto find(StringView, usize start = 0) const -> Option<usize>;

    /**
     * @brief Returns the last index of the given needle into the src StringView
     */
    auto find_last(char) const -> Option<usize>;

    /**
     * @brief Returns all the indexes of needle inside the src StringView
     */
    [[nodiscard]]
    auto find_all(StringView) const -> Vector<usize>;
    auto try_find_all(StringView) const -> ErrorOr<Vector<usize>>;

    /**
     * @brief Returns a new string with the lowercase of this one
     */
    [[nodiscard]]
    auto to_lowercase() const -> String;
    auto try_to_lowercase() const -> ErrorOr<String>;

    /**
     * @brief Returns a new string with the lowercase of this one
     */
    [[nodiscard]]
    auto to_uppercase() const -> String;
    auto try_to_uppercase() const -> ErrorOr<String>;

    /**
     * @brief Returns a new string with the reverse of this one
     */
    [[nodiscard]]
    auto to_reverse() const -> String;
    auto try_to_reverse() const -> ErrorOr<String>;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]]
    auto operator==(String const&) const -> bool;
    [[nodiscard]]
    auto operator==(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator!=(String const&) const -> bool;
    [[nodiscard]]
    auto operator!=(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator<(String const&) const -> bool;
    [[nodiscard]]
    auto operator<(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator<=(String const&) const -> bool;
    [[nodiscard]]
    auto operator<=(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator>(String const&) const -> bool;
    [[nodiscard]]
    auto operator>(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator>=(String const&) const -> bool;
    [[nodiscard]]
    auto operator>=(StringView const&) const -> bool;

    /**
     * @brief Returns whether the given rhs is contained into this String
     */
    [[nodiscard]]
    auto contains(StringView, CaseSensible = CaseSensible::Yes) const -> bool;
    [[nodiscard]]
    auto contains(char, CaseSensible = CaseSensible::Yes) const -> bool;

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
    [[nodiscard]]
    auto as_cstr() const -> char const*;
    [[nodiscard]]
    auto len() const -> usize;
    [[nodiscard]]
    auto count() const -> usize;

    [[nodiscard]]
    auto is_empty() const -> bool;

    [[nodiscard]]
    auto as_string_view() const -> StringView;

private:
    explicit String(NonNullRef<StringStorage>);

private:
    NonNullRef<StringStorage> m_string_storage_ref;
};

template<>
struct TypeTraits<String> final : public Details::TypeTraits<String> {
    static auto hash(String const& value) -> usize {
        return Hashing::hash_string(value.as_cstr(), value.len());
    }

    static constexpr auto is_trivial() -> bool {
        return false;
    }
};

namespace Cxx {

constexpr auto swap(String& lhs, String& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
