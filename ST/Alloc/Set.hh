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

#include <ST/Forward.hh>

#include <ST/Alloc/Details.hh>
#include <ST/Core/Assertions.hh>
#include <ST/Core/Concept.hh>
#include <ST/Core/Hashing.hh>
#include <ST/Core/Math.hh>
#include <ST/Core/Meta.hh>
#include <ST/Core/TypeTraits.hh>
#include <ST/Lang/Cxx.hh>
#include <ST/Lang/DenyCopy.hh>
#include <ST/Lang/IntTypes.hh>
#include <ST/Lang/Must.hh>
#include <ST/Lang/Option.hh>
#include <ST/Lang/Range.hh>
#include <ST/Lang/ReverseIteratorSupport.hh>
#include <ST/Lang/Try.hh>

namespace Details {

enum class SetBucketState : u8 {
    Free     = 0x00,
    Used     = 0x10,
    Deleted  = 0x01,
    Rehashed = 0x12,
    End      = 0xFF,
};

constexpr auto set_bucket_state_is_used(SetBucketState state) -> bool {
    return (static_cast<u8>(state) & 0xf0) == static_cast<u8>(SetBucketState::Used);
}

constexpr auto set_bucket_state_is_free(SetBucketState state) -> bool {
    return (static_cast<u8>(state) & 0xf0) == static_cast<u8>(SetBucketState::Free);
}

template<typename T, typename TBucket>
class SetIterator final {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_empty() -> SetIterator<T, TBucket> {
        return SetIterator<T, TBucket>{ nullptr };
    }
    [[nodiscard]]
    static auto new_from_bucket(TBucket* bucket) -> SetIterator<T, TBucket> {
        return SetIterator<T, TBucket>{ bucket };
    }

    SetIterator(SetIterator const&) = default;

    auto operator=(SetIterator const&) -> SetIterator& = default;

    /**
     * @brief Increment operators
     */
    auto operator++() -> SetIterator& {
        if ( m_current_bucket == nullptr )
            return *this;

        do {
            ++m_current_bucket;
            if ( m_current_bucket->m_bucket_state == SetBucketState::Used )
                return *this;
        } while ( m_current_bucket->m_bucket_state != SetBucketState::End );

        if ( m_current_bucket->m_bucket_state == SetBucketState::End )
            m_current_bucket = nullptr;

        return *this;
    }
    auto operator++(int) -> SetIterator {
        SetIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    auto operator*() -> T& {
        return *operator->();
    }
    auto operator*() const -> T const& {
        return *operator->();
    }

    /**
     * @brief Pointer access operators
     */
    auto operator->() -> T* {
        verify_not_null$(m_current_bucket);
        return m_current_bucket->slot();
    }
    auto operator->() const -> T const* {
        verify_not_null$(m_current_bucket);
        return m_current_bucket->slot();
    }

    /**
     * @brief Comparison operators
     */
    auto operator==(SetIterator const& rhs) const -> bool {
        return m_current_bucket == rhs.m_current_bucket;
    }
    auto operator!=(SetIterator const& rhs) const -> bool {
        return m_current_bucket != rhs.m_current_bucket;
    }
    auto operator<=>(SetIterator const& rhs) const -> bool = default;

private:
    explicit constexpr SetIterator(TBucket* bucket)
        : m_current_bucket{ bucket } {
    }

private:
    TBucket* m_current_bucket{ nullptr };
};

template<typename T, typename TBucket, bool IsReverse>
class OrderedSetIterator final {
public:
    /**
     * @brief Error safe factory functions
     */
    [[nodiscard]]
    static auto new_empty() -> OrderedSetIterator<T, TBucket, IsReverse> {
        return OrderedSetIterator<T, TBucket, IsReverse>{ nullptr };
    }
    [[nodiscard]]
    static auto new_from_bucket(TBucket* bucket) -> OrderedSetIterator<T, TBucket, IsReverse> {
        return OrderedSetIterator<T, TBucket, IsReverse>{ bucket };
    }

    OrderedSetIterator(OrderedSetIterator const&)                    = default;
    auto operator=(OrderedSetIterator const&) -> OrderedSetIterator& = default;

