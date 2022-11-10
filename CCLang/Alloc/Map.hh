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

#include <CCLang/Forward.hh>

#include <CCLang/Alloc/Set.hh>
#include <CCLang/Core/AnyOf.hh>
#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/TypeTraits.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/IntTypes.hh>

using MapReplaceExisting = Details::ReplaceExisting;
using MapInsertResult    = Details::InsertResult;

template<typename K, typename T, typename KTraits, bool IsOrdered>
class Map final {
    TCDenyCopy$(Map);

private:
    struct KeyValue final {
        K m_key;
        T m_value;

        auto try_clone() const -> ErrorOr<KeyValue> {
            if constexpr ( TryCloneable<K, ErrorOr<K>> && TryCloneable<T, ErrorOr<T>> ) {
                return KeyValue{ try$(m_key.try_clone()), try$(m_value.try_clone()) };
            } else if constexpr ( TryCloneable<K, ErrorOr<K>> && Cloneable<T> ) {
                return KeyValue{ try$(m_key.try_clone()), m_value.clone() };
            } else if constexpr ( TryCloneable<K, ErrorOr<K>> && CopyConstructible<T> ) {
                return KeyValue{ try$(m_key.try_clone()), m_value };
            } else if constexpr ( Cloneable<K> && TryCloneable<T, ErrorOr<T>> ) {
                return KeyValue{ m_key.clone(), try$(m_value.try_clone()) };
            } else if constexpr ( Cloneable<K> && Cloneable<T> ) {
                return KeyValue{ m_key.clone(), m_value.clone() };
            } else if constexpr ( Cloneable<K> && CopyConstructible<T> ) {
                return KeyValue{ m_key.clone(), m_value };
            } else if constexpr ( CopyConstructible<K> && TryCloneable<T, ErrorOr<T>> ) {
                return KeyValue{ m_key, try$(m_value.try_clone()) };
            } else if constexpr ( CopyConstructible<K> && Cloneable<T> ) {
                return KeyValue{ m_key, m_value.clone() };
            } else if constexpr ( CopyConstructible<K> && CopyConstructible<T> ) {
                return KeyValue{ m_key, m_value };
            }
        }
    };

    struct KeyValueTraits final {
        static constexpr auto equals(KeyValue const& a, KeyValue const& b) -> bool {
            return KTraits::equals(a.m_key, b.m_key);
        }

        static constexpr auto hash(KeyValue const& pair) -> usize {
            return KTraits::hash(pair.m_key);
        }

        static constexpr auto is_trivial() -> bool {
            return KTraits::is_trivial() && TypeTraits<T>::is_trivial();
        }
    };

public:
    using Iterator                    = typename Set<KeyValue, KeyValueTraits, IsOrdered>::Iterator;
    using ConstIterator               = typename Set<KeyValue, KeyValueTraits, IsOrdered>::ConstIterator;
    using ReverseIterator             = typename Set<KeyValue, KeyValueTraits, IsOrdered>::ReverseIterator;
    using ConstReverseIterator        = typename Set<KeyValue, KeyValueTraits, IsOrdered>::ConstReverseIterator;
    using ReverseIteratorWrapper      = typename Set<KeyValue, KeyValueTraits, IsOrdered>::ReverseIteratorWrapper;
    using ConstReverseIteratorWrapper = typename Set<KeyValue, KeyValueTraits, IsOrdered>::ConstReverseIteratorWrapper;

public:
    /**
     * @brief Non-Error safe factory functions
     */
    [[nodiscard]]
    static constexpr auto new_empty() -> Map<K, T, KTraits, IsOrdered> {
        return Map<K, T, KTraits, IsOrdered>{};
    }
    [[nodiscard]]
    static auto new_with_capacity(usize capacity) -> Map<K, T, KTraits, IsOrdered> {
        return must$(try_new_with_capacity(capacity));
    }
    [[nodiscard]]
    static auto new_from_other(Map<K, T, KTraits, IsOrdered> const& rhs) -> Map<K, T, KTraits, IsOrdered> {
        return must$(try_new_from_other(rhs));
    }
    [[nodiscard]]
    static auto new_from_list(Cxx::InitializerList<KeyValue> initializer_list) -> Map<K, T, KTraits, IsOrdered> {
        return must$(try_new_from_list(initializer_list));
    }

    /**
     * @brief Error safe Factory functions
     */
    static auto try_new_with_capacity(usize capacity) -> ErrorOr<Map<K, T, KTraits, IsOrdered>> {
        auto map = new_empty();
        try$(map.try_ensure_capacity(capacity));
        return map;
    }
    static auto try_new_from_other(Map<K, T, KTraits, IsOrdered> const& rhs) -> ErrorOr<Map<K, T, KTraits, IsOrdered>> {
        auto map = try$(try_new_with_capacity(rhs.count()));
        for ( auto const& key_value : rhs )
            try$(map.try_insert(try$(key_value.try_clone())));

        return map;
    }
    static auto try_new_from_list(Cxx::InitializerList<KeyValue> initializer_list) -> ErrorOr<Map<K, T, KTraits, IsOrdered>> {
        auto map = new_empty();
        for ( auto const& key_value : initializer_list ) /* even with auto initializer_list exposes only T const& */
            try$(map.try_insert(Cxx::move(const_cast<KeyValue&>(key_value))));

        return map;
    }

    /**
     * @brief Move constructor and move assignment
     */
    Map(Map<K, T, KTraits, IsOrdered>&&)                                              = default;
    auto operator=(Map<K, T, KTraits, IsOrdered>&&) -> Map<K, T, KTraits, IsOrdered>& = default;

