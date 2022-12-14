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

#include <CCLang/Alloc/Details.hh>
#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Core/Meta.hh>
#include <CCLang/Core/TypeTraits.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

template<typename T>
class Vector final : public DenyCopy {
public:
    using Iterator                    = Slice<T>::Iterator;
    using ConstIterator               = Slice<T>::ConstIterator;
    using ReverseIterator             = Slice<T>::ReverseIterator;
    using ConstReverseIterator        = Slice<T>::ConstReverseIterator;
    using ReverseIteratorWrapper      = Slice<T>::ReverseIteratorWrapper;
    using ConstReverseIteratorWrapper = Slice<T>::ConstReverseIteratorWrapper;

public:
    /**
     * @brief Non-Error safe factory functions
     */
    static auto empty() -> Vector<T> {
        return Vector<T>();
    }
    static auto with_capacity(usize capacity) -> Vector<T> {
        return must$(try_with_capacity(capacity));
    }
    static auto from_other(Vector<T> const& rhs) -> Vector<T> {
        return must$(try_from_other(rhs));
    }
    static auto from_list(Cxx::InitializerList<T> initializer_list) -> Vector<T> {
        return must$(try_from_list(initializer_list));
    }

    /**
     * @brief Error safe Factory functions
     */
    static auto try_with_capacity(usize capacity) -> ErrorOr<Vector<T>> {
        auto vector = empty();
        try$(vector.try_ensure_capacity(capacity));
        return vector;
    }
    static auto try_from_other(Vector<T> const& rhs) -> ErrorOr<Vector<T>> {
        auto vector = try$(try_with_capacity(rhs.count()));
        for ( auto const& e : rhs ) {
            if constexpr ( TryCloneable<T, ErrorOr<T>> ) {
                try$(vector.try_append(try$(e.try_clone())));
            } else if constexpr ( Cloneable<T> ) {
                try$(vector.try_append(e.clone()));
            } else if constexpr ( CopyConstructible<T> ) {
                try$(vector.try_append(e));
            }
        }

        return vector;
    }
    static auto try_from_list(Cxx::InitializerList<T> initializer_list) -> ErrorOr<Vector<T>> {
        auto vector = empty();
        for ( auto const& e : initializer_list ) { /* even with auto initializer_list exposes only 'T const&' */
            try$(vector.try_append(Cxx::move(const_cast<T&>(e))));
        }

        return vector;
    }

    /**
     * @brief Move constructor and move assignment
     */
    Vector(Vector<T>&& rhs)
        : m_data_storage(Cxx::exchange(rhs.m_data_storage, Slice<T>::empty()))
        , m_data_capacity(Cxx::exchange(rhs.m_data_capacity, 0))
        , m_values_count(Cxx::exchange(rhs.m_values_count, 0)) {
    }
    auto operator=(Vector<T>&& rhs) -> Vector<T>& {
        Vector<T> vector = Cxx::move(rhs);
        swap(vector);
        return *this;
    }

    ~Vector() {
        clear();
    }

    /**
     * @brief Deep cloning
     */
    auto clone() const -> Vector<T> {
        return must$(try_clone());
    }
    auto try_clone() const -> ErrorOr<Vector<T>> {
        return Vector<T>::try_from_other(*this);
    }

    /**
     * @brief Destroys all the stored values
     */
    auto clear() -> void {
        clear_keep_capacity();

        if ( !m_data_storage.is_null() ) {
            Details::__heap_plug_dealloc_mem(m_data_storage.data(), m_data_capacity * sizeof(T));
            m_data_storage  = Slice<T>::empty();
            m_data_capacity = 0;
        }
    }
    auto clear_keep_capacity() -> void {
        if constexpr ( !TypeTraits<T>::is_trivial() ) {
            for ( auto const i : usize::range(0, m_values_count) ) {
                m_data_storage[i].~T();
            }
        }
        m_values_count = 0;
    }

