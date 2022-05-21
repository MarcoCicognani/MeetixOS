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

#include <initializer_list>
#include <LibTC/Assertions.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/Raw.hh>
#include <LibTC/Trait/TypeIntrinsics.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename Collection, typename T>
class VectorIterator {
public:
    /**
     * @brief Construction functions
     */
    static VectorIterator begin(Collection& collection) {
        return VectorIterator{ collection, 0 };
    }
    static VectorIterator end(Collection& collection) {
        return VectorIterator{ collection, collection.count() };
    }

    VectorIterator(VectorIterator const&) = default;

    VectorIterator& operator=(VectorIterator const&) = default;

    /**
     * @brief Increment operators
     */
    VectorIterator& operator++() {
        ++m_index;
        return *this;
    }
    VectorIterator operator++(int) {
        VectorIterator it{ *this };
        ++m_index;
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    T& operator*() {
        VERIFY_FALSE(is_end());
        return m_collection->at(m_index);
    }
    T const& operator*() const {
        VERIFY_FALSE(is_end());
        return m_collection->at(m_index);
    }

    /**
     * @brief Pointer access operators
     */
    T* operator->() {
        return &operator*();
    }
    T const* operator->() const {
        return &operator*();
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] bool is_end() const {
        return m_index == end(*m_collection).index();
    }
    [[nodiscard]] usize index() const {
        return m_index;
    }

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(VectorIterator const& rhs) const {
        return m_collection == rhs.m_collection && m_index == rhs.m_index;
    }
    [[nodiscard]] bool operator!=(VectorIterator const& rhs) const {
        return m_collection != rhs.m_collection || m_index != rhs.m_index;
    }
    [[nodiscard]] bool operator<(VectorIterator const& rhs) const {
        return m_index < rhs.m_index;
    }
    [[nodiscard]] bool operator>(VectorIterator const& rhs) const {
        return m_index > rhs.m_index;
    }
    [[nodiscard]] bool operator<=(VectorIterator const& rhs) const {
        return m_index <= rhs.m_index;
    }
    [[nodiscard]] bool operator>=(VectorIterator const& rhs) const {
        return m_index >= rhs.m_index;
    }

private:
    VectorIterator(Collection& collection, usize index)
        : m_collection{ &collection }
        , m_index{ index } {
    }

private:
    Collection* m_collection{ nullptr };
    usize       m_index{ 0 };
};

} /* namespace Details */

template<typename T>
class Vector {
public:
    using Iterator      = Details::VectorIterator<Vector, T>;
    using ConstIterator = Details::VectorIterator<Vector const, T const>;

    enum class KeepStorageCapacity {
        Yes,
        No
    };

    enum AdoptTag {
        Adopt
    };

public:
    /**
     * @brief Constructors
     */
    explicit Vector() = default;
    explicit Vector(usize capacity) {
        ensure_capacity(capacity);
    }
    explicit Vector(AdoptTag, T* data_storage, usize size)
        : m_data_storage{ data_storage }
        , m_data_capacity{ size }
        , m_values_count{ size } {
    }

    Vector(Vector const& rhs)
        : m_values_count{ rhs.m_values_count } {
        if ( !is_empty() ) {
            ensure_capacity(rhs.count());
            for ( usize i = 0; i < m_values_count; ++i )
                new (&m_data_storage[i]) T{ rhs.at(i) };
        }
    }
    Vector(Vector&& rhs) noexcept
        : m_data_storage{ exchange(rhs.m_data_storage, nullptr) }
        , m_data_capacity{ exchange(rhs.m_data_capacity, 0) }
        , m_values_count{ exchange(rhs.m_values_count, 0) } {
    }
    Vector(std::initializer_list<T> initializer_list) {
        ensure_capacity(initializer_list.size());
        for ( auto const& element : initializer_list )
            append_unchecked(move(element));
    }
    ~Vector() {
        clear(KeepStorageCapacity::No);
    }

    Vector& operator=(Vector const& rhs) {
        if ( this == &rhs )
            return *this;

        Vector vector{ rhs };
        swap(vector);
        return *this;
    }
    Vector& operator=(Vector&& rhs) noexcept {
        Vector vector{ move(rhs) };
        swap(vector);
        return *this;
    }
    Vector& operator=(std::initializer_list<T> initializer_list) {
        Vector vector{ initializer_list };
        swap(vector);
        return *this;
    }

