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

#include <LibTC/Collection/Enums/InsertResult.hh>
#include <LibTC/Collection/Enums/KeepStorageCapacity.hh>
#include <LibTC/Collection/Enums/OnExistingEntry.hh>
#include <LibTC/Collection/Pair.hh>
#include <LibTC/Collection/Set.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Trait/TypeIntrinsics.hh>

namespace TC {
namespace Collection {

template<Hashable K, typename T, bool IsOrdered>
class BaseMap {
private:
    using MapSet = BaseSet<Pair<K, T>, TypeIntrinsics<Pair<K, T>>, IsOrdered>;

public:
    using Iterator      = typename MapSet::Iterator;
    using ConstIterator = typename MapSet::ConstIterator;

public:
    /**
     * @brief Constructors
     */
    BaseMap() = default;
    explicit BaseMap(usize capacity)
        : m_hash_set{ capacity } {
    }
    BaseMap(BaseMap const&)     = default;
    BaseMap(BaseMap&&) noexcept = default;
    BaseMap(std::initializer_list<Pair<K, T>> initializer_list)
        : m_hash_set{ initializer_list } {
    }

    ~BaseMap() = default;

    BaseMap& operator=(BaseMap const&)     = default;
    BaseMap& operator=(BaseMap&&) noexcept = default;
    BaseMap& operator=(std::initializer_list<Pair<K, T>> initializer_list) {
        BaseMap map{ initializer_list };
        swap(map);
        return *this;
    }

    /**
     * @brief Clears this map
     */
    void clear(KeepStorageCapacity keep_storage_capacity = KeepStorageCapacity::Yes) {
        m_hash_set.clear(keep_storage_capacity);
    }

    /**
     * @brief Swaps in O(1) the content of this BaseMap with another
     */
    void swap(BaseMap& rhs) noexcept {
        m_hash_set.swap(rhs.m_hash_set);
    }

    /**
     * @brief Inserts a new pair if doesn't exists or update it
     */
    InsertResult insert(K const& key, T const& value, OnExistingEntry on_existing_entry = OnExistingEntry::Replace) {
        return m_hash_set.insert(Pair{ key, value }, on_existing_entry);
    }
    InsertResult insert(K&& key, T&& value, OnExistingEntry on_existing_entry = OnExistingEntry::Replace) {
        return m_hash_set.insert(Pair{ move(key), move(value) }, on_existing_entry);
    }

    ErrorOr<InsertResult> try_insert(K const& key, T const& value, OnExistingEntry on_existing_entry = OnExistingEntry::Replace) {
        return m_hash_set.try_insert(Pair{ key, value }, on_existing_entry);
    }
    ErrorOr<InsertResult> try_insert(K&& key, T&& value, OnExistingEntry on_existing_entry = OnExistingEntry::Replace) {
        return m_hash_set.try_insert(Pair{ move(key), move(value) }, on_existing_entry);
    }

    /**
     * @brief Returns the pair referenced by the given key if exists
     */
    Option<Pair<K, T>&> find(K const& key) {
        return m_hash_set.find(TypeIntrinsics<K>::hash(key),
                               [&key](auto const& pair) { return TypeIntrinsics<K>::equals(pair.key(), key); });
    }
    Option<Pair<K, T> const&> find(K const& key) const {
        return m_hash_set.find(TypeIntrinsics<K>::hash(key),
                               [&key](auto const& pair) { return TypeIntrinsics<K>::equals(pair.key(), key); });
    }

    /**
     * @brief Returns the value by the given key if exists
     */
    Option<T&> at(K const& key) {
        auto pair_or_none = find(key);
        if ( pair_or_none.is_present() )
            return pair_or_none.value().value();
        else
            return {};
    }
    Option<T const&> at(K const& key) const {
        auto pair_or_none = find(key);
        if ( pair_or_none.is_present() )
            return pair_or_none.value().value();
        else
            return {};
    }

    Option<T&> operator[](K const& key) {
        return at(key);
    }
    Option<T const&> operator[](K const& key) const {
        return at(key);
    }

    /**
     * @brief Returns whether this map has the given key or the given value
     */
    [[nodiscard]] bool has_key(K const& key) const {
        return find(key).is_present();
    }
    [[nodiscard]] bool has_value(T const& value) const {
        for ( auto const& pair : *this ) {
            if ( TypeIntrinsics<T>::equals(pair.value(), value) )
                return true;
        }
        return false;
    }

    /**
     * @brief Erases from this map the given key or alle the keys related to the given value
     */
    bool remove(K const& key) {
        auto pair_or_none = find(key);
        if ( pair_or_none.is_present() ) {
            m_hash_set.remove(pair_or_none.value());
            return true;
        } else
            return false;
    }
    template<typename CallBack>
    usize remove_all_matching(CallBack call_back) {
        return m_hash_set.remove_all_matching([&call_back](auto& pair) { return call_back(pair.key(), pair.value()); });
    }

    /**
     * @brief Allocates new capacity for this set for at least the given capacity
     */
    void ensure_capacity(usize capacity) {
        m_hash_set.ensure_capacity(capacity);
    }
    ErrorOr<void> try_ensure_capacity(usize capacity) {
        return m_hash_set.try_ensure_capacity(capacity);
    }

    /**
     * @brief for-each support
     */
    Iterator begin() {
        return m_hash_set.begin();
    }
    Iterator end() {
        return m_hash_set.end();
    }

    ConstIterator begin() const {
        return m_hash_set.begin();
    }
    ConstIterator end() const {
        return m_hash_set.end();
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] usize count() const {
        return m_hash_set.count();
    }
    [[nodiscard]] usize capacity() const {
        return m_hash_set.capacity();
    }
    [[nodiscard]] bool is_empty() const {
        return m_hash_set.is_empty();
    }

private:
    MapSet m_hash_set{};
};

template<Hashable K, typename T>
using Map = BaseMap<K, T, false>;

template<Hashable K, typename T>
using OrderedMap = BaseMap<K, T, true>;

} /* namespace Collection */

using Collection::Map;
using Collection::OrderedMap;

} /* namespace TC */
