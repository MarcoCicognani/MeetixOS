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

#include <LibTC/Assertions.hh>
#include <LibTC/Collection/Enums/KeepStorageCapacity.hh>
#include <LibTC/Collection/Range.hh>
#include <LibTC/Collection/ReverseIteratorSupport.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/Raw.hh>
#include <LibTC/Meta.hh>
#include <LibTC/TypeTraits.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename TCollection, typename T, bool IsReverse>
class VectorIterator {
    using TIndex = Conditional<IsReverse, isize, usize>;

public:
    /**
     * @brief Construction functions
     */
    static auto begin(TCollection& collection) -> VectorIterator {
        return VectorIterator{ collection, 0 };
    }
    static auto end(TCollection& collection) -> VectorIterator {
        return VectorIterator{ collection, collection.count() };
    }

    static auto rbegin(TCollection& collection) -> VectorIterator {
        return VectorIterator{ collection, collection.count() - 1 };
    }
    static auto rend(TCollection& collection) -> VectorIterator {
        return VectorIterator{ collection, -1 };
    }

    VectorIterator(VectorIterator const&) = default;

    auto operator=(VectorIterator const&) -> VectorIterator& = default;

    /**
     * @brief Increment operators
     */
    auto operator++() -> VectorIterator& {
        if constexpr ( IsReverse ) {
            --m_index;
        } else {
            ++m_index;
        }
        return *this;
    }
    auto operator++(int) -> VectorIterator {
        VectorIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    auto operator*() -> T& {
        VERIFY_FALSE(is_end());
        if constexpr ( IsReverse ) {
            return m_collection->at(static_cast<usize>(m_index));
        } else {
            return m_collection->at(m_index);
        }
    }
    auto operator*() const -> T const& {
        VERIFY_FALSE(is_end());
        if constexpr ( IsReverse ) {
            return m_collection->at(static_cast<usize>(m_index));
        } else {
            return m_collection->at(m_index);
        }
    }

    /**
     * @brief Pointer access operators
     */
    auto operator->() -> T* {
        return &operator*();
    }
    auto operator->() const -> T const* {
        return &operator*();
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] auto is_end() const -> bool {
        if constexpr ( IsReverse ) {
            return m_index == rend(*m_collection).index();
        } else {
            return m_index == end(*m_collection).index();
        }
    }
    [[nodiscard]] auto index() const -> usize {
        return m_index;
    }

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] auto operator==(VectorIterator const& rhs) const -> bool {
        return m_collection == rhs.m_collection && m_index == rhs.m_index;
    }
    [[nodiscard]] auto operator!=(VectorIterator const& rhs) const -> bool {
        return m_collection != rhs.m_collection || m_index != rhs.m_index;
    }
    [[nodiscard]] auto operator<(VectorIterator const& rhs) const -> bool {
        return m_index < rhs.m_index;
    }
    [[nodiscard]] auto operator>(VectorIterator const& rhs) const -> bool {
        return m_index > rhs.m_index;
    }
    [[nodiscard]] auto operator<=(VectorIterator const& rhs) const -> bool {
        return m_index <= rhs.m_index;
    }
    [[nodiscard]] auto operator>=(VectorIterator const& rhs) const -> bool {
        return m_index >= rhs.m_index;
    }

private:
    VectorIterator(TCollection& collection, TIndex index)
        : m_collection{ &collection }
        , m_index{ index } {
    }

private:
    TCollection* m_collection{ nullptr };
    TIndex       m_index{ 0 };
};

} /* namespace Details */

template<typename T>
class Vector {
    TC_DENY_COPY(Vector);

public:
    using Iterator                    = Details::VectorIterator<Vector, T, false>;
    using ConstIterator               = Details::VectorIterator<Vector const, T const, false>;
    using ReverseIterator             = Details::VectorIterator<Vector, T, true>;
    using ConstReverseIterator        = Details::VectorIterator<Vector const, T const, true>;
    using ReverseIteratorWrapper      = ReverseIteratorSupport::Wrapper<Vector<T>>;
    using ConstReverseIteratorWrapper = ReverseIteratorSupport::Wrapper<Vector<T> const>;

public:
    /**
     * @brief Non-error safe Factory functions
     */
    [[nodiscard]] static constexpr auto construct_empty() -> Vector<T> {
        return Vector<T>{};
    }
    [[nodiscard]] static auto construct_with_capacity(usize capacity) -> Vector<T> {
        return MUST(try_construct_with_capacity(capacity));
    }
    [[nodiscard]] static auto construct_from_other(Vector<T> const& rhs) -> Vector<T> {
        return MUST(try_construct_from_other(rhs));
    }
    [[nodiscard]] static auto construct_from_list(Cxx::initializer_list<T> initializer_list) -> Vector<T> {
        return MUST(try_construct_from_list(initializer_list));
    }