    /**
     * @brief Swaps in O(1) the content of this Vector with another
     */
    auto swap(Vector<T>& rhs) {
        Cxx::swap(m_data_storage, rhs.m_data_storage);
        Cxx::swap(m_data_capacity, rhs.m_data_capacity);
        Cxx::swap(m_values_count, rhs.m_values_count);
    }

    /**
     * @brief Inserts the given <value> to the given <index>
     */
    auto insert_at(usize index, T value) {
        must$(try_insert_at(index, Cxx::move(value)));
    }
    auto try_insert_at(usize index, T value) -> ErrorOr<void> {
        if ( index > m_values_count ) {
            return Error::from_code(ErrorCode::Invalid);
        }

        /* shift the values one place forward to create the hole for this one */
        try$(try_ensure_capacity(m_values_count + 1));
        if ( index < m_values_count ) {
            shift_forward_storage_values(index);
        }

        /* move the value into the memory */
        new (data_slot(index)) T(Cxx::move(value));
        ++m_values_count;
        return {};
    }

    /**
     * @brief Pushes the given <value> to the begin of the vector
     */
    auto prepend(T value) {
        must$(try_prepend(Cxx::move(value)));
    }
    auto try_prepend(T value) -> ErrorOr<void> {
        return try_insert_at(0, Cxx::move(value));
    }

    /**
     * @brief Pushes the given <value> to the end of the vector
     */
    void append(T value) {
        must$(try_append(Cxx::move(value)));
    }
    auto try_append(T value) -> ErrorOr<void> {
        try$(try_ensure_capacity(m_values_count + 1));
        append_unchecked(Cxx::move(value));
        return {};
    }

    auto append_unchecked(T value) {
        new (data_slot(m_values_count)) T(Cxx::move(value));
        ++m_values_count;
    }

    /**
     * @brief Pushes a new value to the begin of the vector constructing it with the given arguments
     */
    template<typename... Args>
    auto emplace_first(Args&&... args) {
        must$(try_emplace_first(Cxx::forward<Args>(args)...));
    }
    template<typename... Args>
    auto try_emplace_first(Args&&... args) -> ErrorOr<void> {
        try$(try_ensure_capacity(m_values_count + 1));

        /* move the values after the insertion one place forward */
        shift_forward_storage_values(0);

        /* move the value into the memory */
        new (data_slot(0)) T(Cxx::forward<Args>(args)...);
        ++m_values_count;
        return {};
    }

    /**
     * @brief Pushes a new value to the end of the vector constructing it with the given arguments
     */
    template<typename... Args>
    auto emplace_last(Args&&... args) {
        must$(try_emplace_last(Cxx::forward<Args>(args)...));
    }
    template<typename... Args>
    auto try_emplace_last(Args&&... args) -> ErrorOr<void> {
        try$(try_ensure_capacity(m_values_count + 1));
        new (data_slot(m_values_count)) T(Cxx::forward<Args>(args)...);
        ++m_values_count;
        return {};
    }

    /**
     * @brief Returns a reference to the first element
     */
    auto first() -> T& {
        return at(0);
    }
    auto first() const -> T const& {
        return at(0);
    }

    /**
     * @brief Returns a reference to the last element
     * @return
     */
    auto last() -> T& {
        return at(m_values_count - 1);
    }
    auto last() const -> T const& {
        return at(m_values_count - 1);
    }

