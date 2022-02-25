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
#include <LibTC/Collection/Pair.hh>
#include <LibTC/Collection/Vector.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Trait/TypeIntrinsics.hh>

namespace TC {
namespace Collection {
namespace Details {

template<typename MainContainer, typename InnerContainer, typename T>
class MapIterator {
public:
    /**
     * @brief Construction functions
     */
    static MapIterator begin(MainContainer& main_container) {
        usize first_level_index = 0;
        for ( auto const& inner : main_container.data() ) {
            if ( !inner.is_empty() )
                break;

            ++first_level_index;
        }

        return MapIterator{ main_container, first_level_index, main_container.data().at(first_level_index).begin() };
    }
    static MapIterator end(MainContainer& main_container) {
        return MapIterator{ main_container, main_container.data().count() - 1, main_container.data().last().end() };
    }

    MapIterator(MapIterator const&) = default;

    MapIterator& operator=(MapIterator const&) = default;

    /**
     * @brief Increment operators
     */
    MapIterator& operator++() {
        ++m_nested_iterator;
        if ( m_nested_iterator.is_end() ) {
            /* find the next non-empty inner collector */
            while ( ++m_first_level_index < m_main_container.data().count() ) {
                auto& inner = m_main_container.data().at(m_first_level_index);

                /* always assign the iterator even the <inner> is empty, since the operator!= and == of the linear
                 * iterator checks whether the source container is the same, and our end container is the last of
                 * the m_main_container.data() */
                m_nested_iterator = inner.begin();
                if ( !inner.is_empty() )
                    break;
            }
        }

        return *this;
    }
    MapIterator operator++(int) {
        MapIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    T& operator*() {
        return *m_nested_iterator;
    }
    T const& operator*() const {
        return *m_nested_iterator;
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
        return m_nested_iterator.is_end();
    }

    /**
     * @brief Comparison operators
     */
    [[nodiscard]] bool operator==(MapIterator const& rhs) const {
        return m_nested_iterator == rhs.m_nested_iterator;
    }
    [[nodiscard]] bool operator!=(MapIterator const& rhs) const {
        return m_nested_iterator != rhs.m_nested_iterator;
    }
    [[nodiscard]] bool operator<=>(MapIterator const&) const = default;

private:
    MapIterator(MainContainer&                    main_container,
                usize                             first_level_index,
                typename InnerContainer::Iterator nested_iterator)
        : m_main_container{ main_container }
        , m_first_level_index{ first_level_index }
        , m_nested_iterator{ nested_iterator } {
    }

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
    explicit Map()
        : Map{ 128 } {
    }
    explicit Map(usize bucket_count)
        : m_buckets_storage{ bucket_count } {
        m_buckets_storage.resize(bucket_count);
    }
    Map(Map const& rhs)
        : m_buckets_storage{ rhs.m_buckets_storage }
        , m_values_count{ rhs.m_values_count } {
    }
    Map(Map&& rhs) noexcept
        : m_buckets_storage{ move(rhs.m_buckets_storage) }
        , m_values_count{ exchange(rhs.m_values_count, 0) } {
    }
    Map(std::initializer_list<Pair<K, T>> initializer_list)
        : Map{} {
        for ( auto const& pair : initializer_list )
            insert(pair.key(), pair.value());
    }

    ~Map() = default;

    Map& operator=(Map const& rhs) {
        if ( this == &rhs )
            return *this;

        Map map{ rhs };
        swap(map);
        return *this;
    }
    Map& operator=(Map&& rhs) noexcept {
        Map map{ move(rhs) };
        swap(map);
        return *this;
    }

    Map& operator=(std::initializer_list<Pair<K, T>> initializer_list) {
        Map map{ initializer_list };
        swap(map);
        return *this;
    }

    /**
     * @brief Clears this map
     */
    void clear() {
        auto capacity = m_buckets_storage.capacity();
        m_buckets_storage.clear(decltype(m_buckets_storage)::KeepStorageCapacity::Yes);
        m_buckets_storage.resize(capacity);
        m_values_count = 0;
    }

    /**
     * @brief Swaps in O(1) the content of this Map with another
     */
    void swap(Map& rhs) noexcept {
        m_buckets_storage.swap(rhs.m_buckets_storage);
        Cxx::swap(m_values_count, rhs.m_values_count);
    }

    /**
     * @brief Inserts a new pair if doesn't exists or update it
     */
    Option<T> insert(K const& key, T const& value, OnExistingKey on_existing_key = OnExistingKey::Update) {
        return MUST(try_insert(K{ key }, T{ value }, on_existing_key));
    }
    Option<T> insert(K&& key, T&& value, OnExistingKey on_existing_key = OnExistingKey::Update) {
        return MUST(try_insert(move(key), move(value), on_existing_key));
    }

    ErrorOr<Option<T>> try_insert(K const& key, T const& value, OnExistingKey on_existing_key = OnExistingKey::Update) {
        return try_insert(K{ key }, T{ value }, on_existing_key);
    }
    ErrorOr<Option<T>> try_insert(K&& key, T&& value, OnExistingKey on_existing_key = OnExistingKey::Update) {
        auto pair_or_none = find_pair_by_key(key);
        if ( pair_or_none.is_present() ) {
            if ( on_existing_key == OnExistingKey::KeepOldValue )
                return Option<T>{};
            else {
                auto& pair_ref  = pair_or_none.unwrap();
                auto& value_ref = pair_ref.value();

                /* overwrite the value and save the old one */
                T old_value{ move(value_ref) };
                new (&value_ref) T{ move(value) };

                return Option<T>{ old_value };
            }
        } else {
            auto& sub_vector = bucket_by_hash(Trait::TypeIntrinsics<K>::hash(key));
            if constexpr ( Ordered ) {
                TRY(sub_vector.try_insert_sorted(Pair{ move(key), move(value) },
                                                 [](auto const& a, auto const& b) { return a.key() > b.key(); }));
            } else
                TRY(sub_vector.try_append(Pair{ move(key), move(value) }));

            ++m_values_count;
            return Option<T>{};
        }
    }

    /**
     * @brief Returns the value by the given key, inserts a new one if the doesn't exists
     */
    Option<T&> at(K const& key) {
        auto pair_or_none = find_pair_by_key(key);
        if ( pair_or_none.is_present() )
            return pair_or_none.value().value();
        else
            return {};
    }
    Option<T&> operator[](K const& key) {
        auto pair_or_none = find_pair_by_key(key);
        if ( pair_or_none.is_present() )
            return pair_or_none.value().value();
        else
            return {};
    }

    /**
     * @brief Erases from this map the given key or alle the keys related to the given value
     */
    ErrorOr<void> erase_key(K const& key) {
        auto erased_count
            = TRY(bucket_by_hash(Trait::TypeIntrinsics<K>::hash(key)).erase_all_matches([&key](auto const& pair) {
                  return key == pair.key();
              }));
        VERIFY_EQUAL(erased_count, 1);
        m_values_count -= erased_count;
        return {};
    }
    ErrorOr<usize> erase_value(T const& value) {
        usize removed_keys_count = 0;
        for ( auto& bucket : m_buckets_storage ) {
            auto removed_keys_count_or_error
                = bucket.erase_all_matches([&value](auto const& pair) { return pair.value() == value; });
            if ( removed_keys_count_or_error.is_value() )
                removed_keys_count += removed_keys_count_or_error.value();
        }

        if ( removed_keys_count == 0 )
            return ENOENT;
        else {
            m_values_count -= removed_keys_count;
            return removed_keys_count;
        }
    }

    /**
     * @brief Returns whether this map has the given key or the given value
     */
    [[nodiscard]] bool has_key(K const& key) const {
        return find_pair_by_key(key).is_present();
    }
    [[nodiscard]] bool has_value(T const& value) const {
        for ( auto const& bucket : m_buckets_storage ) {
            for ( auto const& pair : bucket ) {
                if ( value == pair.value() )
                    return true;
            }
        }
        return false;
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
     * @brief Returns the size of this map
     */
    [[nodiscard]] usize count() const {
        return m_values_count;
    }
    [[nodiscard]] bool is_empty() const {
        return count() == 0;
    }
    [[nodiscard]] usize capacity() const {
        return m_buckets_storage.capacity();
    }

    [[nodiscard]] Vector<Vector<Pair<K, T>>>& data() {
        return m_buckets_storage;
    }
    [[nodiscard]] Vector<Vector<Pair<K, T>>> const& data() const {
        return m_buckets_storage;
    }

private:
    Vector<Pair<K, T>>& bucket_by_hash(usize key_hash) {
        return m_buckets_storage.at(key_hash % m_buckets_storage.count());
    }
    Vector<Pair<K, T>> const& bucket_by_hash(usize key_hash) const {
        return m_buckets_storage.at(key_hash % m_buckets_storage.count());
    }

    Option<Pair<K, T>&> find_pair_by_key(K const& key) {
        return bucket_by_hash(Trait::TypeIntrinsics<K>::hash(key)).find_if([&key](auto const& pair) {
            return pair.key() == key;
        });
    }
    Option<Pair<K, T> const&> find_pair_by_key(K const& key) const {
        return bucket_by_hash(Trait::TypeIntrinsics<K>::hash(key)).find_if([&key](auto const& pair) {
            return pair.key() == key;
        });
    }

private:
    Vector<Vector<Pair<K, T>>> m_buckets_storage{};
    usize                      m_values_count{ 0 };
};

} /* namespace Collection */

using Collection::Map;

} /* namespace TC */