    /**
     * @brief Error safe Factory functions
     */
    [[nodiscard]] static auto try_construct_with_capacity(usize capacity) -> ErrorOr<Vector<T>> {
        auto vector = construct_empty();
        TRY(vector.try_ensure_capacity(capacity));
        return vector;
    }
    [[nodiscard]] static auto try_construct_from_other(Vector<T> const& rhs) -> ErrorOr<Vector<T>> {
        auto vector = TRY(try_construct_with_capacity(rhs.count()));
        for ( auto const& element : rhs ) {
            if constexpr ( TryCloneable<T, ErrorOr<T>> ) {
                TRY(vector.try_append(TRY(element.try_clone())));
            } else if constexpr ( Cloneable<T> ) {
                TRY(vector.try_append(element.clone()));
            } else if constexpr ( CopyConstructible<T> ) {
                TRY(vector.try_append(element));
            }
        }

        return vector;
    }
    [[nodiscard]] static auto try_construct_from_list(Cxx::initializer_list<T> initializer_list) -> ErrorOr<Vector<T>> {
        auto vector = construct_empty();
        for ( auto const& element : initializer_list ) /* even with auto initializer_list exposes only T const& */
            TRY(vector.try_append(Cxx::move(const_cast<T&>(element))));

        return vector;
    }

    /**
     * @brief Move constructor and move assignment
     */
    Vector(Vector<T>&& rhs) noexcept
        : m_data_storage{ Cxx::exchange(rhs.m_data_storage, nullptr) }
        , m_data_capacity{ Cxx::exchange(rhs.m_data_capacity, 0) }
        , m_values_count{ Cxx::exchange(rhs.m_values_count, 0) } {
    }
    auto operator=(Vector<T>&& rhs) noexcept -> Vector<T>& {
        Vector vector{ Cxx::move(rhs) };
        swap(vector);
        return *this;
    }

    ~Vector() {
        clear(KeepStorageCapacity::No);
    }

    /**
     * @brief Deep cloning
     */
    [[nodiscard]] auto clone() const -> Vector<T> {
        return MUST(try_clone());
    }
    [[nodiscard]] auto try_clone() const -> ErrorOr<Vector<T>> {
        return Vector<T>::try_construct_from_other(*this);
    }

    /**
     * @brief Destroys all the stored values keeping the capacity of this vector if requested
     */
    auto clear(KeepStorageCapacity keep_storage_capacity = KeepStorageCapacity::Yes) {
        if ( m_data_storage == nullptr )
            return;

        /* call the destructors only for non-trivial types */
        if constexpr ( !TypeTraits<T>::is_trivial() ) {
            for ( usize i : Range{ 0u, m_values_count } )
                m_data_storage[i].~T();
        }

        /* free the memory if requested */
        if ( keep_storage_capacity == KeepStorageCapacity::No ) {
            Memory::Raw::free_sized(m_data_storage, m_data_capacity);
            m_data_storage  = nullptr;
            m_data_capacity = 0;
        }
        m_values_count = 0;
    }

    /**
     * @brief Swaps in O(1) the content of this Vector with another
     */
    auto swap(Vector& rhs) noexcept {
        Cxx::swap(m_data_storage, rhs.m_data_storage);
        Cxx::swap(m_data_capacity, rhs.m_data_capacity);
        Cxx::swap(m_values_count, rhs.m_values_count);
    }

    /**
     * @brief Inserts the given <value> to the given <index>
     */
    auto insert_at(usize index, T value) {
        MUST(try_insert_at(index, Cxx::move(value)));
    }
    auto try_insert_at(usize index, T value) -> ErrorOr<void> {
        if ( index > m_values_count )
            return Error{ EINVAL };

        TRY(try_ensure_capacity(m_values_count + 1));

        /* move the values after the insertion one place forward */
        if ( index < m_values_count ) {
            if constexpr ( TypeTraits<T>::is_trivial() )
                __builtin_memmove(data_slot(index + 1), data_slot(index), (m_values_count - index) * sizeof(T));
            else {
                for ( usize i : Range{ m_values_count, index }.reverse_iter() ) {
                    new (data_slot(i)) T{ Cxx::move(m_data_storage[i - 1]) };
                    at(i - 1).~T();
                }
            }
        }

        /* move the value into the memory */
        new (data_slot(index)) T{ Cxx::move(value) };
        ++m_values_count;
        return {};
    }

