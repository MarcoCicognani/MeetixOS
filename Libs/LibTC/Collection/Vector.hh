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
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename Collection, typename T>
class VectorIterator {
public:
    /**
     * @brief Construction functions
     */
    static VectorIterator begin(Collection& collection);
    static VectorIterator end(Collection& collection);

    VectorIterator(VectorIterator const&) = default;

    VectorIterator& operator=(VectorIterator const&) = default;

    /**
     * @brief Increment operators
     */
    VectorIterator& operator++();
    VectorIterator  operator++(int);

    /**
     * @brief ValueReference access operators
     */
    T&       operator*();
    T const& operator*() const;

    /**
     * @brief Pointer access operators
     */
    T*       operator->();
    T const* operator->() const;

    /**
     * @brief Getters
     */
    [[nodiscard]] bool  is_end() const;
    [[nodiscard]] usize index() const;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(VectorIterator const& rhs) const;
    [[nodiscard]] bool operator!=(VectorIterator const& rhs) const;
    [[nodiscard]] bool operator<(VectorIterator const& rhs) const;
    [[nodiscard]] bool operator>(VectorIterator const& rhs) const;
    [[nodiscard]] bool operator<=(VectorIterator const& rhs) const;
    [[nodiscard]] bool operator>=(VectorIterator const& rhs) const;

private:
    VectorIterator(Collection& collection, usize index);

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

    enum AdoptTag { Adopt };

public:
    /**
     * @brief Constructors
     */
    explicit Vector() = default;
    explicit Vector(usize capacity);
    explicit Vector(AdoptTag, T* data_storage, usize size);

    Vector(Vector const& rhs);
    Vector(Vector&& rhs) noexcept;
    Vector(std::initializer_list<T> initializer_list);

    ~Vector();

    Vector& operator=(Vector const& rhs);
    Vector& operator=(Vector&& rhs) noexcept;
    Vector& operator=(std::initializer_list<T> initializer_list);

    /**
     * @brief Destroys all the stored values keeping the capacity of this vector
     */
    void clear(KeepStorageCapacity keep_storage_capacity = KeepStorageCapacity::Yes);
    void swap(Vector& rhs) noexcept;

    /**
     * @brief Inserts the given <value> to the given <index>
     */
    void          insert_at(usize index, T const& value);
    void          insert_at(usize index, T&& value);
    ErrorOr<void> try_insert_at(usize index, T const& value);
    ErrorOr<void> try_insert_at(usize index, T&& value);

    /**
     * @brief Inserts the given <value> sorted
     */
    template<typename Comparator>
    void insert_sorted(T const& value, Comparator comparator);
    void insert_sorted(T const& value);
    template<typename Comparator>
    ErrorOr<void> try_insert_sorted(T const& value, Comparator comparator);
    ErrorOr<void> try_insert_sorted(T const& value);

    /**
     * @brief Pushes the given <value> to the begin of the vector
     */
    void          prepend(T const& value);
    void          prepend(T&& value);
    ErrorOr<void> try_prepend(T const& value);
    ErrorOr<void> try_prepend(T&& value);

    /**
     * @brief Pushes the given <value> to the end of the vector
     * @param value
     * @return
     */
    void          append(T const& value);
    void          append(T&& value);
    ErrorOr<void> try_append(T const& value);
    ErrorOr<void> try_append(T&& value);

    void append_unchecked(T const& value);
    void append_unchecked(T&& value);

    /**
     * @brief Pushes a new value to the begin of the vector constructing it with the given arguments
     */
    template<typename... Args>
    void emplace_first(Args&&... args);
    template<typename... Args>
    ErrorOr<void> try_emplace_first(Args&&... args);

    /**
     * @brief Pushes a new value to the end of the vector constructing it with the given arguments
     */
    template<typename... Args>
    void emplace_last(Args&&... args);
    template<typename... Args>
    ErrorOr<void> try_emplace_last(Args&&... args);

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
    ErrorOr<void>  erase_at(usize index);
    ErrorOr<void>  erase_first_of(T const& value);
    ErrorOr<usize> erase_all_of(T const& value);
    template<typename CallBack>
    ErrorOr<usize> erase_all_matches(CallBack matches);

    /**
     * @brief Sorts the vector content using the given comparator
     */
    template<typename Comparator>
    void sort(Comparator comparator);

    /**
     * @brief Resizes the amount of elements inside this vector creating (by default constructor) if new_count >
     * count(), destroying if new_count < count()
     */
    void          resize(usize new_count);
    ErrorOr<void> try_resize(usize new_count);

    /**
     * @brief Allocates new capacity for this vector for at least the given capacity
     */
    void          ensure_capacity(usize capacity);
    ErrorOr<void> try_ensure_capacity(usize capacity);

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
    Option<usize>      index_of(T const& value) const;
    template<typename Callback>
    Option<usize> index_if(Callback callback) const;

    Option<T&>       find(T const& value);
    Option<T const&> find(T const& value) const;

    template<typename Callback>
    Option<T&> find_if(Callback callback);
    template<typename Callback>
    Option<T const&> find_if(Callback callback) const;

    /**
     * @brief Vector data access
     */
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
    T* data_slot(usize index);

private:
    T*    m_data_storage{ nullptr };
    usize m_data_capacity{ 0 };
    usize m_values_count{ 0 };
};

} /* namespace Collection */

using Collection::Vector;

} /* namespace TC */

#include <LibTC/Collection/Vector.hhi>