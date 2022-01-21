/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <initializer_list>
#include <TC/Assertion.hh>
#include <TC/Collection/CommonIterator.hh>
#include <TC/Functional/ErrorOr.hh>
#include <TC/Functional/Must.hh>
#include <TC/Functional/Try.hh>
#include <TC/IntTypes.hh>
#include <TC/RawMemory.hh>
#include <TC/Std/Exchange.hh>
#include <TC/Std/Forward.hh>
#include <TC/Std/Move.hh>
#include <TC/Std/New.hh>
#include <TC/Std/Swap.hh>
#include <TC/Tag/Adopt.hh>
#include <TC/Trait/Type.hh>

namespace TC::Collection {

template<typename T>
class Vector {
public:
    using Iterator      = CommonIterator<Vector, T>;
    using ConstIterator = CommonIterator<Vector const, T const>;

    enum class KeepStorageCapacity {
        Yes,
        No
    };

public:
    /**
     * @brief Constructors
     */
    explicit Vector();
    explicit Vector(usize capacity);
    explicit Vector(TC::Tag::Adopt, T* data_storage, usize size);

    Vector(Vector const& rhs);
    Vector(Vector&& rhs) noexcept;
    Vector(std::initializer_list<T> initializer_list);

    ~Vector();

    /**
     * @brief Destroys all the stored values keeping the capacity of this vector
     */
    void clear(KeepStorageCapacity keep_storage_capacity = KeepStorageCapacity::Yes);

    /**
     * @brief Inserts the given <value> to the given <index>
     */
    void                      insert_at(usize index, T const& value);
    void                      insert_at(usize index, T&& value);
    Functional::ErrorOr<void> try_insert_at(usize index, T const& value);
    Functional::ErrorOr<void> try_insert_at(usize index, T&& value);

    /**
     * @brief Inserts the given <value> sorted
     */
    template<typename Comparator>
    void insert_sorted(T const& value, Comparator comparator);
    void insert_sorted(T const& value);
    template<typename Comparator>
    Functional::ErrorOr<void> try_insert_sorted(T const& value, Comparator comparator);
    Functional::ErrorOr<void> try_insert_sorted(T const& value);

    /**
     * @brief Pushes the given <value> to the begin of the vector
     */
    void                      prepend(T const& value);
    void                      prepend(T&& value);
    Functional::ErrorOr<void> try_prepend(T const& value);
    Functional::ErrorOr<void> try_prepend(T&& value);

    /**
     * @brief Pushes the given <value> to the end of the vector
     * @param value
     * @return
     */
    void                      append(T const& value);
    void                      append(T&& value);
    Functional::ErrorOr<void> try_append(T const& value);
    Functional::ErrorOr<void> try_append(T&& value);

    void append_unchecked(T const& value);
    void append_unchecked(T&& value);

    /**
     * @brief Pushes a new value to the begin of the vector constructing it with the given arguments
     */
    template<typename... Args>
    void emplace_first(Args&&... args);
    template<typename... Args>
    Functional::ErrorOr<void> try_emplace_first(Args&&... args);

    /**
     * @brief Pushes a new value to the end of the vector constructing it with the given arguments
     */
    template<typename... Args>
    void emplace_last(Args&&... args);
    template<typename... Args>
    Functional::ErrorOr<void> try_emplace_last(Args&&... args);

    /**
     * @brief Returns a reference to the first element
     */
    T&       first();
    T const& first() const;

    /**
     * @brief Returns a reference to the last element
     * @return
     */
    T&       last();
    T const& last() const;

    /**
     * @brief Removes the element at the given index and returns it
     */
    [[nodiscard]] T take_first();
    [[nodiscard]] T take_last();
    [[nodiscard]] T take_at(usize index);

    /**
     * @brief Removes the element(s)
     */
    Functional::ErrorOr<void>  erase_at(usize index);
    Functional::ErrorOr<void>  erase_first_of(T const& value);
    Functional::ErrorOr<usize> erase_all_of(T const& value);
    template<typename CallBack>
    Functional::ErrorOr<usize> erase_all_matches(CallBack matches);

    /**
     * @brief Sorts the vector content using the given comparator
     */
    template<typename Comparator>
    void sort(Comparator comparator);