    /**
     * @brief Increment operators
     */
    auto operator++() -> OrderedSetIterator& {
        if ( m_current_bucket == nullptr )
            return *this;

        if constexpr ( IsReverse ) {
            m_current_bucket = m_current_bucket->m_prev_bucket;
        } else {
            m_current_bucket = m_current_bucket->m_next_bucket;
        }
        return *this;
    }
    auto operator++(int) -> OrderedSetIterator {
        OrderedSetIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    auto operator*() -> T& {
        return *operator->();
    }
    auto operator*() const -> T const& {
        return *operator->();
    }

    /**
     * @brief Pointer access operators
     */
    auto operator->() -> T* {
        verify_not_null$(m_current_bucket);
        return m_current_bucket->slot();
    }
    auto operator->() const -> T const* {
        verify_not_null$(m_current_bucket);
        return m_current_bucket->slot();
    }

    /**
     * @brief Comparison operators
     */
    auto operator==(OrderedSetIterator const& rhs) const -> bool {
        return m_current_bucket == rhs.m_current_bucket;
    }
    auto operator!=(OrderedSetIterator const& rhs) const -> bool {
        return m_current_bucket != rhs.m_current_bucket;
    }
    auto operator<=>(OrderedSetIterator const& rhs) const -> bool = default;

private:
    explicit constexpr OrderedSetIterator(TBucket* bucket)
        : m_current_bucket{ bucket } {
    }

private:
    TBucket* m_current_bucket{ nullptr };
};

template<typename T>
struct SetBucket final {
    auto slot() -> T* {
        return Cxx::bit_cast<T*>(&m_storage);
    }
    auto slot() const -> T const* {
        return Cxx::bit_cast<T const*>(&m_storage);
    }

    SetBucketState m_bucket_state;
    alignas(T) u8 m_storage[sizeof(T)];
};

template<typename T>
struct OrderedSetBucket final {
    auto slot() -> T* {
        return Cxx::bit_cast<T*>(&m_storage);
    }
    auto slot() const -> T const* {
        return Cxx::bit_cast<T const*>(&m_storage);
    }

    OrderedSetBucket* m_prev_bucket;
    OrderedSetBucket* m_next_bucket;
    SetBucketState    m_bucket_state;
    alignas(T) u8 m_storage[sizeof(T)];
};

struct CollectionData {
    /* Nothing here */
};

template<typename BucketType>
struct OrderedCollectionData {
    BucketType* m_head{ nullptr };
    BucketType* m_tail{ nullptr };
};

enum class ReplaceExisting : bool {
    Yes,
    No
};

enum class InsertResult {
    New,
    Replaced,
    Kept
};

} /* namespace Details */

using SetReplaceExisting = Details::ReplaceExisting;
using SetInsertResult    = Details::InsertResult;

template<typename T, typename TTraits, bool IsOrdered>
class Set final {
    TCDenyCopy$(Set);

private:
    enum : usize {
        LoadFactorPercent = 60
    };

    using Bucket         = Conditional<IsOrdered, Details::OrderedSetBucket<T>, Details::SetBucket<T>>;
    using DataCollection = Conditional<IsOrdered, Details::OrderedCollectionData<Bucket>, Details::CollectionData>;

public:
    using Iterator             = Conditional<IsOrdered, Details::OrderedSetIterator<T, Bucket, false>, Details::SetIterator<T, Bucket>>;
    using ConstIterator        = Conditional<IsOrdered, Details::OrderedSetIterator<T const, Bucket const, false>, Details::SetIterator<T const, Bucket const>>;
    using ReverseIterator      = Details::OrderedSetIterator<T, Bucket, true>;
    using ConstReverseIterator = Details::OrderedSetIterator<T const, Bucket const, true>;
    using ReverseIteratorWrapper      = ReverseIteratorSupport::Wrapper<Set<T, TTraits, IsOrdered>>;
    using ConstReverseIteratorWrapper = ReverseIteratorSupport::Wrapper<Set<T, TTraits, IsOrdered> const>;

public:
    /**
     * @brief Non-Error safe factory functions
     */
    [[nodiscard]]
    static constexpr auto new_empty() -> Set<T, TTraits, IsOrdered> {
        return Set<T, TTraits, IsOrdered>{};
    }
    [[nodiscard]]
    static auto new_with_capacity(usize capacity) -> Set<T, TTraits, IsOrdered> {
        return must$(try_new_with_capacity(capacity));
    }
    [[nodiscard]]
    static auto new_from_other(Set<T, TTraits, IsOrdered> const& rhs) -> Set<T, TTraits, IsOrdered> {
        return must$(try_new_from_other(rhs));
    }
    [[nodiscard]]
    static auto new_from_list(Cxx::InitializerList<T> initializer_list) -> Set<T, TTraits, IsOrdered> {
        return must$(try_new_from_list(initializer_list));
    }

