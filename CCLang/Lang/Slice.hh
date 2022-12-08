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

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Option.hh>
#include <CCLang/Lang/ReverseIteratorSupport.hh>

namespace Details {

template<typename TSlice, typename T, bool IsReverse>
class SliceIterator final {
    using TIndex = ::Conditional<IsReverse, isize, usize>;

public:
    /**
     * @brief Construction functions
     */
    [[nodiscard]]
    static auto from_begin(TSlice& slice) -> SliceIterator {
        return SliceIterator(slice, 0);
    }
    [[nodiscard]]
    static auto from_end(TSlice& slice) -> SliceIterator {
        return SliceIterator(slice, slice.len());
    }

    [[nodiscard]]
    static auto from_rbegin(TSlice& slice) -> SliceIterator {
        return SliceIterator(slice, slice.len().template as<TIndex>() - 1);
    }
    [[nodiscard]]
    static auto from_rend(TSlice& slice) -> SliceIterator {
        return SliceIterator(slice, -1);
    }

    SliceIterator(SliceIterator const&)                    = default;
    auto operator=(SliceIterator const&) -> SliceIterator& = default;

    /**
     * @brief Increment operators
     */
    auto operator++() -> SliceIterator& {
        if ( IsReverse ) {
            --m_index;
        } else {
            ++m_index;
        }
        return *this;
    }
    auto operator++(int) -> SliceIterator {
        auto __prev = *this;
             operator++();
        return __prev;
    }

    /**
     * @brief ValueReference access operators
     */
    auto operator*() -> T& {
        verify_false_with_msg$(is_end(), "Tried to deref a empty SliceIterator");

        if ( IsReverse ) {
            return m_slice[m_index.template as<usize>()];
        } else {
            return m_slice[m_index];
        }
    }
    auto operator*() const -> T const& {
        verify_false_with_msg$(is_end(), "Tried to deref a empty SliceIterator");

        if ( IsReverse ) {
            return m_slice[m_index.template as<usize>()];
        } else {
            return m_slice[m_index];
        }
    }

    /**
     * @brief Pointer access operators
     */
    auto operator->() -> T* {
        return &operator*();
    }
    auto operator->() const -> char const* {
        return &operator*();
    }

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto is_end() const -> bool {
        if ( IsReverse ) {
            return m_index.template as<usize>() == from_rend(m_slice).index();
        } else {
            return m_index.template as<usize>() == from_end(m_slice).index();
        }
    }
    [[nodiscard]]
    auto index() const -> usize {
        return m_index.template as<usize>();
    }

    /**
     * @brief Comparison operators
     */
    [[nodiscard]]
    auto operator==(SliceIterator<TSlice, T, IsReverse> const& rhs) const -> bool {
        return m_index == rhs.m_index;
    }
    [[nodiscard]]
    auto operator!=(SliceIterator<TSlice, T, IsReverse> const& rhs) const -> bool {
        return m_index != rhs.m_index;
    }
    [[nodiscard]]
    auto operator<(SliceIterator<TSlice, T, IsReverse> const& rhs) const -> bool {
        return m_index < rhs.m_index;
    }
    [[nodiscard]]
    auto operator>(SliceIterator<TSlice, T, IsReverse> const& rhs) const -> bool {
        return m_index > rhs.m_index;
    }
    [[nodiscard]]
    auto operator<=(SliceIterator<TSlice, T, IsReverse> const& rhs) const -> bool {
        return m_index <= rhs.m_index;
    }
    [[nodiscard]]
    auto operator>=(SliceIterator<TSlice, T, IsReverse> const& rhs) const -> bool {
        return m_index >= rhs.m_index;
    }

private:
    explicit SliceIterator(TSlice& slice, TIndex start_index)
        : m_slice(slice)
        , m_index(start_index) {
    }

private:
    TSlice& m_slice;
    TIndex  m_index;
};

} /* namespace Details */

template<typename T>
class Slice {
public:
    using Iterator             = Details::SliceIterator<Slice<T>, T, false>;
    using ReverseIterator      = Details::SliceIterator<Slice<T>, T, true>;
    using ConstIterator        = Details::SliceIterator<Slice<T> const, T const, false>;
    using ConstReverseIterator = Details::SliceIterator<Slice<T> const, T const, true>;

    using ReverseIteratorWrapper      = ReverseIteratorSupport::Wrapper<Slice<T>>;
    using ConstReverseIteratorWrapper = ReverseIteratorSupport::Wrapper<Slice<T> const>;

public:
    [[nodiscard]]
    static auto empty() -> Slice<T> {
        return Slice<T>();
    }
    [[nodiscard]]
    static auto from_raw_parts(T* raw_array_ptr, usize raw_array_len) -> Slice<T> {
        return Slice<T>(raw_array_ptr, raw_array_len);
    }

