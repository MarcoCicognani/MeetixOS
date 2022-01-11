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

#include <Api/StdInt.h>
#include <assert.h>
#include <stdlib.h>
#include <TC/Collections/CommonIterator.hh>
#include <TC/Std/Exchange.hh>
#include <TC/Std/Forward.hh>
#include <TC/Std/InitializerList.hh>
#include <TC/Std/Move.hh>
#include <TC/Std/New.hh>
#include <TC/Std/Swap.hh>
#include <TC/Tags/Adopt.hh>

namespace TC::Collections {

template<typename T>
class Vector {
public:
    using Iterator      = CommonIterator<Vector, T>;
    using ConstIterator = CommonIterator<Vector const, T const>;

public:
    /**
     * @brief Constructors
     */
    explicit Vector();
    explicit Vector(usize capacity);
    explicit Vector(TC::Tags::Adopt, T* data_storage, usize size);

    Vector(Vector const& rhs);
    Vector(Vector&& rhs) noexcept;
    Vector(std::initializer_list<T> initializer_list);

    ~Vector();

    /**
     * @brief Destroys all the stored values keeping the capacity of this vector
     */
    void clear();

    /**
     * @brief Returns the reference to the element at the given <index>
     */
    T&       at(usize index);
    T const& at(usize index) const;

    /**
     * @brief Returns the reference to the element at the given <index>
     */
    T&       operator[](usize index);
    T const& operator[](usize index) const;

    /**
     * @brief Inserts the given <value> to the given <index>
     */
    T& insert(usize index, T const& value);
    T& insert(usize index, T&& value);

    /**
     * @brief Inserts the given <value> sorted
     */
    template<typename Comparator>
    void insert_sorted(T const& value, Comparator comparator);
    void insert_sorted(T const& value);

    /**
     * @brief Removes and returns the value at the index
     */
    T take_at(usize index);

    /**
     * @brief Pushes the given <value> to the begin of the vector
     */
    T& push(T const& value);
    T& push(T&& value);

    /**
     * @brief Pushes the given <value> to the end of the vector
     * @param value
     * @return
     */
    T& push_back(T const& value);
    T& push_back(T&& value);

    /**
     * @brief Pushes a new value to the begin of the vector constructing it with the given arguments
     */
    template<typename... Args>
    T& emplace(Args&&... args);

    /**
     * @brief Pushes a new value to the end of the vector constructing it with the given arguments
     */
    template<typename... Args>
    T& emplace_back(Args&&... args);

    /**
     * @brief Returns a reference to the first element
     */
    T& peek() const;

    /**
     * @brief Returns a reference to the last element
     */
    T& peek_back() const;

    /**
     * @brief Removes the first element
     */
    T pop();

    /**
     * @brief Removes the last element
     */
    T pop_back();

    /**
     * @brief Removes the element(s)
     */
    void erase_at(usize index);
    void erase_first_of(T const& value);
    void erase_all_of(T const& value);
    template<typename CallBack>
    void erase_all_matches(CallBack is_match);

    /**
     * @brief Sorts the vector content using the given comparator
     */
    template<typename Comparator>
    void sort(Comparator comparator);

    /**
     * @brief Resizes the content of this vector, allocating new values (and initializing them with default constructor)
     * if the <new_count> exceed the capacity, or destroying them if <new_count> is less
     */
    void resize(usize new_count);
    void ensure_capacity(usize capacity);
    void grow();
    void shrink();

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
    [[nodiscard]] bool contains(T const& value) const;

    [[nodiscard]] usize count() const;
    [[nodiscard]] usize capacity() const;