    /**
     * @brief Destroys all the stored values keeping the capacity of this vector if requested
     */
    void clear(KeepStorageCapacity keep_storage_capacity = KeepStorageCapacity::Yes) {
        if ( m_data_storage == nullptr )
            return;

        /* call the destructors only for non-trivial types */
        if constexpr ( !Trait::TypeIntrinsics<T>::is_trivial() ) {
            for ( usize i = 0; i < m_values_count; ++i )
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
    void swap(Vector& rhs) noexcept {
        Cxx::swap(m_data_storage, rhs.m_data_storage);
        Cxx::swap(m_data_capacity, rhs.m_data_capacity);
        Cxx::swap(m_values_count, rhs.m_values_count);
    }

    /**
     * @brief Inserts the given <value> to the given <index>
     */
    void insert_at(usize index, T const& value) {
        MUST(try_insert_at(index, T{ value }));
    }
    template<typename U = T>
    void insert_at(usize index, U&& value) {
        MUST(try_insert_at(index, forward<U>(value)));
    }

    ErrorOr<void> try_insert_at(usize index, T const& value) {
        return try_insert_at(index, T{ value });
    }
    template<typename U = T>
    ErrorOr<void> try_insert_at(usize index, U&& value) {
        if ( index > m_values_count )
            return Error{ EINVAL };

        TRY(try_ensure_capacity(m_values_count + 1));

        /* move the values after the insertion one place forward */
        if ( index < m_values_count ) {
            if constexpr ( Trait::TypeIntrinsics<T>::is_trivial() )
                __builtin_memmove(data_slot(index + 1), data_slot(index), (m_values_count - index) * sizeof(T));
            else {
                for ( usize i = m_values_count; i > index; --i ) {
                    new (data_slot(i)) T{ forward<U>(m_data_storage[i - 1]) };
                    at(i - 1).~T();
                }
            }
        }

        /* move the value into the memory */
        new (data_slot(index)) T{ forward<U>(value) };
        ++m_values_count;
        return {};
    }

    /**
     * @brief Inserts the given <value> sorted
     */
    template<typename Comparator>
    void insert_sorted(T const& value, Comparator comparator) {
        MUST(try_insert_sorted(value, comparator));
    }
    void insert_sorted(T const& value) {
        MUST(try_insert_sorted(value));
    }

    template<typename Comparator>
    ErrorOr<void> try_insert_sorted(T const& value, Comparator comparator) {
        /* find the insertion index */
        usize insert_index = 0;
        for ( usize i = 0; i < m_values_count; ++i ) {
            if ( comparator(m_data_storage[i], value) )
                insert_index = i + 1;
            else
                break;
        }

        /* insert at the position */
        return try_insert_at(insert_index, value);
    }
    ErrorOr<void> try_insert_sorted(T const& value) {
        return try_insert_sorted(value, [](T const& a, T const& b) { return a < b; });
    }

    /**
     * @brief Pushes the given <value> to the begin of the vector
     */
    void prepend(T const& value) {
        MUST(try_prepend(T{ value }));
    }
    template<typename U = T>
    void prepend(U&& value) {
        MUST(try_prepend<U>(forward<U>(value)));
    }

    ErrorOr<void> try_prepend(T const& value) {
        return try_prepend(T{ value });
    }
    template<typename U = T>
    ErrorOr<void> try_prepend(U&& value) {
        return try_insert_at<U>(0, forward<U>(value));
    }

    /**
     * @brief Pushes the given <value> to the end of the vector
     */
    void append(T const& value) {
        MUST(try_append(T{ value }));
    }
    template<typename U = T>
    void append(U&& value) {
        MUST(try_append<U>(forward<U>(value)));
    }

    ErrorOr<void> try_append(T const& value) {
        return try_append(T{ value });
    }
    template<typename U = T>
    ErrorOr<void> try_append(U&& value) {
        return try_insert_at<U>(m_values_count, forward<U>(value));
    }

    void append_unchecked(T const& value) {
        append_unchecked(T{ value });
    }
    template<typename U = T>
    void append_unchecked(U&& value) {
        new (data_slot(m_values_count)) T{ forward<U>(value) };
        ++m_values_count;
    }

    /**
     * @brief Pushes a new value to the begin of the vector constructing it with the given arguments
     */
    template<typename... Args>
    void emplace_first(Args&&... args) {
        MUST(try_emplace_first(forward<Args>(args)...));
    }
    template<typename... Args>
    ErrorOr<void> try_emplace_first(Args&&... args) {
        TRY(try_ensure_capacity(m_values_count + 1));

        /* move the values after the insertion one place forward */
        if constexpr ( Trait::TypeIntrinsics<T>::is_trivial() )
            __builtin_memmove(data_slot(1), m_data_storage, m_values_count * sizeof(T));
        else {
            for ( usize i = m_values_count; i > 0; --i ) {
                new (data_slot(i)) T{ move(m_data_storage[i - 1]) };
                at(i - 1).~T();
            }
        }

        /* move the value into the memory */
        new (data_slot(0)) T{ forward<Args>(args)... };
        ++m_values_count;
        return {};
    }

    /**
     * @brief Pushes a new value to the end of the vector constructing it with the given arguments
     */
    template<typename... Args>
    void emplace_last(Args&&... args) {
        MUST(try_emplace_last(forward<Args>(args)...));
    }
    template<typename... Args>
    ErrorOr<void> try_emplace_last(Args&&... args) {
        TRY(try_ensure_capacity(m_values_count + 1));

        /* move the value into the memory */
        new (data_slot(m_values_count)) T{ forward<Args>(args)... };
        ++m_values_count;
        return {};
    }

    /**
     * @brief Returns a reference to the first element
     */
    T& first() {
        return at(0);
    }
    T const& first() const {
        return at(0);
    }

    /**
     * @brief Returns a reference to the last element
     * @return
     */
    T& last() {
        return at(m_values_count - 1);
    }
    T const& last() const {
        return at(m_values_count - 1);
    }

    /**
     * @brief Removes the element at the given index and returns it
     */
    [[nodiscard]] T take_first() {
        return take_at(0);
    }
    [[nodiscard]] T take_last() {
        return take_at(count() - 1);
    }
    [[nodiscard]] T take_at(usize index) {
        VERIFY_LESS(index, m_values_count);

        T value{ move(m_data_storage[index]) };
        erase_at(index);
        return value;
    }

    /**
     * @brief Removes the element(s)
     */
    ErrorOr<void> erase_at(usize index) {
        if ( index >= m_values_count )
            return Error{ EINVAL };

        /* shift all the values one position back */
        if constexpr ( Trait::TypeIntrinsics<T>::is_trivial() )
            __builtin_memmove(data_slot(index), data_slot(index + 1), (m_values_count - index - 1) * sizeof(T));
        else {
            at(index).~T();
            for ( usize i = index + 1; i < m_values_count; ++i ) {
                new (data_slot(i - 1)) T{ move(m_data_storage[i]) };
                m_data_storage[i].~T();
            }
        }
        --m_values_count;
        return {};
    }
    ErrorOr<void> erase_first_of(T const& value) {
        for ( usize i = 0; i < m_values_count; ++i ) {
            if ( m_data_storage[i] == value ) {
                TRY(erase_at(i));
                return {};
            }
        }
        return Error{ ENOENT };
    }
    ErrorOr<usize> erase_all_of(T const& value) {
        return erase_all_matches([&value](T const& current) { return current == value; });
    }
    template<typename CallBack>
    ErrorOr<usize> erase_all_matches(CallBack matches) {
        usize erased_count = 0;
        for ( usize i = 0; i < m_values_count; ) {
            if ( matches(m_data_storage[i]) ) {
                erase_at(i);
                ++erased_count;
            } else
                ++i;
        }

        if ( erased_count > 0 )
            return erased_count;
        else
            return ENOENT;
    }

    /**
     * @brief Sorts the vector content using the given comparator
     */
    template<typename Comparator>
    void sort(Comparator comparator) {
        for ( usize i = 0; i + 1 < m_values_count; ++i ) {
            for ( usize j = i + 1; j < m_values_count; ++j ) {
                if ( comparator(m_data_storage[i], m_data_storage[j]) > 0 )
                    Cxx::swap(m_data_storage[i], m_data_storage[j]);
            }
        }
    }

    /**
     * @brief Resizes the amount of elements inside this vector creating (by default constructor) if new_count >
     * count(), destroying if new_count < count()
     */
    void resize(usize new_count) {
        MUST(try_resize(new_count));
    }
    ErrorOr<void> try_resize(usize new_count) {
        if ( new_count < m_values_count ) {
            for ( usize i = new_count; i < m_values_count; ++i )
                at(i).~T();
        } else if ( new_count > m_values_count ) {
            TRY(try_ensure_capacity(new_count));

            /* default construct the new values */
            for ( usize i = m_values_count; i < new_count; ++i )
                new (data_slot(i)) T{};
        }

        m_values_count = new_count;
        return {};
    }

    /**
     * @brief Allocates new capacity for this vector for at least the given capacity
     */
    void ensure_capacity(usize capacity) {
        MUST(try_ensure_capacity(capacity));
    }
    ErrorOr<void> try_ensure_capacity(usize capacity) {
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
        if constexpr ( Trait::TypeIntrinsics<T>::is_trivial() )
            __builtin_memmove(new_data_storage, m_data_storage, m_values_count * sizeof(T));
        else {
            for ( usize i = 0; i < m_values_count; ++i ) {
                new (&new_data_storage[i]) T{ move(at(i)) };
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
    Iterator begin() {
        return Iterator::begin(*this);
    }
    Iterator end() {
        return Iterator::end(*this);
    }

    ConstIterator begin() const {
        return ConstIterator::begin(*this);
    }
    ConstIterator end() const {
        return ConstIterator::end(*this);
    }

    /**
     * @brief Returns whether this vector contains the given value
     */
    [[nodiscard]] bool contains(T const& value) const {
        return index_of(value).is_present();
    }

    /**
     * @brief Returns the index of the value if exists or when the callback returns true
     */
    Option<usize> index_of(T const& value) const {
        return index_if([&value](auto const& v) { return v == value; });
    }
    template<typename Callback>
    Option<usize> index_if(Callback callback) const {
        for ( usize i = 0; i < m_values_count; ++i ) {
            if ( callback(m_data_storage[i]) )
                return i;
        }
        return {};
    }

    /**
     * @brief Returns a reference to the element into this Vector if exists
     */
    Option<T&> find(T const& value) {
        auto index_or_none = index_of(value);
        if ( index_or_none.is_present() )
            return at(index_or_none.value());
        else
            return {};
    }
    Option<T const&> find(T const& value) const {
        auto index_or_none = index_of(value);
        if ( index_or_none.is_present() )
            return at(index_or_none.value());
        else
            return {};
    }

    /**
     * @brief Returns a reference to the element if the callback returns true
     */
    template<typename Callback>
    Option<T&> find_if(Callback callback) {
        auto index_or_none = index_if(callback);
        if ( index_or_none.is_present() )
            return at(index_or_none.value());
        else
            return {};
    }
    template<typename Callback>
    Option<T const&> find_if(Callback callback) const {
        auto index_or_none = index_if(callback);
        if ( index_or_none.is_present() )
            return at(index_or_none.value());
        else
            return {};
    }

    /**
     * @brief Vector data access
     */
    T& at(usize index) {
        VERIFY_LESS(index, m_values_count);
        return m_data_storage[index];
    }
    T const& at(usize index) const {
        VERIFY_LESS(index, m_values_count);
        return m_data_storage[index];
    }

    T& operator[](usize index) {
        return at(index);
    }
    T const& operator[](usize index) const {
        return at(index);
    }

    [[nodiscard]] usize count() const {
        return m_values_count;
    }
    [[nodiscard]] usize capacity() const {
        return m_data_capacity;
    }
    [[nodiscard]] T* data() {
        return m_data_storage;
    }
    [[nodiscard]] T const* data() const {
        return m_data_storage;
    }

    [[nodiscard]] bool is_empty() const {
        return m_values_count == 0;
    }
    [[nodiscard]] bool any() const {
        return !is_empty();
    }

private:
    T* data_slot(usize index) {
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