    /**
     * @brief Error safe Factory functions
     */
    static auto try_new_with_capacity(usize capacity) -> ErrorOr<Set<T, TTraits, IsOrdered>> {
        auto set = new_empty();
        try$(set.try_rehash(capacity));
        return set;
    }
    static auto try_new_from_other(Set<T, TTraits, IsOrdered> const& rhs) -> ErrorOr<Set<T, TTraits, IsOrdered>> {
        auto set = try$(try_new_with_capacity(rhs.count()));
        for ( auto const& e : rhs ) {
            if constexpr ( TryCloneable<T, ErrorOr<T>> ) {
                try$(set.try_insert(try$(e.try_clone())));
            } else if constexpr ( Cloneable<T> ) {
                try$(set.try_insert(e.clone()));
            } else if constexpr ( CopyConstructible<T> ) {
                try$(set.try_insert(e));
            }
        }

        return set;
    }
    static auto try_new_from_list(Cxx::InitializerList<T> initializer_list) -> ErrorOr<Set<T, TTraits, IsOrdered>> {
        auto set = new_empty();
        for ( auto const& e : initializer_list ) /* even with auto initializer_list exposes only T const& */
            try$(set.try_insert(Cxx::move(const_cast<T&>(e))));

        return set;
    }

    /**
     * @brief Move constructor and move assignment
     */
    Set(Set<T, TTraits, IsOrdered>&& rhs)
        : m_buckets_storage{ Cxx::exchange(rhs.m_buckets_storage, nullptr) }
        , m_collection_data{ Cxx::exchange(rhs.m_collection_data, DataCollection{}) }
        , m_data_capacity{ Cxx::exchange(rhs.m_data_capacity, 0) }
        , m_values_count{ Cxx::exchange(rhs.m_values_count, 0) }
        , m_deleted_count{ Cxx::exchange(rhs.m_deleted_count, 0) } {
    }
    auto operator=(Set<T, TTraits, IsOrdered>&& rhs) -> Set<T, TTraits, IsOrdered>& {
        Set set{ Cxx::move(rhs) };
        swap(set);
        return *this;
    }

    ~Set() {
        clear();
    }

    /**
     * @brief Deep cloning
     */
    [[nodiscard]]
    auto clone() const -> Set<T, TTraits, IsOrdered> {
        return must$(try_clone());
    }
    auto try_clone() const -> ErrorOr<Set<T, TTraits, IsOrdered>> {
        return Set<T, TTraits, IsOrdered>::try_new_from_other(*this);
    }

    /**
     * @brief Destroys all the stored values keeping the capacity of this set if requested
     */
    auto clear() -> void {
        clear_keep_capacity();

        if ( m_data_capacity > 0 ) {
            Details::__heap_plug_dealloc_mem(m_buckets_storage, size_in_bytes(capacity()));
            m_buckets_storage = nullptr;
            m_data_capacity   = 0;
        }
    }
    auto clear_keep_capacity() -> void {
        if constexpr ( !TTraits::is_trivial() ) {
            for ( auto const i : Range{ 0u, m_data_capacity } ) {
                if ( Details::set_bucket_state_is_used(m_buckets_storage[i].m_bucket_state) )
                    m_buckets_storage[i].slot()->~T();
            }
        }

        __builtin_memset(m_buckets_storage, 0, size_in_bytes(capacity()));
        if constexpr ( IsOrdered )
            m_collection_data = Details::OrderedCollectionData<Bucket>{ nullptr, nullptr };
        else
            m_buckets_storage[m_data_capacity].m_bucket_state = Details::SetBucketState::End;

        m_values_count  = 0;
        m_deleted_count = 0;
    }