    [[nodiscard]] bool is_empty() const;
    [[nodiscard]] bool any() const;

private:
    void                copy_content(T const* source, usize count) const;
    void                move_content_to(T* new_storage, usize count);
    [[nodiscard]] usize max(usize a, usize b) const;

private:
    T*    m_data_storage{ nullptr };
    usize m_capacity{ 0 };
    usize m_count{ 0 };
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
Vector<T>::Vector(TC::Tags::Adopt, T* data_storage, usize size)
    : m_data_storage{ data_storage }
    , m_capacity{ size }
    , m_count{ size } {
}

template<typename T>
Vector<T>::Vector(Vector const& rhs)
    : m_count{ rhs.m_count } {
    ensure_capacity(rhs.count());
    copy_content(rhs.m_data_storage, m_count);
}

template<typename T>
Vector<T>::Vector(Vector&& rhs) noexcept
    : m_data_storage{ std::exchange(rhs.m_data_storage, nullptr) }
    , m_capacity{ std::exchange(rhs.m_capacity, 0) }
    , m_count{ std::exchange(rhs.m_count, 0) } {
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> initializer_list) {
    ensure_capacity(initializer_list.size());

    for ( auto const& element : initializer_list )
        push_back(element);
}

template<typename T>
Vector<T>::~Vector() {
    clear();

    free(m_data_storage);
}

template<typename T>
void Vector<T>::clear() {
    if ( m_data_storage != nullptr ) {
        for ( auto i = 0; i < m_count; ++i )
            m_data_storage[i].~T();
    }

    m_count = 0;
}

template<typename T>
T& Vector<T>::at(usize index) {
    assert(index < m_count);

    return m_data_storage[index];
}

template<typename T>
T const& Vector<T>::at(usize index) const {
    assert(index < m_count);

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
T& Vector<T>::insert(usize index, T const& value) {
    return insert(index, T{ value });
}

template<typename T>
T& Vector<T>::insert(usize index, T&& value) {
    assert(index <= m_count);

    grow();

    /* move forward the values of one place in memory */
    for ( auto i = m_count - 1; i > index; --i ) {
        new (&m_data_storage[i]) T{ std::move(m_data_storage[i - 1]) };
        at(i - 1).~T();
    }

    /* insert the given value */
    return *new (&m_data_storage[index]) T{ std::move(value) };
}

template<typename T>
template<typename Comparator>
void Vector<T>::insert_sorted(T const& value, Comparator comparator) {
    /* find the insertion index */
    auto insert_index = 0;
    for ( auto i = 0; i < m_count; ++i ) {
        if ( comparator(m_data_storage[i], value) )
            insert_index = i;
        else
            break;
    }

    /* insert at the position */
    insert(insert_index, value);
}

template<typename T>
void Vector<T>::insert_sorted(T const& value) {
    return insert_sorted(value, [](T const& a, T const& b) { return a < b; });
}

template<typename T>
T Vector<T>::take_at(usize index) {
    assert(index < m_count);

    T value{ std::move(m_data_storage[index]) };
    erase_at(index);
    return value;
}
template<typename T>
T& Vector<T>::push(const T& value) {
    return push(T{ value });
}
template<typename T>
T& Vector<T>::push(T&& value) {
    return insert(0, std::move(value));
}

template<typename T>
T& Vector<T>::push_back(T const& value) {
    return push_back(T{ value });
}

template<typename T>
T& Vector<T>::push_back(T&& value) {
    return insert(m_count, std::move(value));
}

template<typename T>
template<typename... Args>
T& Vector<T>::emplace(Args&&... args) {
    grow();

    /* move forward the values of one place in memory */
    for ( auto i = m_count - 1; i > 0; --i ) {
        new (&m_data_storage[i]) T{ std::move(m_data_storage[i - 1]) };
        at(i - 1).~T();
    }

    /* insert the given value */
    new (&m_data_storage[0]) T{ std::forward<Args>(args)... };
    return m_data_storage[0];
}

template<typename T>
template<typename... Args>
T& Vector<T>::emplace_back(Args&&... args) {
    grow();

    /* insert the given value */
    new (&m_data_storage[m_count - 1]) T{ std::forward<Args>(args)... };
    return m_data_storage[m_count - 1];
}

template<typename T>
T& Vector<T>::peek() const {
    return at(0);
}

template<typename T>
T& Vector<T>::peek_back() const {
    return at(m_count - 1);
}

template<typename T>
T Vector<T>::pop() {
    assert(any());

    T value{ std::move(m_data_storage[0]) };
    erase_at(0);

    return value;
}

template<typename T>
T Vector<T>::pop_back() {
    assert(any());

    T value{ std::move(m_data_storage[m_count - 1]) };
    erase_at(m_count - 1);

    return value;
}

template<typename T>
void Vector<T>::erase_at(usize index) {
    assert(index < m_count);

    /* destroy the value at the given index */
    m_data_storage[index].~T();

    /* shift all the values one position back */
    for ( auto i = index; i < m_count - 1; ++i ) {
        new (&m_data_storage[i]) T{ (std::move(m_data_storage[i + 1])) };
        m_data_storage[i + 1].~T();
    }

    /* free memory if possible */
    shrink();
}

template<typename T>
void Vector<T>::erase_first_of(T const& value) {
    for ( auto i = 0; i < m_count; ++i ) {
        if ( m_data_storage[i] == value ) {
            erase_at(i);
            return;
        }
    }
}

template<typename T>
void Vector<T>::erase_all_of(T const& value) {
    for ( auto i = 0; i < m_count; ) {
        if ( m_data_storage[i] == value )
            erase_at(i);
        else
            ++i;
    }
}

template<typename T>
template<typename CallBack>
void Vector<T>::erase_all_matches(CallBack is_match) {
    for ( auto i = 0; i < m_count; ) {
        if ( is_match(m_data_storage[i]) )
            erase_at(i);
        else
            ++i;
    }
}

template<typename T>
template<typename Comparator>
void Vector<T>::sort(Comparator comparator) {
    for ( auto i = 0; i + 1 < m_count; ++i ) {
        for ( auto j = i + 1; j < m_count; ++j ) {
            if ( comparator(m_data_storage[i], m_data_storage[j]) > 0 )
                std::swap(m_data_storage[i], m_data_storage[j]);
        }
    }
}

template<typename T>
void Vector<T>::resize(usize new_count) {
    ensure_capacity(new_count);

    if ( m_count < new_count ) {
        /* extend the data with default value */
        for ( auto i = m_count; i < new_count; ++i )
            new (&m_data_storage[i]) T{};
    } else if ( m_count > new_count ) {
        /* shrink the data destroying exceeding values */
        for ( auto i = new_count; i < m_count; ++i )
            m_data_storage[i].~T();
    }

    m_count = new_count;
}

template<typename T>
void Vector<T>::ensure_capacity(usize capacity) {
    capacity = max(capacity, 16);

    /* allocate the storage if not already done */
    if ( m_data_storage == nullptr ) {
        m_data_storage = reinterpret_cast<T*>(calloc(capacity, sizeof(T)));
        m_capacity     = capacity;
        m_count        = 0;

        return;
    }

    /* do not waste time allocating less memory */
    capacity = max(capacity, m_count);
    if ( capacity < m_capacity )
        return;

    /* increase the capacity logarithmically */
    auto new_capacity = m_capacity;
    while ( new_capacity <= capacity )
        new_capacity += new_capacity / 4;

    /* move the storage to the new memory area */
    auto new_storage = reinterpret_cast<T*>(calloc(new_capacity, sizeof(T)));
    move_content_to(new_storage, m_count);

    /* update the fields */
    free(m_data_storage);
    m_data_storage = new_storage;
    m_capacity     = capacity;
}

template<typename T>
void Vector<T>::grow() {
    if ( m_count + 1 >= m_capacity ) {
        /* move the storage to the new memory area */
        auto new_capacity = m_capacity + m_capacity / 4;
        auto new_storage  = reinterpret_cast<T*>(calloc(new_capacity, sizeof(T)));
        move_content_to(new_storage, m_count);

        /* update the fields */
        free(m_data_storage);
        m_data_storage = new_storage;
        m_capacity     = new_capacity;
    }

    ++m_count;
}

template<typename T>
void Vector<T>::shrink() {
    --m_count;

    auto new_capacity = m_capacity - m_capacity / 4;
    if ( max(m_count, 16) < new_capacity ) {
        /* move the storage to the new memory area */
        auto new_storage = reinterpret_cast<T*>(calloc(new_capacity, sizeof(T)));
        move_content_to(new_storage, m_count);

        /* update the fields */
        free(m_data_storage);
        m_data_storage = new_storage;
        m_capacity     = new_capacity;
    }
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
    for ( auto i = 0; i < m_count; ++i ) {
        if ( m_data_storage[i] == value )
            return true;
    }
    return false;
}

template<typename T>
usize Vector<T>::count() const {
    return m_count;
}

template<typename T>
usize Vector<T>::capacity() const {
    return m_capacity;
}

template<typename T>
bool Vector<T>::is_empty() const {
    return m_count == 0;
}

template<typename T>
bool Vector<T>::any() const {
    return !is_empty();
}

template<typename T>
void Vector<T>::copy_content(T const* source, usize count) const {
    for ( auto i = 0; i < count; ++i )
        new (&m_data_storage[i]) T{ source[i] };
}

template<typename T>
void Vector<T>::move_content_to(T* new_storage, usize count) {
    for ( auto i = 0; i < count; ++i )
        new (&new_storage[i]) T{ std::move(m_data_storage[i]) };
}

template<typename T>
usize Vector<T>::max(usize a, usize b) const {
    if ( a > b )
        return a;
    else
        return b;
}

} /* namespace TC::Collections */