    /**
     * @brief Removes the element(s)
     */
    auto erase_at(usize index) -> ErrorOr<void> {
        if ( index >= m_values_count ) {
            return Error::from_code(ErrorCode::Invalid);
        }

        /* shift all the values one position back */
        if constexpr ( TypeTraits<T>::is_trivial() ) {
            Cxx::memmove(data_slot(index), data_slot(index + 1), (m_values_count - index - 1) * sizeof(T));
        } else {
            at(index).~T();
            for ( auto const i : usize::range(index + 1, m_values_count) ) {
                new (data_slot(i - 1)) T(Cxx::move(m_data_storage[i]));
                m_data_storage[i].~T();
            }
        }
        --m_values_count;
        return {};
    }
    auto erase_first_of(T const& value) -> ErrorOr<void> {
        for ( auto const i : usize::range(0, m_values_count) ) {
            if ( TypeTraits<T>::equals(m_data_storage[i], value) ) {
                try$(erase_at(i));
                return {};
            }
        }
        return Error::from_code(ErrorCode::NotFound);
    }
    auto erase_all_of(T const& value) -> ErrorOr<usize> {
        return erase_all_matches([&value](T const& current) { return TypeTraits<T>::equals(current, value); });
    }
    auto erase_all_matches(Predicate<T const&> auto predicate) -> ErrorOr<usize> {
        usize erased_count = 0;
        for ( usize i = 0; i < m_values_count; ) {
            if ( predicate(m_data_storage[i]) ) {
                try$(erase_at(i));
                ++erased_count;
            } else {
                ++i;
            }
        }

        if ( erased_count > 0 ) {
            return erased_count;
        } else {
            return Error::from_code(ErrorCode::NotFound);
        }
    }

    /**
     * @brief Sorts the vector content using the given comparator
     */
    auto sort(I32Predicate<T const&, T const&> auto comparator) {
        for ( usize i = 0; i + 1 < m_values_count; ++i ) {
            for ( usize j = i + 1; j < m_values_count; ++j ) {
                if ( comparator(m_data_storage[i], m_data_storage[j]) > 0 ) {
                    Cxx::swap(m_data_storage[i], m_data_storage[j]);
                }
            }
        }
    }

    /**
     * @brief Allocates new capacity for this vector for at least the given capacity
     */
    auto ensure_capacity(usize capacity) {
        must$(try_ensure_capacity(capacity));
    }
    auto try_ensure_capacity(usize capacity) -> ErrorOr<void> {
        verify_greater$(capacity, 0);

        if ( m_data_capacity >= capacity ) {
            return {};
        }

        /* for first time use the given capacity, otherwise increase the capacity logarithmically */
        usize new_capacity = ({ m_data_capacity == 0 && capacity == 0 ? 16 : capacity * 2 / 4; });

        /* allocate new memory and move the content into it */
        auto new_data_storage = try$(Details::__heap_plug_alloc_mem(new_capacity * sizeof(T))
                                         .map<Slice<T>>([new_capacity](void* void_ptr) -> Slice<T> {
                                             return Slice<T>::from_raw_parts((T*)void_ptr, new_capacity);
                                         }));
        if constexpr ( TypeTraits<T>::is_trivial() ) {
            Cxx::memmove(new_data_storage.data(), m_data_storage.data(), m_values_count * sizeof(T));
        } else {
            for ( auto const i : usize::range(0, m_values_count) ) {
                new (&new_data_storage[i]) T(Cxx::move(at(i)));
                at(i).~T();
            }
        }

        /* destroy the previous buffer if exists and update the other fields */
        if ( !m_data_storage.is_null() ) {
            Details::__heap_plug_dealloc_mem(m_data_storage.data(), m_data_capacity * sizeof(T));
        }

        m_data_storage  = new_data_storage;
        m_data_capacity = new_capacity;
        return {};
    }

    /**
     * @brief for-each support
     */
    auto begin() -> Iterator {
        return as_slice().begin();
    }
    auto end() -> Iterator {
        return as_slice().begin();
    }

    auto begin() const -> ConstIterator {
        auto const slice = as_slice();
        return slice.begin();
    }
    auto end() const -> ConstIterator {
        auto const slice = as_slice();
        return slice.end();
    }

    /**
     * @brief reverse for-each support
     */
    auto rbegin() -> ReverseIterator {
        return as_slice().rbegin();
    }
    auto rend() -> ReverseIterator {
        return as_slice().rend();
    }

