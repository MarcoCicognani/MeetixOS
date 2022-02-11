
#include <TC/Collection/Map.hh>

namespace TC::Collection {
namespace Details {

template<typename MainContainer, typename InnerContainer, typename T>
MapIterator<MainContainer, InnerContainer, T>
MapIterator<MainContainer, InnerContainer, T>::begin(MainContainer& main_container) {
    usize first_level_index = 0;
    for ( auto const& inner : main_container.data() ) {
        if ( !inner.is_empty() )
            break;

        ++first_level_index;
    }

    return MapIterator{ main_container, first_level_index, main_container.data().at(first_level_index).begin() };
}

template<typename MainContainer, typename InnerContainer, typename T>
MapIterator<MainContainer, InnerContainer, T>
MapIterator<MainContainer, InnerContainer, T>::end(MainContainer& main_container) {
    return MapIterator{ main_container, main_container.data().count() - 1, main_container.data().last().end() };
}

template<typename MainContainer, typename InnerContainer, typename T>
MapIterator<MainContainer, InnerContainer, T>& MapIterator<MainContainer, InnerContainer, T>::operator++() {
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

template<typename MainContainer, typename InnerContainer, typename T>
MapIterator<MainContainer, InnerContainer, T> MapIterator<MainContainer, InnerContainer, T>::operator++(int) {
    MapIterator it{ *this };

    operator++();
    return it;
}

template<typename MainContainer, typename InnerContainer, typename T>
T& MapIterator<MainContainer, InnerContainer, T>::operator*() {
    return *m_nested_iterator;
}

template<typename MainContainer, typename InnerContainer, typename T>
T const& MapIterator<MainContainer, InnerContainer, T>::operator*() const {
    return *m_nested_iterator;
}

template<typename MainContainer, typename InnerContainer, typename T>
T* MapIterator<MainContainer, InnerContainer, T>::operator->() {
    return &operator*();
}

template<typename MainContainer, typename InnerContainer, typename T>
T const* MapIterator<MainContainer, InnerContainer, T>::operator->() const {
    return &operator*();
}

template<typename MainContainer, typename InnerContainer, typename T>
bool MapIterator<MainContainer, InnerContainer, T>::is_end() const {
    return m_nested_iterator.is_end();
}

template<typename MainContainer, typename InnerContainer, typename T>
bool MapIterator<MainContainer, InnerContainer, T>::operator==(const MapIterator& rhs) const {
    return m_nested_iterator == rhs.m_nested_iterator;
}

template<typename MainContainer, typename InnerContainer, typename T>
bool MapIterator<MainContainer, InnerContainer, T>::operator!=(const MapIterator& rhs) const {
    return m_nested_iterator != rhs.m_nested_iterator;
}

template<typename MainContainer, typename InnerContainer, typename T>
MapIterator<MainContainer, InnerContainer, T>::MapIterator(MainContainer&                    main_container,
                                                           usize                             first_level_index,
                                                           typename InnerContainer::Iterator nested_iterator)
    : m_main_container{ main_container }
    , m_first_level_index{ first_level_index }
    , m_nested_iterator{ nested_iterator } {
}

} /* namespace Details */

template<typename K, typename T, bool Ordered>
Map<K, T, Ordered>::Map()
    : Map{ 128 } {
}

template<typename K, typename T, bool Ordered>
Map<K, T, Ordered>::Map(usize bucket_count)
    : m_buckets_storage{ bucket_count } {
    m_buckets_storage.resize(bucket_count);
}

template<typename K, typename T, bool Ordered>
Map<K, T, Ordered>::Map(Map const& rhs)
    : m_buckets_storage{ rhs.m_buckets_storage }
    , m_values_count{ rhs.m_values_count } {
}

template<typename K, typename T, bool Ordered>
Map<K, T, Ordered>::Map(Map&& rhs) noexcept
    : m_buckets_storage{ move(rhs.m_buckets_storage) }
    , m_values_count{ exchange(rhs.m_values_count, 0) } {
}

template<typename K, typename T, bool Ordered>
Map<K, T, Ordered>::Map(std::initializer_list<Pair<K, T>> initializer_list)
    : Map{} {
    for ( auto const& pair : initializer_list )
        insert(pair.key(), pair.value());
}

template<typename K, typename T, bool Ordered>
Map<K, T, Ordered>& Map<K, T, Ordered>::operator=(Map const& rhs) {
    if ( this == &rhs )
        return *this;

    Map map{ rhs };
    swap(map);
    return *this;
}

template<typename K, typename T, bool Ordered>
Map<K, T, Ordered>& Map<K, T, Ordered>::operator=(Map&& rhs) noexcept {
    Map map{ move(rhs) };
    swap(map);
    return *this;
}

template<typename K, typename T, bool Ordered>
Map<K, T, Ordered>& Map<K, T, Ordered>::operator=(std::initializer_list<Pair<K, T>> initializer_list) {
    Map map{ initializer_list };
    swap(map);
    return *this;
}

template<typename K, typename T, bool Ordered>
void Map<K, T, Ordered>::clear() {
    auto capacity = m_buckets_storage.capacity();
    m_buckets_storage.clear(decltype(m_buckets_storage)::KeepStorageCapacity::Yes);
    m_buckets_storage.resize(capacity);
    m_values_count = 0;
}

template<typename K, typename T, bool Ordered>
void Map<K, T, Ordered>::swap(Map& rhs) noexcept {
    m_buckets_storage.swap(rhs.m_buckets_storage);
    Cxx::swap(m_values_count, rhs.m_values_count);
}

template<typename K, typename T, bool Ordered>
Option<T> Map<K, T, Ordered>::insert(K const& key, T const& value, Map::OnExistingKey on_existing_key) {
    return MUST(try_insert(K{ key }, T{ value }, on_existing_key));
}

template<typename K, typename T, bool Ordered>
Option<T> Map<K, T, Ordered>::insert(K&& key, T&& value, Map::OnExistingKey on_existing_key) {
    return MUST(try_insert(move(key), move(value), on_existing_key));
}

template<typename K, typename T, bool Ordered>
ErrorOr<Option<T>> Map<K, T, Ordered>::try_insert(K const& key, T const& value, Map::OnExistingKey on_existing_key) {
    return try_insert(K{ key }, T{ value }, on_existing_key);
}

template<typename K, typename T, bool Ordered>
ErrorOr<Option<T>> Map<K, T, Ordered>::try_insert(K&& key, T&& value, Map::OnExistingKey on_existing_key) {
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

template<typename K, typename T, bool Ordered>
Option<T&> Map<K, T, Ordered>::at(K const& key) {
    auto pair_or_none = find_pair_by_key(key);
    if ( pair_or_none.is_present() )
        return pair_or_none.value().value();
    else
        return {};
}

template<typename K, typename T, bool Ordered>
Option<T&> Map<K, T, Ordered>::operator[](K const& key) {
    auto pair_or_none = find_pair_by_key(key);
    if ( pair_or_none.is_present() )
        return pair_or_none.value().value();
    else
        return {};
}

template<typename K, typename T, bool Ordered>
ErrorOr<void> Map<K, T, Ordered>::erase_key(K const& key) {
    auto erased_count
        = TRY(bucket_by_hash(Trait::TypeIntrinsics<K>::hash(key)).erase_all_matches([&key](auto const& pair) {
              return key == pair.key();
          }));
    VERIFY_EQUAL(erased_count, 1);
    m_values_count -= erased_count;
    return {};
}

template<typename K, typename T, bool Ordered>
ErrorOr<usize> Map<K, T, Ordered>::erase_value(T const& value) {
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

template<typename K, typename T, bool Ordered>
bool Map<K, T, Ordered>::has_key(K const& key) const {
    return find_pair_by_key(key).is_present();
}

template<typename K, typename T, bool Ordered>
bool Map<K, T, Ordered>::has_value(T const& value) const {
    for ( auto const& bucket : m_buckets_storage ) {
        for ( auto const& pair : bucket ) {
            if ( value == pair.value() )
                return true;
        }
    }
    return false;
}

template<typename K, typename T, bool Ordered>
typename Map<K, T, Ordered>::Iterator Map<K, T, Ordered>::begin() {
    return Iterator::begin(*this);
}

template<typename K, typename T, bool Ordered>
typename Map<K, T, Ordered>::Iterator Map<K, T, Ordered>::end() {
    return Iterator::end(*this);
}

template<typename K, typename T, bool Ordered>
typename Map<K, T, Ordered>::ConstIterator Map<K, T, Ordered>::begin() const {
    return ConstIterator::begin(*this);
}

template<typename K, typename T, bool Ordered>
typename Map<K, T, Ordered>::ConstIterator Map<K, T, Ordered>::end() const {
    return ConstIterator::end(*this);
}

template<typename K, typename T, bool Ordered>
usize Map<K, T, Ordered>::count() const {
    return m_values_count;
}

template<typename K, typename T, bool Ordered>
bool Map<K, T, Ordered>::is_empty() const {
    return count() == 0;
}

template<typename K, typename T, bool Ordered>
usize Map<K, T, Ordered>::capacity() const {
    return m_buckets_storage.capacity();
}

template<typename K, typename T, bool Ordered>
Vector<Vector<Pair<K, T>>>& Map<K, T, Ordered>::data() {
    return m_buckets_storage;
}

template<typename K, typename T, bool Ordered>
const Vector<Vector<Pair<K, T>>>& Map<K, T, Ordered>::data() const {
    return m_buckets_storage;
}

template<typename K, typename T, bool Ordered>
Vector<Pair<K, T>> const& Map<K, T, Ordered>::bucket_by_hash(usize key_hash) const {
    return m_buckets_storage.at(key_hash % m_buckets_storage.count());
}

template<typename K, typename T, bool Ordered>
Vector<Pair<K, T>>& Map<K, T, Ordered>::bucket_by_hash(usize key_hash) {
    return m_buckets_storage.at(key_hash % m_buckets_storage.count());
}

template<typename K, typename T, bool Ordered>
Option<Pair<K, T>&> Map<K, T, Ordered>::find_pair_by_key(K const& key) {
    return bucket_by_hash(Trait::TypeIntrinsics<K>::hash(key)).find_if([&key](auto const& pair) {
        return pair.key() == key;
    });
}

template<typename K, typename T, bool Ordered>
Option<Pair<K, T> const&> Map<K, T, Ordered>::find_pair_by_key(K const& key) const {
    return bucket_by_hash(Trait::TypeIntrinsics<K>::hash(key)).find_if([&key](auto const& pair) {
        return pair.key() == key;
    });
}

} /* namespace TC::Collection */