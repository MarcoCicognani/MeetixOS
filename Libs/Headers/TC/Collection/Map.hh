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
#include <TC/Collection/Pair.hh>
#include <TC/Collection/Vector.hh>
#include <TC/Functional/ErrorOr.hh>
#include <TC/Functional/Option.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename MainContainer, typename InnerContainer, typename T>
class MapIterator {
public:
    /**
     * @brief Construction functions
     */
    static MapIterator begin(MainContainer& main_container);
    static MapIterator end(MainContainer& main_container);

    MapIterator(MapIterator const&) = default;

    MapIterator& operator=(MapIterator const&) = default;

    /**
     * @brief Increment operators
     */
    MapIterator& operator++();
    MapIterator  operator++(int);

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
    [[nodiscard]] bool is_end() const;

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(MapIterator const& rhs) const;
    [[nodiscard]] bool operator!=(MapIterator const& rhs) const;
    [[nodiscard]] bool operator<=>(MapIterator const&) const = default;

private:
    MapIterator(MainContainer&                    main_container,
                usize                             first_level_index,
                typename InnerContainer::Iterator nested_iterator);

private:
    MainContainer&                    m_main_container;
    usize                             m_first_level_index{ 0 };
    typename InnerContainer::Iterator m_nested_iterator{};
};

} /* namespace Details */

template<typename K, typename T, bool Ordered = true>
class Map {
public:
    using Iterator      = Details::MapIterator<Map, Vector<Pair<K, T>>, Pair<K, T>>;
    using ConstIterator = Details::MapIterator<Map const, Vector<Pair<K, T>> const, Pair<K, T> const>;

    enum class OnExistingKey {
        Update,
        KeepOldValue,
    };

public:
    /**
     * @brief Constructors
     */
    explicit Map();
    explicit Map(usize bucket_count);
    Map(Map const& rhs);
    Map(Map&& rhs) noexcept;
    Map(std::initializer_list<Pair<K, T>> initializer_list);

    ~Map() = default;

    Map& operator=(Map const&);
    Map& operator=(Map&&) noexcept;

    Map& operator=(std::initializer_list<Pair<K, T>> initializer_list);

    /**
     * @brief Clears this map
     */
    void clear();
    void swap(Map& rhs) noexcept;

    /**
     * @brief Inserts a new pair if doesn't exists or update it
     */
    Option<T> insert(K const& key, T const& value, OnExistingKey on_existing_key = OnExistingKey::Update);
    Option<T> insert(K&& key, T&& value, OnExistingKey on_existing_key = OnExistingKey::Update);

    ErrorOr<Option<T>> try_insert(K const& key, T const& value, OnExistingKey on_existing_key = OnExistingKey::Update);
    ErrorOr<Option<T>> try_insert(K&& key, T&& value, OnExistingKey on_existing_key = OnExistingKey::Update);

    /**
     * @brief Returns the value by the given key, inserts a new one if the doesn't exists
     */
    Option<T&> at(K const& key);
    Option<T&> operator[](K const& key);

    /**
     * @brief Erases from this map the given key or alle the keys related to the given value
     */
    ErrorOr<void>  erase_key(K const& key);
    ErrorOr<usize> erase_value(T const& value);

    /**
     * @brief Returns whether this map has the given key or the given value
     */
    [[nodiscard]] bool has_key(K const& key) const;
    [[nodiscard]] bool has_value(T const& value) const;

    /**
     * @brief for-each support
     */
    Iterator begin();
    Iterator end();

    ConstIterator begin() const;
    ConstIterator end() const;

    /**
     * @brief Returns the size of this map
     */
    [[nodiscard]] usize count() const;
    [[nodiscard]] bool  is_empty() const;
    [[nodiscard]] usize capacity() const;

    [[nodiscard]] Vector<Vector<Pair<K, T>>>&       data();
    [[nodiscard]] Vector<Vector<Pair<K, T>>> const& data() const;

private:
    Vector<Pair<K, T>>&       bucket_by_hash(usize key_hash);
    Vector<Pair<K, T>> const& bucket_by_hash(usize key_hash) const;
    Option<Pair<K, T>&>       find_pair_by_key(K const& key);
    Option<Pair<K, T> const&> find_pair_by_key(K const& key) const;

private:
    Vector<Vector<Pair<K, T>>> m_buckets_storage{};
    usize                      m_values_count{ 0 };
};

} /* namespace Collection */

using Collection::Map;

} /* namespace TC */

#include <../LibTC/Collection/Map.hhi>