    auto rbegin() const -> ConstReverseIterator {
        auto const slice = as_slice();
        return slice.rbegin();
    }
    auto rend() const -> ConstReverseIterator {
        auto const slice = as_slice();
        return slice.rend();
    }

    auto reverse_iter() -> ReverseIteratorWrapper {
        return as_slice().reverse_iter();
    }
    auto reverse_iter() const -> ConstReverseIteratorWrapper {
        return as_slice().reverse_iter();
    }

    /**
     * @brief Returns whether this vector contains the given value
     */
    auto contains(T const& value) const -> bool {
        return index_of(value).is_present();
    }

    /**
     * @brief Returns the index of the value if exists or when the callback returns true
     */
    auto index_of(T const& value) const -> Option<usize> {
        return index_if([&value](T const& v) -> bool { return TypeTraits<T>::equals(value, v); });
    }
    auto index_if(Predicate<T const&> auto predicate) const -> Option<usize> {
        for ( auto const i : usize::range(0, m_values_count) ) {
            if ( predicate(m_data_storage[i]) ) {
                return i;
            }
        }
        return {};
    }

    /**
     * @brief Returns a reference to the element into this Vector if exists
     */
    auto find(T const& value) -> Option<T&> {
        return index_of(value).template map<T&>([this](usize index) -> T& { return at(index); });
    }
    auto find(T const& value) const -> Option<T const&> {
        return index_of(value).template map<T const&>([this](usize index) -> T const& { return at(index); });
    }

    /**
     * @brief Returns a reference to the element if the callback returns true
     */
    auto find_if(Predicate<T const&> auto predicate) -> Option<T&> {
        return index_if(predicate).template map<T&>([this](usize index) -> T& { return at(index); });
    }
    auto find_if(Predicate<T const&> auto predicate) const -> Option<T const&> {
        return index_if(predicate).template map<T const&>([this](usize index) -> T const& { return at(index); });
    }

    /**
     * @brief Vector data access
     */
    auto at(usize index) -> T& {
        verify_less$(index, m_values_count);
        return m_data_storage[index];
    }
    auto at(usize index) const -> T const& {
        verify_less$(index, m_values_count);
        return m_data_storage[index];
    }

    auto operator[](usize index) -> T& {
        return at(index);
    }
    auto operator[](usize index) const -> T const& {
        return at(index);
    }

    auto as_slice() const -> Slice<T> {
        return m_data_storage.sub_slice(0, m_values_count);
    }

    auto count() const -> usize {
        return m_values_count;
    }
    auto capacity() const -> usize {
        return m_data_capacity;
    }
    auto raw_data() -> T* {
        return m_data_storage.data();
    }
    auto raw_data() const -> T const* {
        return m_data_storage.data();
    }

    auto is_empty() const -> bool {
        return m_values_count == 0;
    }
    auto any() const -> bool {
        return !is_empty();
    }

private:
    explicit constexpr Vector() = default;

    constexpr auto data_slot(usize index) -> T* {
        return &m_data_storage[index];
    }

    auto shift_forward_storage_values(usize start_index) -> void {
        if constexpr ( TypeTraits<T>::is_trivial() ) {
            Cxx::memmove(data_slot(start_index + 1), data_slot(start_index), (m_values_count - start_index) * sizeof(T));
        } else {
            for ( auto const i : usize::range(m_values_count, start_index).reverse_iter() ) {
                new (data_slot(i)) T(Cxx::move(m_data_storage[i - 1]));
                at(i - 1).~T();
            }
        }
    }

private:
    Slice<T> m_data_storage  = Slice<T>::empty();
    usize    m_data_capacity = 0;
    usize    m_values_count  = 0;
};

namespace Cxx {

template<typename T>
auto swap(Vector<T>& lhs, Vector<T>& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