    /**
     * @brief Resizes the amount of elements inside this vector creating (by default constructor) if new_count >
     * count(), destroying if new_count < count()
     */
    void                      resize(usize new_count);
    Functional::ErrorOr<void> try_resize(usize new_count);

    /**
     * @brief Allocates new capacity for this vector for at least the given capacity
     */
    void                      ensure_capacity(usize capacity);
    Functional::ErrorOr<void> try_ensure_capacity(usize capacity);

    /**
     * @brief for-each support
     */
    Iterator begin();
    Iterator end();

    ConstIterator begin() const;
    ConstIterator end() const;

    /**
     * @brief Returns whether this vector contains the given value
     */
    [[nodiscard]] bool         contains(T const& value) const;
    Functional::ErrorOr<usize> contains_at(T const& value) const;

    T&       at(usize index);
    T const& at(usize index) const;

    T&       operator[](usize index);
    T const& operator[](usize index) const;

    [[nodiscard]] usize    count() const;
    [[nodiscard]] usize    capacity() const;
    [[nodiscard]] T*       data();
    [[nodiscard]] T const* data() const;

    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] bool any() const;

private:
    T*    m_data_storage{ nullptr };
    usize m_data_capacity{ 0 };
    usize m_values_count{ 0 };
};

template<typename T>
Vector<T>::Vector()
    : Vector(16) {
}

template<typename T>
Vector<T>::Vector(usize capacity) {
    ensure_capacity(capacity);
}

template<typename T>
Vector<T>::Vector(TC::Tag::Adopt, T* data_storage, usize size)
    : m_data_storage{ data_storage }
    , m_data_capacity{ size }
    , m_values_count{ size } {
}

template<typename T>
Vector<T>::Vector(Vector const& rhs)
    : m_values_count{ rhs.m_values_count } {
    ensure_capacity(rhs.count());
    for ( usize i = 0; i < m_values_count; ++i )
        new (&m_data_storage[i]) T{ rhs.at(i) };
}

template<typename T>
Vector<T>::Vector(Vector&& rhs) noexcept
    : m_data_storage{ std::exchange(rhs.m_data_storage, nullptr) }
    , m_data_capacity{ std::exchange(rhs.m_data_capacity, 0) }
    , m_values_count{ std::exchange(rhs.m_values_count, 0) } {
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> initializer_list) {
    ensure_capacity(initializer_list.size());

    for ( auto const& element : initializer_list )
        append_unchecked(element);
}

template<typename T>
Vector<T>::~Vector() {
    clear(KeepStorageCapacity::No);
}

template<typename T>
void Vector<T>::clear(KeepStorageCapacity keep_storage_capacity) {
    if ( m_data_storage != nullptr ) {
        for ( usize i = 0; i < m_values_count; ++i )
            m_data_storage[i].~T();

        /* free the memory if requested */
        if ( keep_storage_capacity == KeepStorageCapacity::No )
            RawMemory::free_sized(m_data_storage, m_data_capacity);
    }
    m_values_count = 0;
}

template<typename T>
void Vector<T>::insert_at(usize index, T const& value) {
    MUST(try_insert_at(index, T{ value }));
}