    /**
     * @brief Pushes the given <value> to the begin of the vector
     */
    auto prepend(T value) {
        MUST(try_prepend(Cxx::move(value)));
    }
    auto try_prepend(T value) -> ErrorOr<void> {
        return try_insert_at(0, Cxx::move(value));
    }

    /**
     * @brief Pushes the given <value> to the end of the vector
     */
    void append(T value) {
        MUST(try_append(Cxx::move(value)));
    }
    auto try_append(T value) -> ErrorOr<void> {
        TRY(try_ensure_capacity(m_values_count + 1));
        append_unchecked(Cxx::move(value));
        return {};
    }

    auto append_unchecked(T value) {
        new (data_slot(m_values_count)) T{ Cxx::move(value) };
        ++m_values_count;
    }

    /**
     * @brief Pushes a new value to the begin of the vector constructing it with the given arguments
     */
    template<typename... Args>
    auto emplace_first(Args&&... args) {
        MUST(try_emplace_first(Cxx::forward<Args>(args)...));
    }
    template<typename... Args>
    auto try_emplace_first(Args&&... args) -> ErrorOr<void> {
        TRY(try_ensure_capacity(m_values_count + 1));

        /* move the values after the insertion one place forward */
        if constexpr ( TypeTraits<T>::is_trivial() )
            __builtin_memmove(data_slot(1), m_data_storage, m_values_count * sizeof(T));
        else {
            for ( usize i : Range{ m_values_count, 0u }.reverse_iter() ) {
                new (data_slot(i)) T{ Cxx::move(m_data_storage[i - 1]) };
                at(i - 1).~T();
            }
        }

        /* move the value into the memory */
        new (data_slot(0)) T{ Cxx::forward<Args>(args)... };
        ++m_values_count;
        return {};
    }

    /**
     * @brief Pushes a new value to the end of the vector constructing it with the given arguments
     */
    template<typename... Args>
    auto emplace_last(Args&&... args) {
        MUST(try_emplace_last(Cxx::forward<Args>(args)...));
    }
    template<typename... Args>
    auto try_emplace_last(Args&&... args) -> ErrorOr<void> {
        TRY(try_ensure_capacity(m_values_count + 1));
        new (data_slot(m_values_count)) T{ Cxx::forward<Args>(args)... };
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
        if ( index >= m_values_count )
            return Error{ EINVAL };

        /* shift all the values one position back */
        if constexpr ( TypeTraits<T>::is_trivial() )
            __builtin_memmove(data_slot(index), data_slot(index + 1), (m_values_count - index - 1) * sizeof(T));
        else {
            at(index).~T();
            for ( usize i : Range{ index + 1, m_values_count } ) {
                new (data_slot(i - 1)) T{ Cxx::move(m_data_storage[i]) };
                m_data_storage[i].~T();
            }
        }
        --m_values_count;
        return {};
    }
    auto erase_first_of(T const& value) -> ErrorOr<void> {
        for ( usize i : Range{ 0u, m_values_count } ) {
            if ( m_data_storage[i] == value ) {
                TRY(erase_at(i));
                return {};
            }
        }
        return Error{ ENOENT };
    }
    auto erase_all_of(T const& value) -> ErrorOr<usize> {
        return erase_all_matches([&value](T const& current) { return TypeTraits<T>::equals(current, value); });
    }
    auto erase_all_matches(Predicate<T const&> auto predicate) -> ErrorOr<usize> {
        usize erased_count = 0;
        for ( usize i = 0; i < m_values_count; ) {
            if ( predicate(m_data_storage[i]) ) {
                TRY(erase_at(i));
                ++erased_count;
            } else
                ++i;
        }

        if ( erased_count > 0 )
            return erased_count;
        else
            return Error{ ENOENT };
    }

    /**
     * @brief Sorts the vector content using the given comparator
     */
    auto sort(I32Predicate<T const&, T const&> auto comparator) {
        for ( usize i = 0; i + 1 < m_values_count; ++i ) {
            for ( usize j = i + 1; j < m_values_count; ++j ) {
                if ( comparator(m_data_storage[i], m_data_storage[j]) > 0 )
                    Cxx::swap(m_data_storage[i], m_data_storage[j]);
            }
        }
    }