    /**
     * @brief Swaps in O(1) the content of this Set with another
     */
    auto swap(Set<T, TTraits, IsOrdered>& rhs) {
        Cxx::swap(m_buckets_storage, rhs.m_buckets_storage);
        Cxx::swap(m_data_capacity, rhs.m_data_capacity);
        Cxx::swap(m_values_count, rhs.m_values_count);
        Cxx::swap(m_deleted_count, rhs.m_deleted_count);

        if constexpr ( IsOrdered )
            Cxx::swap(m_collection_data, rhs.m_collection_data);
    }

    /**
     * @brief Inserts a value using his hash. The value may already exists inside the set, then the given
     * SetReplaceExisting value is used to perform a decision
     */
    auto insert(T value, SetReplaceExisting replace_existing = SetReplaceExisting::Yes) -> SetInsertResult {
        return must$(try_insert(Cxx::move(value), replace_existing));
    }
    auto try_insert(T value, SetReplaceExisting replace_existing = SetReplaceExisting::Yes) -> ErrorOr<SetInsertResult> {
        /* find a found_bucket which is suitable for writing the value */
        auto found_bucket = try$(try_lookup_for_writing(value));
        if ( Details::set_bucket_state_is_used(found_bucket->m_bucket_state) ) {
            /* keep the existing value and return */
            if ( replace_existing == SetReplaceExisting::No )
                return SetInsertResult::Kept;

            /* replace the value with the given one */
            (*found_bucket->slot()) = Cxx::move(value); /* use move assignment to let the existing value to be correctly replaced */
            return SetInsertResult::Replaced;
        }

        /* write the value into the new bucket.
         * NOTE even if the bucket is in the Deleted state, we can overwrite using move constructor
         * because destructor of previous value was already called by remove */
        new (found_bucket->slot()) T{ Cxx::move(value) };
        if ( found_bucket->m_bucket_state == Details::SetBucketState::Deleted )
            --m_deleted_count;

        found_bucket->m_bucket_state = Details::SetBucketState::Used;

        /* update linked list of ordered values if is ordered */
        if constexpr ( IsOrdered ) {
            if ( m_collection_data.m_head == nullptr ) [[unlikely]] {
                m_collection_data.m_head = found_bucket;
            } else {
                found_bucket->m_prev_bucket             = m_collection_data.m_tail;
                m_collection_data.m_tail->m_next_bucket = found_bucket;
            }
            m_collection_data.m_tail = found_bucket;
        }

        ++m_values_count;
        return SetInsertResult::New;
    }

    /**
     * @brief Removes the given value from this set and returns whether it was existing
     */
    [[nodiscard]]
    auto remove(T const& value) -> bool {
        auto bucket = lookup_with_hash(TTraits::hash(value), [&value](T const& current) -> bool { return TTraits::equals(value, current); });
        if ( bucket != nullptr )
            return remove(*bucket);
        else
            return false;
    }

    /**
     * @brief Removes the element referenced by the given iterator
     */
    [[nodiscard]]
    auto remove(Iterator iterator) -> bool {
        if ( iterator != end() )
            return remove(*iterator.m_bucket);
        else
            return false;
    }

    /**
     * @brief Removes all the elements for which the given call_back returns true
     */
    [[nodiscard]]
    auto remove_all_matching(Predicate<T const&> auto predicate) -> usize {
        /* iterate all the used buckets and give them to the given call_back */
        usize removed_count = 0;
        for ( auto const i : Range{ 0u, m_data_capacity } ) {
            auto& bucket = m_buckets_storage[i];
            if ( Details::set_bucket_state_is_used(bucket.m_bucket_state) && predicate(*bucket.slot()) ) {
                delete_bucket(bucket);
                ++removed_count;
            }
        }

        /* update the statistics */
        if ( removed_count > 0 ) {
            m_deleted_count += removed_count;
            m_values_count -= removed_count;
        }

        /* rehash if needed and return the removed amount */
        rehash_in_place_if_needed();
        return removed_count;
    }

