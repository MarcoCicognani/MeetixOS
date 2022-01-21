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

#include <TC/Collection/Vector.hh>
#include <TC/Functional/ErrorOr.hh>
#include <TC/Functional/Option.hh>
#include <TC/Trait/TypeIntrinsics.hh>

namespace TC::Collection {

template<typename K, typename T>
class Map {
public:
    /**
     * @brief Constructors
     */
    explicit Map();
    Map(Map const&)     = default;
    Map(Map&&) noexcept = default;

    ~Map() = default;

    /**
     * @brief Clears this map
     */
    void clear();

    /**
     * @brief Inserts a new pair if doesn't exists or update it
     */
    T& insert(K const& key, T const& value);
    T& insert(K&& key, T&& value);

    /**
     * @brief Erases from this map the given key or alle the keys related to the given value
     */
    Functional::ErrorOr<void>  erase_key(K const& key);
    Functional::ErrorOr<usize> erase_value(T const& value);

    /**
     * @brief Returns the value by the given key, inserts a new one if the doesn't exists
     */
    T& at(K const& key);
    T& operator[](K const& key);

    /**
     * @brief Returns whether this map has the given key or the given value
     */
    [[nodiscard]] bool has_key(K const& key) const;
    [[nodiscard]] bool has_value(T const& value) const;

private:
    struct InnerItem {
        usize m_hash{};
        K     m_key{};
        T     m_value{};
    };

    Vector<InnerItem>&             vector_by_hash(usize hash);
    Functional::Option<InnerItem*> item_by_key(K const& key);
    Functional::Option<InnerItem*> item_by_key(K const& key, usize hash);

private:
    Vector<Vector<InnerItem>> m_tree{};
    usize                     m_values_count{ 0 };
};

template<typename K, typename T>
Map<K, T>::Map() {
    m_tree.resize(256);
}

template<typename K, typename T>
void Map<K, T>::clear() {
    m_tree.clear();
}

template<typename K, typename T>
T& Map<K, T>::insert(const K& key, const T& value) {
    return insert(K{ key }, T{ value });
}

template<typename K, typename T>
T& Map<K, T>::insert(K&& key, T&& value) {
    auto key_hash          = Trait::TypeIntrinsics<K>::hash(key);
    auto inner_item_or_not = item_by_key(key, key_hash);

    if ( inner_item_or_not.is_present() ) {
        auto* inner_item    = inner_item_or_not.unwrap();
        inner_item->m_value = std::move(value);

        return inner_item->m_value;
    } else {
        auto& sub_vector = vector_by_hash(key_hash);
        sub_vector.append(InnerItem{ key_hash, std::move(key) });

        return sub_vector.last().m_value;
    }
}

template<typename K, typename T>
Functional::ErrorOr<void> Map<K, T>::erase_key(const K& key) {
    auto key_hash = Trait::TypeIntrinsics<K>::hash(key);

    TRY(vector_by_hash(key_hash).erase_all_matches(
        [&](auto const& inner_item) { return inner_item.m_hash == key_hash && inner_item.m_key == key; }));
    return {};
}

template<typename K, typename T>
Functional::ErrorOr<usize> Map<K, T>::erase_value(const T& value) {
    usize removed_keys = 0;
    for ( auto& sub_vector : m_tree ) {
        auto removed_keys_or_error
            = sub_vector.erase_all_matches([&value](auto const& inner_item) { return inner_item.m_value == value; });
        if ( removed_keys_or_error.is_value() )
            removed_keys += removed_keys_or_error.unwrap_value();
    }

    if ( removed_keys == 0 )
        return ENOENT;
    else
        return removed_keys;
}

template<typename K, typename T>
T& Map<K, T>::at(const K& key) {
    auto key_hash          = Trait::TypeIntrinsics<K>::hash(key);
    auto inner_item_or_not = item_by_key(key, key_hash);

    if ( inner_item_or_not.is_present() )
        return inner_item_or_not.unwrap()->m_value;
    else
        return insert(key, T{});
}

template<typename K, typename T>
T& Map<K, T>::operator[](const K& key) {
    return at(key);
}

template<typename K, typename T>
bool Map<K, T>::has_key(const K& key) const {
    return item_by_key(key).is_present();
}

template<typename K, typename T>
bool Map<K, T>::has_value(const T& value) const {
    for ( auto const& sub_vector : m_tree ) {
        for ( auto const& item : sub_vector ) {
            if ( item.m_value == value )
                return true;
        }
    }
    return false;
}

template<typename K, typename T>
Vector<typename Map<K, T>::InnerItem>& Map<K, T>::vector_by_hash(usize hash) {
    return m_tree.at(hash % m_tree.count());
}

template<typename K, typename T>
Functional::Option<typename Map<K, T>::InnerItem*> Map<K, T>::item_by_key(const K& key) {
    return item_by_key(key, Trait::TypeIntrinsics<K>::hash(key));
}

template<typename K, typename T>
Functional::Option<typename Map<K, T>::InnerItem*> Map<K, T>::item_by_key(const K& key, usize hash) {
    for ( auto const& inner_item : vector_by_hash(hash) ) {
        if ( inner_item.m_hash == hash && inner_item.m_key == key )
            return &inner_item;
    }
    return {};
}

} /* namespace TC::Collection */