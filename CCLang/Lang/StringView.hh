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

#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/Hashing.hh>
#include <CCLang/Core/Order.hh>
#include <CCLang/Core/TypeTraits.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/ReverseIteratorSupport.hh>

#define as_string_view$(expression) #expression##sv

namespace Details {

template<typename TStringView, bool IsReverse>
class StringViewIterator final {
    using TIndex = ::Conditional<IsReverse, isize, usize>;

public:
    /**
     * @brief Construction functions
     */
    [[nodiscard]]
    static auto new_from_begin(TStringView const* string_view) -> StringViewIterator {
        return StringViewIterator{ string_view, 0 };
    }
    [[nodiscard]]
    static auto new_from_end(TStringView const* string_view) -> StringViewIterator {
        return StringViewIterator{ string_view, string_view->len() };
    }

    [[nodiscard]]
    static auto new_from_rbegin(TStringView const* string_view) -> StringViewIterator {
        return StringViewIterator{ string_view, string_view->len().template as<isize>() - 1 };
    }
    [[nodiscard]]
    static auto new_from_rend(TStringView const* string_view) -> StringViewIterator {
        return StringViewIterator{ string_view, -1 };
    }

    StringViewIterator(StringViewIterator const&)                    = default;
    auto operator=(StringViewIterator const&) -> StringViewIterator& = default;

    /**
     * @brief Increment operators
     */
    auto operator++() -> StringViewIterator& {
        if constexpr ( IsReverse ) {
            --m_index;
        } else {
            ++m_index;
        }
        return *this;
    }
    auto operator++(int) -> StringViewIterator {
        StringViewIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    auto operator*() const -> char const& {
        verify_false$(is_end());
        if constexpr ( IsReverse ) {
            return m_string_view->at(static_cast<usize>(m_index));
        } else {
            return m_string_view->at(m_index);
        }
    }

    /**
     * @brief Pointer access operators
     */
    auto operator->() const -> char const* {
        return &operator*();
    }

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto is_end() const -> bool {
        if constexpr ( IsReverse ) {
            return m_index == new_from_rend(m_string_view).index();
        } else {
            return m_index == new_from_end(m_string_view).index();
        }
    }
    [[nodiscard]]
    auto index() const -> usize {
        return m_index;
    }

    /**
     * @brief Comparison operators
     */
    [[nodiscard]]
    auto operator==(StringViewIterator const& rhs) const -> bool {
        return m_index == rhs.m_index;
    }
    [[nodiscard]]
    auto operator!=(StringViewIterator const& rhs) const -> bool {
        return m_index != rhs.m_index;
    }
    [[nodiscard]]
    auto operator<(StringViewIterator const& rhs) const -> bool {
        return m_index < rhs.m_index;
    }
    [[nodiscard]]
    auto operator>(StringViewIterator const& rhs) const -> bool {
        return m_index > rhs.m_index;
    }
    [[nodiscard]]
    auto operator<=(StringViewIterator const& rhs) const -> bool {
        return m_index <= rhs.m_index;
    }
    [[nodiscard]]
    auto operator>=(StringViewIterator const& rhs) const -> bool {
        return m_index >= rhs.m_index;
    }

private:
    explicit constexpr StringViewIterator(TStringView const* string_view, TIndex index)
        : m_string_view{ string_view }
        , m_index{ index } {
    }

private:
    TStringView const* m_string_view;
    TIndex             m_index{ 0 };
};

} /* namespace Details */

class StringView final {
public:
    enum class CaseSensible : bool {
        Yes,
        No
    };

    enum class KeepEmpty : bool {
        Yes,
        No
    };

    enum class IntBase {
        Binary  = 2,
        Octal   = 8,
        Decimal = 10,
        Hex     = 16
    };

    enum class ParseMode {
        Begin,
        TrimWhitesAndBegin,
        BeginToEnd,
        TrimWhitesAndBeginToEnd,
    };

    enum class TrimMode {
        Left,
        Right,
        Both
    };

    using ConstIterator               = Details::StringViewIterator<StringView, false>;
    using ConstReverseIterator        = Details::StringViewIterator<StringView, true>;
    using ConstReverseIteratorWrapper = ReverseIteratorSupport::Wrapper<StringView const>;

public:
    /**
     * @brief Error safe factory function
     */
    [[nodiscard]]
    static auto new_from_cstr(char const*) -> StringView;
    [[nodiscard]]
    static constexpr auto new_from_raw_parts(char const* c_str, usize len) -> StringView {
        return StringView{ c_str, len };
    }

    /**
     * @brief Constructors
     */
    constexpr explicit(false) StringView() = default;
    constexpr explicit(false) StringView(StringView const& rhs)
        : StringView{ rhs.m_chars_ptr, rhs.m_chars_count } {
    }
    constexpr explicit(false) StringView(StringView&& rhs)
        : m_chars_ptr{ Cxx::exchange(rhs.m_chars_ptr, nullptr) }
        , m_chars_count{ Cxx::exchange(rhs.m_chars_count, 0) } {
    }
    ~StringView() = default;