    /**
     * @brief Allocates new capacity for this set for at least the given capacity
     */
    auto ensure_capacity(usize capacity) {
        verify_greater_equal$(capacity, count());
        rehash(capacity * 2);
    }
    auto try_ensure_capacity(usize capacity) -> ErrorOr<void> {
        verify_greater_equal$(capacity, count());
        return try_rehash(capacity * 2);
    }

    /**
     * @brief for-each support
     */
    auto begin() -> Iterator {
        if constexpr ( IsOrdered )
            return Iterator::new_from_bucket(m_collection_data.m_head);
        else {
            /* find the first used bucket */
            for ( auto const i : Range{ 0u, m_data_capacity } ) {
                if ( Details::set_bucket_state_is_used(m_buckets_storage[i].m_bucket_state) )
                    return Iterator::new_from_bucket(&m_buckets_storage[i]);
            }
            return end();
        }
    }
    auto end() -> Iterator {
        return Iterator::new_empty();
    }

    auto begin() const -> ConstIterator {
        if constexpr ( IsOrdered )
            return ConstIterator::new_from_bucket(m_collection_data.m_head);
        else {
            /* find the first used bucket */
            for ( auto const i : Range{ 0u, m_data_capacity } ) {
                if ( Details::set_bucket_state_is_used(m_buckets_storage[i].m_bucket_state) )
                    return ConstIterator::new_from_bucket(&m_buckets_storage[i]);
            }
            return end();
        }
    }
    auto end() const -> ConstIterator {
        return ConstIterator::new_empty();
    }

    /**
     * @brief reverse for-each support
     */
    auto rbegin() -> ReverseIterator {
        static_assert(IsOrdered, "Reverse iterator only available with OrderedSet/OrderedMap");
        return ReverseIterator::new_from_bucket(m_collection_data.m_tail);
    }
    auto rend() -> ReverseIterator {
        return ReverseIterator::new_empty();
    }

    auto rbegin() const -> ConstReverseIterator {
        static_assert(IsOrdered, "Reverse iterator only available with OrderedSet/OrderedMap");
        return ConstReverseIterator::new_from_bucket(m_collection_data.m_tail);
    }
    auto rend() const -> ConstReverseIterator {
        return ConstReverseIterator::new_empty();
    }

    auto reverse_iter() -> ReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }
    auto reverse_iter() const -> ConstReverseIteratorWrapper {
        return ReverseIteratorSupport::in_reverse(*this);
    }

    /**
     * @brief Returns whether this vector contains the given value
     */
    [[nodiscard]]
    auto contains(T const& value) const -> bool {
        return find(value).is_present();
    }

    /**
     * @brief Returns a reference to the element into this Vector if exists
     */
    auto find(T const& value) -> Option<T&> {
        return find(TTraits::hash(value), [&value](auto const& current) -> bool { return TTraits::equals(value, current); });
    }
    auto find(T const& value) const -> Option<T const&> {
        return find(TTraits::hash(value), [&value](auto const& current) -> bool { return TTraits::equals(value, current); });
    }

    /**
     * @brief Returns a reference to the element for which the given call_back return true
     */
    auto find(usize hash, Predicate<T&> auto predicate) -> Option<T&> {
        auto bucket_type = lookup_with_hash(hash, Cxx::move(predicate));
        if ( bucket_type != nullptr )
            return *bucket_type->slot();
        else
            return {};
    }
    auto find(usize hash, Predicate<T const&> auto predicate) const -> Option<T const&> {
        auto const bucket_type = lookup_with_hash(hash, Cxx::move(predicate));
        if ( bucket_type != nullptr )
            return *bucket_type->slot();
        else
            return {};
    }

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto count() const -> usize {
        return m_values_count;
    }
    [[nodiscard]]
    auto capacity() const -> usize {
        return m_data_capacity;
    }
    [[nodiscard]]
    auto is_empty() const -> bool {
        return m_values_count == 0;
    }

private:
    explicit constexpr Set() = default;