    /**
     * @brief Deep cloning
     */
    [[nodiscard]]
    auto clone() const -> Map<K, T, KTraits, IsOrdered> {
        return must$(try_clone());
    }
    auto try_clone() const -> ErrorOr<Map<K, T, KTraits, IsOrdered>> {
        return Map<K, T, KTraits, IsOrdered>::try_new_from_other(*this);
    }

    /**
     * @brief Clears this map
     */
    auto clear() -> void {
        m_hash_set.clear();
    }
    auto clear_keep_capacity() -> void {
        m_hash_set.clear_keep_capacity();
    }

    /**
     * @brief Swaps in O(1) the content of this BaseMap with another
     */
    auto swap(Map<K, T, KTraits, IsOrdered>& rhs) {
        m_hash_set.swap(rhs.m_hash_set);
    }

    /**
     * @brief Inserts a new pair if doesn't exists or update it
     */
    auto insert(KeyValue key_value, MapReplaceExisting replace_existing = MapReplaceExisting::Yes) -> MapInsertResult {
        return m_hash_set.insert(Cxx::move(key_value), replace_existing);
    }
    auto insert(K key, T value, MapReplaceExisting replace_existing = MapReplaceExisting::Yes) -> MapInsertResult {
        return m_hash_set.insert(KeyValue{ Cxx::move(key), Cxx::move(value) }, replace_existing);
    }

    auto try_insert(KeyValue key_value, MapReplaceExisting replace_existing = MapReplaceExisting::Yes) -> ErrorOr<MapInsertResult> {
        return m_hash_set.insert(Cxx::move(key_value), replace_existing);
    }
    auto try_insert(K key, T value, MapReplaceExisting replace_existing = MapReplaceExisting::Yes) -> ErrorOr<MapInsertResult> {
        return m_hash_set.insert(KeyValue{ Cxx::move(key), Cxx::move(value) }, replace_existing);
    }

    /**
     * @brief Returns the pair referenced by the given key if exists
     */
    auto find(K const& key) -> Option<KeyValue&> {
        return m_hash_set.find(KeyValue{ key, T{} });
    }
    auto find(K const& key) const -> Option<KeyValue const&> {
        return m_hash_set.find(KeyValue{ key, T{} });
    }

    /**
     * @brief Returns the value by the given key if exists
     */
    auto at(K const& key) -> Option<T&> {
        return find(key).template map<T&>([&key](auto& pair) -> T& { return pair.m_value; });
    }
    auto at(K const& key) const -> Option<T const&> {
        return find(key).template map<T const&>([&key](auto const& pair) -> T const& { return pair.m_value; });
    }

    auto operator[](K const& key) -> Option<T&> {
        return at(key);
    }
    auto operator[](K const& key) const -> Option<T const&> {
        return at(key);
    }

    /**
     * @brief Returns whether this map has the given key or the given value
     */
    [[nodiscard]]
    auto has_key(K const& key) const -> bool {
        return find(key).is_present();
    }
    [[nodiscard]]
    auto has_value(T const& value) const -> bool {
        return any_of<KeyValue>(begin(), end(), [&value](auto const& pair) { return TypeTraits<T>::equals(pair.m_value, value); });
    }

    /**
     * @brief Erases from this map the given key or alle the keys related to the given value
     */
    [[nodiscard]]
    auto remove(K const& key) -> bool {
        return find(key)
            .template map<bool>([this](KeyValue const& pair) {
                m_hash_set.remove(pair);
                return true;
            })
            .unwrap_or(false);
    }
    [[nodiscard]]
    auto remove_all_matching(Predicate<KeyValue const&> auto predicate) -> usize {
        return m_hash_set.remove_all_matching([&predicate](auto const& pair) -> bool { return predicate(pair); });
    }

    /**
     * @brief Allocates new capacity for this set for at least the given capacity
     */
    auto ensure_capacity(usize capacity) -> void {
        m_hash_set.ensure_capacity(capacity);
    }
    auto try_ensure_capacity(usize capacity) -> ErrorOr<void> {
        return m_hash_set.try_ensure_capacity(capacity);
    }

    /**
     * @brief for-each support
     */
    auto begin() -> Iterator {
        return m_hash_set.begin();
    }
    auto end() -> Iterator {
        return m_hash_set.end();
    }

    auto begin() const -> ConstIterator {
        return m_hash_set.begin();
    }
    auto end() const -> ConstIterator {
        return m_hash_set.end();
    }

    /**
     * @brief reverse for-each support
     */
    auto rbegin() -> ReverseIterator {
        return m_hash_set.rbegin();
    }
    auto rend() -> ReverseIterator {
        return m_hash_set.rend();
    }

    auto rbegin() const -> ConstReverseIterator {
        return m_hash_set.rbegin();
    }
    auto rend() const -> ConstReverseIterator {
        return m_hash_set.rend();
    }

    auto reverse_iter() -> ReverseIteratorWrapper {
        return m_hash_set.reverse_iter();
    }
    auto reverse_iter() const -> ConstReverseIteratorWrapper {
        return m_hash_set.reverse_iter();
    }

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto count() const -> usize {
        return m_hash_set.count();
    }
    [[nodiscard]]
    auto capacity() const -> usize {
        return m_hash_set.capacity();
    }
    [[nodiscard]]
    auto is_empty() const -> bool {
        return m_hash_set.is_empty();
    }

private:
    explicit constexpr Map() = default;

private:
    Set<KeyValue, KeyValueTraits, IsOrdered> m_hash_set{ Set<KeyValue, KeyValueTraits, IsOrdered>::new_empty() };
};