    auto operator=(StringView const&) -> StringView&;
    auto operator=(StringView&&) -> StringView&;

    /**
     * @brief Swaps this string view with another
     */
    auto swap(StringView&) -> void;

    /**
     * @brief Access operators
     */
    [[nodiscard]]
    auto at(usize index) const -> char const&;
    [[nodiscard]]
    auto operator[](usize index) const -> char const&;

    /**
     * @brief Compares this string view with another
     */
    [[nodiscard]]
    auto compare(StringView) const -> Order;
    [[nodiscard]]
    auto equals_ignore_case(StringView) const -> bool;

    /**
     * @brief Returns a sub_string_view of this string view
     */
    [[nodiscard]]
    auto sub_string_view(usize start) const -> StringView;
    [[nodiscard]]
    auto sub_string_view(usize start, usize count) const -> StringView;

    /**
     * @brief Returns a sub-StringView of this trimmed out of chars
     */
    [[nodiscard]]
    auto trim(StringView chars, TrimMode = TrimMode::Both) const -> StringView;
    [[nodiscard]]
    auto trim_whitespaces(TrimMode = TrimMode::Both) const -> StringView;

    /**
     * @brief Returns whether the given rhs is at the start of this string view
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
     * @brief Converts this StringView into an integer
     */
    template<typename T = i32>
    auto as_int(IntBase = IntBase::Decimal, ParseMode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    template<typename T = u32>
    auto as_uint(IntBase = IntBase::Decimal, ParseMode = ParseMode::TrimWhitesAndBeginToEnd) const -> ErrorOr<T>;

    /**
     * @brief Returns the index of the given needle into the src StringView
     */
    auto find(char needle, usize start = 0) const -> Option<usize>;
    auto find(StringView needle, usize start = 0) const -> Option<usize>;

    /**
     * @brief Returns the last index of the given needle into the src StringView
     */
    auto find_last(char needle) const -> Option<usize>;
    auto find_last_if(Predicate<char> auto predicate) const -> Option<usize> {
        for ( auto const i : Range<usize>{ len() - 1, 0 }.reverse_iter() ) {
            if ( predicate(at(i)) )
                return i;
        }
        return {};
    }

    /**
     * @brief Returns all the indexes of needle inside the src StringView
     */
    [[nodiscard]]
    auto find_all(StringView) const -> Vector<usize>;
    auto try_find_all(StringView) const -> ErrorOr<Vector<usize>>;

    /**
     * @brief Splits this StringView and puts the split_view values into a vector or pass them to a predicate
     */
    [[nodiscard]]
    auto split_view(char separator, KeepEmpty = KeepEmpty::No) const -> Vector<StringView>;
    [[nodiscard]]
    auto split_view(StringView separator, KeepEmpty = KeepEmpty::No) const -> Vector<StringView>;

    auto try_split_view(char separator, KeepEmpty = KeepEmpty::No) const -> ErrorOr<Vector<StringView>>;
    auto try_split_view(StringView separator, KeepEmpty = KeepEmpty::No) const -> ErrorOr<Vector<StringView>>;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]]
    auto operator==(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator!=(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator<(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator<=(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator>(StringView const&) const -> bool;
    [[nodiscard]]
    auto operator>=(StringView const&) const -> bool;

    /**
     * @brief Returns whether the given rhs is contained into this StringView
     */
    [[nodiscard]]
    auto contains(StringView rhs, CaseSensible = CaseSensible::Yes) const -> bool;
    [[nodiscard]]
    auto contains(char rhs, CaseSensible = CaseSensible::Yes) const -> bool;

    /**
     * @brief For-each support
     */
    auto begin() const -> ConstIterator;
    auto end() const -> ConstIterator;

    /**
     * @brief Reverse for-each support
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
    auto is_empty() const -> bool;
    [[nodiscard]]
    auto is_null() const -> bool;
    [[nodiscard]]
    auto is_null_or_empty() const -> bool;

private:
    constexpr explicit StringView(char const* c_str, usize count)
        : m_chars_ptr{ c_str }
        , m_chars_count{ count } {
    }

private:
    char const* m_chars_ptr{ nullptr }; /* TODO char8_t */
    usize       m_chars_count{ 0 };
};

[[nodiscard]]
constexpr auto operator""sv(char const* c_str, __SIZE_TYPE__ len) -> StringView {
    return StringView::new_from_raw_parts(c_str, len);
}

template<>
struct TypeTraits<StringView> final : public Details::TypeTraits<StringView> {
    static auto hash(StringView const& value) -> usize {
        return Hashing::hash_string(value.as_cstr(), value.len());
    }

    static constexpr auto is_trivial() -> bool {
        return true; /* since this type is a read-only view of a slice the destructor does nothing */
    }
};

namespace Cxx {

constexpr auto swap(StringView& lhs, StringView& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx` */