    auto insert_during_rehash(T&& value) {
        auto& bucket = lookup_for_writing(value);

        /* fill the selected bucket */
        bucket.m_bucket_state = Details::SetBucketState::Used;
        new (bucket.slot()) T{ Cxx::move(value) };

        /* link to the linked list if the set is ordered */
        if constexpr ( IsOrdered ) {
            if ( !m_collection_data.m_head ) [[unlikely]] {
                m_collection_data.m_head = &bucket;
            } else {
                bucket.m_prev_bucket                    = m_collection_data.m_tail;
                m_collection_data.m_tail->m_next_bucket = &bucket;
            }
            m_collection_data.m_tail = &bucket;
        }
    }

    auto rehash(usize new_capacity) -> void {
        must$(try_rehash(new_capacity));
    }
    auto try_rehash(usize new_capacity) -> ErrorOr<void> {
        if ( new_capacity == m_data_capacity && new_capacity >= 4 ) {
            rehash_in_place();
            return {};
        }

        /* roundup the given new capacity */
        new_capacity = Math::max(new_capacity, 4u);
        new_capacity = new_capacity * sizeof(Bucket) / sizeof(Bucket);

        /* keep old references */
        auto const old_buckets  = m_buckets_storage;
        auto const old_capacity = m_data_capacity;
        auto       old_iter     = begin();

        /* allocate the new memory */
        m_buckets_storage = try$(Details::__heap_plug_alloc_mem(size_in_bytes(new_capacity))
                                     .map<Bucket*>([](void* void_ptr) -> Bucket* {
                                         return Cxx::bit_cast<Bucket*>(void_ptr);
                                     }));
        m_data_capacity   = new_capacity;
        m_deleted_count   = 0;

        /* update the collection */
        if constexpr ( IsOrdered )
            m_collection_data = Details::OrderedCollectionData<Bucket>{ nullptr, nullptr };
        else
            m_buckets_storage[m_data_capacity].m_bucket_state = Details::SetBucketState::End;

        /* return if this set was emtpy */
        if ( old_buckets == nullptr )
            return {};

        /* move the values to the new memory */
        for ( auto it = Cxx::move(old_iter); it != end(); ++it ) {
            insert_during_rehash(Cxx::move(*it));
            it->~T();
        }

        /* free the old memory */
        Details::__heap_plug_dealloc_mem(m_buckets_storage, size_in_bytes(old_capacity));
        return {};
    }

    void rehash_in_place_if_needed() {
        if ( m_deleted_count >= m_values_count && should_grow() )
            rehash_in_place();
    }