    /**
     * @brief Allocates new capacity for this vector for at least the given capacity
     */
    auto ensure_capacity(usize capacity) {
        MUST(try_ensure_capacity(capacity));
    }
    auto try_ensure_capacity(usize capacity) -> ErrorOr<void> {
        VERIFY_GREATER(capacity, 0);

        if ( m_data_capacity >= capacity )
            return {};

        /* for first time use the given capacity, otherwise increase the capacity logarithmically */
        usize new_capacity;
        if ( m_data_capacity == 0 )
            new_capacity = capacity == 0 ? 16 : capacity;
        else
            new_capacity = capacity + capacity / 4;

        /* allocate new memory and move the content into it */
        auto new_data_storage = TRY(Memory::Raw::clean_alloc_array<T>(new_capacity));
        if constexpr ( TypeTraits<T>::is_trivial() )
            __builtin_memmove(new_data_storage, m_data_storage, m_values_count * sizeof(T));
        else {
            for ( usize i : Range{ 0u, m_values_count } ) {
                new (&new_data_storage[i]) T{ Cxx::move(at(i)) };
                at(i).~T();
            }
        }

        /* destroy the previous buffer if exists and update the other fields */
        if ( m_data_storage )
            Memory::Raw::free_sized<T>(m_data_storage, m_data_capacity);

        m_data_storage  = new_data_storage;
        m_data_capacity = new_capacity;
        return {};
    }

    /**
     * @brief for-each support
     */
    auto begin() -> Iterator {
        return Iterator::begin(*this);
    }
    auto end() -> Iterator {
        return Iterator::end(*this);
    }

    auto begin() const -> ConstIterator {
        return ConstIterator::begin(*this);
    }
    auto end() const -> ConstIterator {
        return ConstIterator::end(*this);
    }

    /**
     * @brief reverse for-each support
     */
    auto rbegin() -> ReverseIterator {
        return ReverseIterator::rbegin(*this);
    }
    auto rend() -> ReverseIterator {
        return ReverseIterator::rend(*this);
    }

    auto rbegin() const -> ConstReverseIterator {
        return ReverseIterator::rbegin(*this);
    }
    auto rend() const -> ConstReverseIterator {
        return ReverseIterator::rend(*this);
    }

    auto reverse_iter() -> ReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }
    auto reverse_iter() const -> ConstReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }

    /**
     * @brief Returns whether this vector contains the given value
     */
    [[nodiscard]] auto contains(T const& value) const -> bool {
        return index_of(value).is_present();
    }

    /**
     * @brief Returns the index of the value if exists or when the callback returns true
     */
    auto index_of(T const& value) const -> Option<usize> {
        return index_if([&value](T const& v) -> bool { return TypeTraits<T>::equals(value, v); });
    }
    auto index_if(Predicate<T const&> auto predicate) const -> Option<usize> {
        for ( usize i : Range{ 0u, m_values_count } ) {
            if ( predicate(m_data_storage[i]) )
                return i;
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
        VERIFY_LESS(index, m_values_count);
        return m_data_storage[index];
    }
    auto at(usize index) const -> T const& {
        VERIFY_LESS(index, m_values_count);
        return m_data_storage[index];
    }

    auto operator[](usize index) -> T& {
        return at(index);
    }
    auto operator[](usize index) const -> T const& {
        return at(index);
    }

    [[nodiscard]] auto count() const -> usize {
        return m_values_count;
    }
    [[nodiscard]] auto capacity() const -> usize {
        return m_data_capacity;
    }
    [[nodiscard]] auto raw_data() -> T* {
        return m_data_storage;
    }
    [[nodiscard]] auto raw_data() const -> T const* {
        return m_data_storage;
    }

    [[nodiscard]] auto is_empty() const -> bool {
        return m_values_count == 0;
    }
    [[nodiscard]] auto any() const -> bool {
        return !is_empty();
    }

private:
    explicit constexpr Vector() noexcept = default;

    [[gnu::always_inline]] auto data_slot(usize index) -> T* {
        return &m_data_storage[index];
    }

private:
    T*    m_data_storage{ nullptr };
    usize m_data_capacity{ 0 };
    usize m_values_count{ 0 };
};

} /* namespace Collection */

using Collection::Vector;

} /* namespace TC */