template<typename T>
void Vector<T>::insert_at(usize index, T&& value) {
    MUST(try_insert_at(index, std::move(value)));
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_insert_at(usize index, const T& value) {
    return try_insert_at(index, T{ value });
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_insert_at(usize index, T&& value) {
    if ( index > m_values_count )
        return EINVAL;

    TRY(try_ensure_capacity(m_values_count + 1));

    /* move the values after the insertion one place forward */
    if constexpr ( Trait::Type<T>::is_trivial() )
        __builtin_memmove(m_data_storage + index + 1, m_data_storage + index, m_values_count - index - 1);
    else {
        for ( usize i = m_values_count; i > index; --i ) {
            new (&m_data_storage[i]) T{ std::move(m_data_storage[i - 1]) };
            at(i - 1).~T();
        }
    }

    /* move the value into the memory */
    new (&m_data_storage[index]) T{ std::move(value) };
    ++m_values_count;
    return {};
}

template<typename T>
template<typename Comparator>
void Vector<T>::insert_sorted(T const& value, Comparator comparator) {
    MUST(try_insert_sorted(value, comparator));
}

template<typename T>
void Vector<T>::insert_sorted(T const& value) {
    MUST(try_insert_sorted(value));
}

template<typename T>
template<typename Comparator>
Functional::ErrorOr<void> Vector<T>::try_insert_sorted(const T& value, Comparator comparator) {
    /* find the insertion index */
    usize insert_index = 0;
    for ( usize i = 0; i < m_values_count; ++i ) {
        if ( comparator(m_data_storage[i], value) )
            insert_index = i;
        else
            break;
    }

    /* insert at the position */
    return try_insert_at(insert_index, value);
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_insert_sorted(const T& value) {
    return try_insert_sorted(value, [](T const& a, T const& b) { return a < b; });
}

template<typename T>
void Vector<T>::prepend(const T& value) {
    MUST(try_prepend(T{ value }));
}

template<typename T>
void Vector<T>::prepend(T&& value) {
    MUST(try_prepend(std::move(value)));
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_prepend(const T& value) {
    return try_prepend(T{ value });
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_prepend(T&& value) {
    return try_insert_at(0, std::move(value));
}

template<typename T>
void Vector<T>::append(T const& value) {
    MUST(try_append(T{ value }));
}

template<typename T>
void Vector<T>::append(T&& value) {
    MUST(try_append(std::move(value)));
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_append(const T& value) {
    return try_append(T{ value });
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_append(T&& value) {
    return try_insert_at(m_values_count, std::move(value));
}

template<typename T>
void Vector<T>::append_unchecked(const T& value) {
    append_unchecked(T{ value });
}

template<typename T>
void Vector<T>::append_unchecked(T&& value) {
    new (&m_data_storage[m_values_count]) T{ std::move(value) };
    ++m_values_count;
}

template<typename T>
template<typename... Args>
void Vector<T>::emplace_first(Args&&... args) {
    MUST(try_emplace_first(std::forward<Args>(args)...));
}

template<typename T>
template<typename... Args>
Functional::ErrorOr<void> Vector<T>::try_emplace_first(Args&&... args) {
    TRY(try_ensure_capacity(m_values_count + 1));

    /* move the values after the insertion one place forward */
    if constexpr ( Trait::Type<T>::is_trivial() )
        __builtin_memmove(m_data_storage + 1, m_data_storage, m_values_count - 1);
    else {
        for ( usize i = m_values_count; i > 0; --i ) {
            new (&m_data_storage[i]) T{ std::move(m_data_storage[i - 1]) };
            at(i - 1).~T();
        }
    }

    /* move the value into the memory */
    new (&m_data_storage[0]) T{ std::forward<Args>(args)... };
    ++m_values_count;
    return {};
}

template<typename T>
template<typename... Args>
void Vector<T>::emplace_last(Args&&... args) {
    MUST(try_emplace_last(std::forward<Args>(args)...));
}

template<typename T>
template<typename... Args>
Functional::ErrorOr<void> Vector<T>::try_emplace_last(Args&&... args) {
    TRY(try_ensure_capacity(m_values_count + 1));

    /* move the value into the memory */
    new (&m_data_storage[m_values_count]) T{ std::forward<Args>(args)... };
    ++m_values_count;
    return {};
}

template<typename T>
T& Vector<T>::first() {
    return at(0);
}

template<typename T>
T const& Vector<T>::first() const {
    return at(0);
}

template<typename T>
T& Vector<T>::last() {
    return at(m_values_count - 1);
}

template<typename T>
T const& Vector<T>::last() const {
    return at(m_values_count - 1);
}

template<typename T>
T Vector<T>::take_first() {
    return take_at(0);
}

template<typename T>
T Vector<T>::take_last() {
    return take_at(m_values_count - 1);
}

template<typename T>
T Vector<T>::take_at(usize index) {
    VERIFY(index < m_values_count);

    T value{ std::move(m_data_storage[index]) };
    erase_at(index);
    return value;
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::erase_at(usize index) {
    if ( index >= m_values_count )
        return EINVAL;

    /* destroy the value at the given index */
    m_data_storage[index].~T();

    /* shift all the values one position back */
    for ( usize i = index; i < m_values_count - 1; ++i ) {
        new (&m_data_storage[i]) T{ (std::move(m_data_storage[i + 1])) };
        m_data_storage[i + 1].~T();
    }
    --m_values_count;
    return {};
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::erase_first_of(T const& value) {
    for ( usize i = 0; i < m_values_count; ++i ) {
        if ( m_data_storage[i] == value ) {
            erase_at(i);
            return {};
        }
    }
    return ENOENT;
}

template<typename T>
Functional::ErrorOr<usize> Vector<T>::erase_all_of(T const& value) {
    return erase_all_matches([&value](T const& current) { return current == value; });
}

template<typename T>
template<typename Matcher>
Functional::ErrorOr<usize> Vector<T>::erase_all_matches(Matcher matches) {
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

template<typename T>
template<typename Comparator>
void Vector<T>::sort(Comparator comparator) {
    for ( usize i = 0; i + 1 < m_values_count; ++i ) {
        for ( usize j = i + 1; j < m_values_count; ++j ) {
            if ( comparator(m_data_storage[i], m_data_storage[j]) > 0 )
                std::swap(m_data_storage[i], m_data_storage[j]);
        }
    }
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_resize(usize new_count) {
    if ( new_count < m_values_count ) {
        for ( usize i = new_count; i < m_values_count; ++i )
            at(i).~T();
    } else if ( new_count > m_values_count ) {
        TRY(try_ensure_capacity(new_count));

        /* default construct the new values */
        for ( usize i = m_values_count; i < new_count; ++i )
            new (&m_data_storage[i]) T{};
    }

    m_values_count = new_count;
    return {};
}

template<typename T>
Functional::ErrorOr<void> Vector<T>::try_ensure_capacity(usize capacity) {
    if ( m_data_capacity >= capacity )
        return {};

    /* increase the capacity logarithmically */
    auto new_capacity = m_data_capacity == 0 ? 16 : m_data_capacity;
    while ( new_capacity <= capacity )
        new_capacity += new_capacity / 4;

    /* allocate new memory and move the content into it */
    auto new_data_storage = TRY(RawMemory::clean_alloc<T>(new_capacity));
    if constexpr ( Trait::Type<T>::is_trivial() )
        __builtin_memmove(new_data_storage, m_data_storage, m_values_count * sizeof(T));
    else {
        for ( usize i = 0; i < m_values_count; ++i ) {
            new (&new_data_storage[i]) T{ std::move(at(i)) };
            at(i).~T();
        }
    }

    /* destroy the previous buffer if exists and update the other fields */
    if ( m_data_storage )
        RawMemory::free_sized<T>(m_data_storage, m_data_capacity);

    m_data_storage  = new_data_storage;
    m_data_capacity = new_capacity;
    return {};
}

template<typename T>
void Vector<T>::resize(usize new_count) {
    MUST(try_resize(new_count));
}

template<typename T>
void Vector<T>::ensure_capacity(usize capacity) {
    MUST(try_ensure_capacity(capacity));
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::begin() {
    return Iterator::begin(*this);
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::end() {
    return Iterator::end(*this);
}

template<typename T>
typename Vector<T>::ConstIterator Vector<T>::begin() const {
    return ConstIterator::begin(*this);
}

template<typename T>
typename Vector<T>::ConstIterator Vector<T>::end() const {
    return ConstIterator::end(*this);
}

template<typename T>
bool Vector<T>::contains(T const& value) const {
    return contains_at(value).is_value();
}

template<typename T>
Functional::ErrorOr<usize> Vector<T>::contains_at(const T& value) const {
    for ( usize i = 0; i < m_values_count; ++i ) {
        if ( m_data_storage[i] == value )
            return i;
    }
    return ENOENT;
}

template<typename T>
T& Vector<T>::at(usize index) {
    VERIFY(index < m_values_count);

    return m_data_storage[index];
}

template<typename T>
T const& Vector<T>::at(usize index) const {
    VERIFY(index < m_values_count);

    return m_data_storage[index];
}

template<typename T>
T& Vector<T>::operator[](usize index) {
    return at(index);
}

template<typename T>
T const& Vector<T>::operator[](usize index) const {
    return at(index);
}

template<typename T>
usize Vector<T>::count() const {
    return m_values_count;
}

template<typename T>
usize Vector<T>::capacity() const {
    return m_data_capacity;
}

template<typename T>
T* Vector<T>::data() {
    return m_data_storage;
}

template<typename T>
T const* Vector<T>::data() const {
    return m_data_storage;
}

template<typename T>
bool Vector<T>::is_empty() const {
    return m_values_count == 0;
}

template<typename T>
bool Vector<T>::any() const {
    return !is_empty();
}

} /* namespace TC::Collection */