    void rehash_in_place() {
        for ( auto const i : Range{ 0u, m_data_capacity } ) {
            auto& bucket = m_buckets_storage[i];

            if ( bucket.m_bucket_state == Details::SetBucketState::Rehashed || bucket.m_bucket_state == Details::SetBucketState::End
                 || bucket.m_bucket_state == Details::SetBucketState::Free )
                continue;
            if ( bucket.m_bucket_state == Details::SetBucketState::Deleted ) {
                bucket.m_bucket_state = Details::SetBucketState::Free;
                continue;
            }

            auto const new_hash = TTraits::hash(*bucket.slot());
            if ( new_hash % m_data_capacity == i ) {
                bucket.m_bucket_state = Details::SetBucketState::Rehashed;
                continue;
            }

            auto const to_move_hash   = i;
            auto       target_hash    = new_hash;
            auto       target_bucket  = &m_buckets_storage[target_hash % m_data_capacity];
            auto       bucket_to_move = &m_buckets_storage[i];

            /* Try to move the bucket to move into its correct spot.
             * During the procedure, we might re-hash or actually change the bucket to move
             */
            while ( !Details::set_bucket_state_is_free(bucket_to_move->m_bucket_state) ) {
                /* if we're targeting ourselves, there's nothing to do */
                if ( to_move_hash == target_hash % m_data_capacity ) {
                    bucket_to_move->m_bucket_state = Details::SetBucketState::Rehashed;
                    break;
                }

                if ( Details::set_bucket_state_is_free(target_bucket->m_bucket_state) ) {
                    /* we can just overwrite the target bucket and bail out */
                    new (target_bucket->slot()) T{ Cxx::move(*bucket_to_move->slot()) };
                    target_bucket->m_bucket_state  = Details::SetBucketState::Rehashed;
                    bucket_to_move->m_bucket_state = Details::SetBucketState::Free;

                    if constexpr ( IsOrdered ) {
                        Cxx::swap(bucket_to_move->m_prev_bucket, target_bucket->m_prev_bucket);
                        Cxx::swap(bucket_to_move->m_next_bucket, target_bucket->m_next_bucket);

                        if ( target_bucket->m_prev_bucket )
                            target_bucket->m_prev_bucket->m_next_bucket = target_bucket;
                        else
                            m_collection_data.m_head = target_bucket;
                        if ( target_bucket->m_next_bucket )
                            target_bucket->m_next_bucket->m_prev_bucket = target_bucket;
                        else
                            m_collection_data.m_tail = target_bucket;
                    }
                } else if ( target_bucket->m_bucket_state == Details::SetBucketState::Rehashed ) {
                    /* if the target bucket is already re-hashed, we do normal probing */
                    target_hash   = Hashing::hash_again(target_hash);
                    target_bucket = &m_buckets_storage[target_hash % m_data_capacity];
                } else {
                    verify$(target_bucket->m_bucket_state != Details::SetBucketState::End);

                    /* the target bucket is a used bucket that hasn't been re-hashed.
                     * Swap the data into the target; now the target's data resides in the bucket to move again.
                     * (That's of course what we want, how neat!)
                     */
                    Cxx::swap(*bucket_to_move->slot(), *target_bucket->slot());
                    bucket_to_move->m_bucket_state = target_bucket->m_bucket_state;
                    target_bucket->m_bucket_state  = Details::SetBucketState::Rehashed;

                    if constexpr ( IsOrdered ) {
                        /* update state for the target bucket, we'll do the bucket to move later */
                        Cxx::swap(bucket_to_move->m_prev_bucket, target_bucket->m_prev_bucket);
                        Cxx::swap(bucket_to_move->m_next_bucket, target_bucket->m_next_bucket);

                        if ( target_bucket->m_prev_bucket )
                            target_bucket->m_prev_bucket->m_next_bucket = target_bucket;
                        else
                            m_collection_data.m_head = target_bucket;
                        if ( target_bucket->m_next_bucket )
                            target_bucket->m_next_bucket->m_prev_bucket = target_bucket;
                        else
                            m_collection_data.m_tail = target_bucket;
                    }

                    target_hash   = TTraits::hash(*bucket_to_move->slot());
                    target_bucket = &m_buckets_storage[target_hash % m_data_capacity];

                    /* the data is already in the correct location: Adjust the pointers */
                    if ( target_hash % m_data_capacity == to_move_hash ) {
                        bucket_to_move->m_bucket_state = Details::SetBucketState::Rehashed;
                        if constexpr ( IsOrdered ) {
                            /* update state for the bucket to move as it's not actually moved anymore */
                            if ( bucket_to_move->m_prev_bucket )
                                bucket_to_move->m_prev_bucket->m_next_bucket = bucket_to_move;
                            else
                                m_collection_data.m_head = bucket_to_move;
                            if ( bucket_to_move->m_next_bucket )
                                bucket_to_move->m_next_bucket->m_prev_bucket = bucket_to_move;
                            else
                                m_collection_data.m_tail = bucket_to_move;
                        }
                        break;
                    }
                }
            }

            /* after this, the bucket_to_move either contains data that rehashes to itself,
             * or it contains nothing as we were able to move the last thing
             */
            if ( bucket_to_move->m_bucket_state == Details::SetBucketState::Deleted )
                bucket_to_move->m_bucket_state = Details::SetBucketState::Free;
        }

        m_deleted_count = 0;
        for ( auto const i : Range{ 0u, m_data_capacity } ) {
            if ( m_buckets_storage[i].m_bucket_state == Details::SetBucketState::Rehashed ) {
                m_buckets_storage[i].m_bucket_state = Details::SetBucketState::Used;
            }
        }
    }