    Slice(Slice<T> const&) = default;
    Slice(Slice<T>&&)      = default;

    /**
     * @brief Swap support
     */
    auto swap(Slice<T>& rhs) -> void {
        Cxx::swap(m_array_ptr, rhs.m_array_ptr);
        Cxx::swap(m_array_len, rhs.m_array_len);
    }

    [[nodiscard]]
    auto at(usize index) -> Option<T&> {
        if ( index < m_array_len ) {
            return m_array_ptr[index.unwrap()];
        } else {
            return OptionNone;
        }
    }
    [[nodiscard]]
    auto at(usize index) const -> Option<T const&> {
        if ( index < m_array_len ) {
            return m_array_ptr[index.unwrap()];
        } else {
            return OptionNone;
        }
    }

    [[nodiscard]]
    auto operator[](usize index) -> T& {
        if ( index < m_array_len ) {
            return m_array_ptr[index.unwrap()];
        } else {
            panic("Slice: Index out of bounds in operator[]");
        }
    }
    [[nodiscard]]
    auto operator[](usize index) const -> T const& {
        if ( index < m_array_len ) {
            return m_array_ptr[index.unwrap()];
        } else {
            panic("Slice: Index out of bounds in operator[]");
        }
    }

    [[nodiscard]]
    auto first() -> Option<T&> {
        return at(0);
    }
    [[nodiscard]]
    auto first() const -> Option<T const&> {
        return at(0);
    }

    [[nodiscard]]
    auto last() -> Option<T&> {
        return at(len() - 1);
    }
    [[nodiscard]]
    auto last() const -> Option<T const&> {
        return at(len() - 1);
    }

    auto fill(T default_value) -> void {
        for ( auto const i : usize::range(0, len()) ) {
            m_array_ptr[i.unwrap()] = default_value;
        }
    }

    /**
     * @brief For-each support
     */
    auto begin() -> Iterator {
        return Iterator::from_begin(*this);
    }
    auto end() -> Iterator {
        return Iterator::from_end(*this);
    }

    auto begin() const -> ConstIterator {
        return ConstIterator::from_begin(*this);
    }
    auto end() const -> ConstIterator {
        return ConstIterator::from_end(*this);
    }

    /**
     * @brief Reverse for-each support
     */
    auto rbegin() -> ReverseIterator {
        return ReverseIterator::from_rbegin(*this);
    }
    auto rend() -> ReverseIterator {
        return ReverseIterator::from_rend(*this);
    }

    auto rbegin() const -> ConstReverseIterator {
        return ConstReverseIterator::from_rbegin(*this);
    }
    auto rend() const -> ConstReverseIterator {
        return ConstReverseIterator::from_rend(*this);
    }

    auto reverse_iter() -> ReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }
    auto reverse_iter() const -> ConstReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }

    [[nodiscard]]
    auto sub_slice(usize start) const -> Slice<T> {
        verify_less_with_msg$(start, m_array_len, "Slice: Index out of bounds in sub_slice()");
        return Slice<T>::from_raw_parts(m_array_ptr + start, len() - start);
    }
    [[nodiscard]]
    auto sub_slice(usize start, usize count) const -> Slice<T> {
        verify_less_with_msg$(start + count, m_array_len, "Slice: Index out of bounds in sub_slice()");
        return Slice<T>::from_raw_parts(m_array_ptr + start, count);
    }

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto len() const -> usize {
        return m_array_len;
    }
    [[nodiscard]]
    auto is_null() const -> bool {
        return m_array_ptr == nullptr;
    }
    [[nodiscard]]
    auto is_empty() const -> bool {
        return m_array_len == 0;
    }
    [[nodiscard]]
    auto is_null_or_empty() const -> bool {
        return is_null() || is_empty();
    }

    [[nodiscard]]
    auto data() -> T* {
        return m_array_ptr;
    }
    [[nodiscard]]
    auto data() const -> T const* {
        return m_array_ptr;
    }

private:
    explicit Slice() = default;
    explicit Slice(T* raw_array_ptr, usize raw_array_len)
        : m_array_ptr(raw_array_ptr)
        , m_array_len(raw_array_len) {
    }

private:
    T*    m_array_ptr = nullptr;
    usize m_array_len = 0;
};

namespace Cxx {

template<typename T>
auto swap(Slice<T>& lhs, Slice<T>& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