    [[nodiscard]]
    auto lookup_with_hash(u32 hash, auto predicate) const -> Bucket* {
        if ( is_empty() )
            return nullptr;

        /* iterate each bucket */
        for ( ;; ) {
            auto& bucket = m_buckets_storage[hash % m_data_capacity];

            /* give to the predicate if it is used */
            if ( Details::set_bucket_state_is_used(bucket.m_bucket_state) && predicate(*bucket.slot()) )
                return &bucket;

            /* stop on end bucket */
            if ( bucket.m_bucket_state != Details::SetBucketState::Used && bucket.m_bucket_state != Details::SetBucketState::Deleted )
                return nullptr;

            /* double the hash and try again */
            hash = Hashing::hash_again(hash);
        }
    }

    auto try_lookup_for_writing(T const& value) -> ErrorOr<Bucket*> {
        if ( should_grow() )
            try$(try_rehash(capacity() * 2));

        /* pre-calculate the hash value */
        auto hash = TTraits::hash(value);

        /* iterate each bucket to find the first usable */
        Bucket* first_empty_bucket = nullptr;
        for ( ;; ) {
            auto& bucket = m_buckets_storage[hash % m_data_capacity];

            /* use the already used bucket if the value matches */
            if ( Details::set_bucket_state_is_used(bucket.m_bucket_state) && TTraits::equals(*bucket.slot(), value) )
                return &bucket;

            /* use a new and empty bucket */
            if ( !Details::set_bucket_state_is_used(bucket.m_bucket_state) ) {
                if ( first_empty_bucket == nullptr )
                    first_empty_bucket = &bucket;

                if ( bucket.m_bucket_state != Details::SetBucketState::Deleted )
                    return first_empty_bucket;
            }

            /* double the hash and try again */
            hash = Hashing::hash_again(hash);
        }
    }
    [[nodiscard]]
    auto lookup_for_writing(T const& value) -> Bucket& {
        return *must$(try_lookup_for_writing(value));
    }

    [[nodiscard]]
    auto used_bucket_count() const -> usize {
        return m_values_count + m_deleted_count;
    }

    [[nodiscard]]
    auto should_grow() const -> bool {
        return ((used_bucket_count() + 1) * 100) >= (m_data_capacity * LoadFactorPercent);
    }

    [[nodiscard]]
    static constexpr auto size_in_bytes(usize capacity) -> usize {
        if constexpr ( IsOrdered )
            return sizeof(Bucket) * capacity;
        else
            return sizeof(Bucket) * (capacity + 1);
    }

    auto remove(Bucket& bucket) -> bool {
        verify$(Details::set_bucket_state_is_used(bucket.m_bucket_state));

        delete_bucket(bucket);
        --m_values_count;
        ++m_deleted_count;
        rehash_in_place_if_needed();
        return true;
    }

    auto delete_bucket(Bucket& bucket) {
        bucket.slot()->~T();
        bucket.m_bucket_state = Details::SetBucketState::Deleted;

        if constexpr ( IsOrdered ) {
            if ( bucket.m_prev_bucket != nullptr )
                bucket.m_prev_bucket->m_next_bucket = bucket.m_next_bucket;
            else
                m_collection_data.m_head = bucket.m_next_bucket;

            if ( bucket.m_next_bucket != nullptr )
                bucket.m_next_bucket->m_prev_bucket = bucket.m_prev_bucket;
            else
                m_collection_data.m_tail = bucket.m_prev_bucket;
        }
    }

    Bucket*        m_buckets_storage{ nullptr };
    DataCollection m_collection_data{};
    usize          m_data_capacity{ 0 };
    usize          m_values_count{ 0 };
    usize          m_deleted_count{ 0 };
};
