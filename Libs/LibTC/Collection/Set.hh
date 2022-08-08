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

#include <LibTC/Assertions.hh>
#include <LibTC/BitCast.hh>
#include <LibTC/Collection/Enums/InsertResult.hh>
#include <LibTC/Collection/Enums/KeepStorageCapacity.hh>
#include <LibTC/Collection/Enums/OnExistingEntry.hh>
#include <LibTC/Collection/Range.hh>
#include <LibTC/Collection/ReverseIteratorSupport.hh>
#include <LibTC/Concept.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Forward.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Hashing.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Math.hh>
#include <LibTC/Memory/Raw.hh>
#include <LibTC/Meta.hh>
#include <LibTC/TypeTraits.hh>

namespace TC {
namespace Collection {
namespace Details {

enum class SetBucketState : u8 {
    Free     = 0x00,
    Used     = 0x10,
    Deleted  = 0x01,
    Rehashed = 0x12,
    End      = 0xFF,
};

constexpr auto is_used_bucket(SetBucketState state) -> bool {
    return (static_cast<u8>(state) & 0xf0) == 0x10;
}

constexpr auto is_free_bucket(SetBucketState state) -> bool {
    return (static_cast<u8>(state) & 0xf0) == 0x00;
}

template<typename Collection, typename T, typename BucketType>
class SetIterator {
public:
    /**
     * @brief Constructors
     */
    explicit SetIterator(BucketType* bucket)
        : m_current_bucket{ bucket } {
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
        VERIFY_NOT_NULL(m_current_bucket);
        return m_current_bucket->slot();
    }
    auto operator->() const -> T const* {
        VERIFY_NOT_NULL(m_current_bucket);
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
    BucketType* m_current_bucket{ nullptr };
};

template<typename Collection, typename T, typename BucketType, bool IsReverse>
class OrderedSetIterator {
public:
    /**
     * @brief Constructors
     */
    explicit OrderedSetIterator(BucketType* bucket)
        : m_current_bucket{ bucket } {
    }
    OrderedSetIterator(OrderedSetIterator const&) = default;

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
        VERIFY_NOT_NULL(m_current_bucket);
        return m_current_bucket->slot();
    }
    auto operator->() const -> T const* {
        VERIFY_NOT_NULL(m_current_bucket);
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
    BucketType* m_current_bucket{ nullptr };
};

template<typename T>
struct SetBucket {
    auto slot() -> T* {
        return bit_cast<T*>(&m_storage);
    }
    auto slot() const -> T const* {
        return bit_cast<T const*>(&m_storage);
    }

    SetBucketState m_bucket_state;
    alignas(T) u8 m_storage[sizeof(T)];
};

template<typename T>
struct OrderedSetBucket {
    auto slot() -> T* {
        return bit_cast<T*>(&m_storage);
    }
    auto slot() const -> T const* {
        return bit_cast<T const*>(&m_storage);
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

} /* namespace Details */

template<typename T, typename TTraits, bool IsOrdered>
class Set {
    TC_DENY_COPY(Set);

private:
    using Bucket         = Conditional<IsOrdered, Details::OrderedSetBucket<T>, Details::SetBucket<T>>;
    using DataCollection = Conditional<IsOrdered, Details::OrderedCollectionData<Bucket>, Details::CollectionData>;

public:
    static constexpr usize LOAD_FACTOR_PERCENT = 60;

    using Iterator      = Conditional<IsOrdered, Details::OrderedSetIterator<Set, T, Bucket, false>, Details::SetIterator<Set, T, Bucket>>;
    using ConstIterator = Conditional<IsOrdered,
                                      Details::OrderedSetIterator<Set const, T const, Bucket const, false>,
                                      Details::SetIterator<Set const, T const, Bucket const>>;
    using ReverseIterator      = Details::OrderedSetIterator<Set, T, Bucket, true>;
    using ConstReverseIterator = Details::OrderedSetIterator<Set const, T const, Bucket const, true>;

public:
    /**
     * @brief Non-error safe Factory functions
     */
    static constexpr auto construct_empty() -> Set<T, TTraits, IsOrdered> {
        return Set<T, TTraits, IsOrdered>{};
    }
    static auto construct_with_capacity(usize capacity) -> Set<T, TTraits, IsOrdered> {
        return MUST(try_construct_with_capacity(capacity));
    }
    static auto construct_from_other(Set<T, TTraits, IsOrdered> const& rhs) -> Set<T, TTraits, IsOrdered> {
        return MUST(try_construct_from_other(rhs));
    }
    static auto construct_from_list(std::initializer_list<T> initializer_list) -> Set<T, TTraits, IsOrdered> {
        return MUST(try_construct_from_list(initializer_list));
    }

    /**
     * @brief Error safe Factory functions
     */
    static auto try_construct_with_capacity(usize capacity) -> ErrorOr<Set<T, TTraits, IsOrdered>> {
        auto set = construct_empty();
        TRY(set.try_rehash(capacity));
        return set;
    }
    static auto try_construct_from_other(Set<T, TTraits, IsOrdered> const& rhs) -> ErrorOr<Set<T, TTraits, IsOrdered>> {
        auto set = TRY(try_construct_with_capacity(rhs.count()));
        for ( auto const& element : rhs ) {
            if constexpr ( TryCloneable<T, ErrorOr<T>> ) {
                TRY(set.try_insert(TRY(element.try_clone())));
            } else if constexpr ( Cloneable<T> ) {
                TRY(set.try_insert(element.clone()));
            } else if constexpr ( CopyConstructible<T> ) {
                TRY(set.try_insert(element));
            }
        }

        return set;
    }
    static auto try_construct_from_list(std::initializer_list<T> initializer_list) -> ErrorOr<Set<T, TTraits, IsOrdered>> {
        auto set = construct_empty();
        for ( auto const& element : initializer_list ) /* even with auto initializer_list exposes only T const& */
            TRY(set.try_insert(Cxx::move(const_cast<T&>(element))));

        return set;
    }

    /**
     * @brief Move constructor and move assignment
     */
    Set(Set<T, TTraits, IsOrdered>&& rhs) noexcept
        : m_buckets_storage{ Cxx::exchange(rhs.m_buckets_storage, nullptr) }
        , m_collection_data{ Cxx::exchange(rhs.m_collection_data, DataCollection{}) }
        , m_data_capacity{ Cxx::exchange(rhs.m_data_capacity, 0) }
        , m_values_count{ Cxx::exchange(rhs.m_values_count, 0) }
        , m_deleted_count{ Cxx::exchange(rhs.m_deleted_count, 0) } {
    }
    auto operator=(Set<T, TTraits, IsOrdered>&& rhs) noexcept -> Set<T, TTraits, IsOrdered>& {
        Set set{ Cxx::move(rhs) };
        swap(set);
        return *this;
    }

    ~Set() {
        clear(KeepStorageCapacity::No);
    }

    /**
     * @brief Deep cloning
     */
    [[nodiscard]] auto clone() const -> Set<T, TTraits, IsOrdered> {
        return MUST(try_clone());
    }
    [[nodiscard]] auto try_clone() const -> ErrorOr<Set<T, TTraits, IsOrdered>> {
        return Set<T, TTraits, IsOrdered>::try_construct_from_other(*this);
    }

    /**
     * @brief Destroys all the stored values keeping the capacity of this set if requested
     */
    auto clear(KeepStorageCapacity keep_storage_capacity = KeepStorageCapacity::Yes) {
        if ( m_buckets_storage == nullptr )
            return;

        /* call the destructors only for non-trivial types */
        if constexpr ( !TTraits::is_trivial() ) {
            for ( auto const i : Range{ 0u, m_data_capacity } ) {
                if ( Details::is_used_bucket(m_buckets_storage[i].m_bucket_state) )
                    m_buckets_storage[i].slot()->~T();
            }
        }

        /* free the memory if requested */
        if ( keep_storage_capacity == KeepStorageCapacity::Yes ) {
            __builtin_memset(m_buckets_storage, 0, size_in_bytes(capacity()));

            if constexpr ( IsOrdered )
                m_collection_data = Details::OrderedCollectionData<Bucket>{ nullptr, nullptr };
            else
                m_buckets_storage[m_data_capacity].m_bucket_state = Details::SetBucketState::End;
        } else {
            Memory::Raw::free_sized(m_buckets_storage, size_in_bytes(capacity()));
            m_data_capacity = 0;
        }
        m_values_count  = 0;
        m_deleted_count = 0;
    }

    /**
     * @brief Swaps in O(1) the content of this Set with another
     */
    auto swap(Set<T, TTraits, IsOrdered>& rhs) noexcept {
        Cxx::swap(m_buckets_storage, rhs.m_buckets_storage);
        Cxx::swap(m_data_capacity, rhs.m_data_capacity);
        Cxx::swap(m_values_count, rhs.m_values_count);
        Cxx::swap(m_deleted_count, rhs.m_deleted_count);

        if constexpr ( IsOrdered )
            Cxx::swap(m_collection_data, rhs.m_collection_data);
    }

    /**
     * @brief Inserts a value using his hash. The value may already exists inside the set, then the given
     * OnExistingEntry value is used to perform a decision
     */
    auto insert(T value, OnExistingEntry const on_existing_entry = OnExistingEntry::Replace) -> InsertResult {
        return MUST(try_insert(Cxx::move(value), on_existing_entry));
    }
    auto try_insert(T value, OnExistingEntry const on_existing_entry = OnExistingEntry::Replace) -> ErrorOr<InsertResult> {
        /* find a found_bucket which is suitable for writing the value */
        auto found_bucket = TRY(try_lookup_for_writing(value));
        if ( Details::is_used_bucket(found_bucket->m_bucket_state) ) {
            /* keep the existing value and return */
            if ( on_existing_entry == OnExistingEntry::Keep )
                return InsertResult::KeptExisting;

            /* replace the value with the given one */
            (*found_bucket->slot()) = Cxx::move(value); /* use move assignment to let the existing value to be correctly replaced */
            return InsertResult::ReplacedExisting;
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
        return InsertResult::InsertedNew;
    }

    /**
     * @brief Removes the given value from this set and returns whether it was existing
     */
    auto remove(T const& value) -> bool {
        auto bucket
            = lookup_with_hash(TTraits::hash(value), [&value](auto const& current) -> bool { return TTraits::equals(value, current); });
        if ( bucket != nullptr )
            return remove(*bucket);
        else
            return false;
    }

    /**
     * @brief Removes the element referenced by the given iterator
     */
    auto remove(Iterator iterator) -> bool {
        if ( iterator != end() )
            return remove(*iterator.m_bucket);
        else
            return false;
    }

    /**
     * @brief Removes all the elements for which the given call_back returns true
     */
    auto remove_all_matching(Predicate<T const&> auto predicate) -> usize {
        /* iterate all the used buckets and give them to the given call_back */
        usize removed_count = 0;
        for ( auto const i : Range{ 0u, m_data_capacity } ) {
            auto& bucket = m_buckets_storage[i];
            if ( Details::is_used_bucket(bucket.m_bucket_state) && predicate(*bucket.slot()) ) {
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
        VERIFY_GREATER_EQUAL(capacity, count());
        rehash(capacity * 2);
    }
    auto try_ensure_capacity(usize capacity) -> ErrorOr<void> {
        VERIFY_GREATER_EQUAL(capacity, count());
        return try_rehash(capacity * 2);
    }

    /**
     * @brief for-each support
     */
    auto begin() -> Iterator {
        if constexpr ( IsOrdered )
            return Iterator{ m_collection_data.m_head };
        else {
            /* find the first used bucket */
            for ( auto const i : Range{ 0u, m_data_capacity } ) {
                if ( Details::is_used_bucket(m_buckets_storage[i].m_bucket_state) )
                    return Iterator{ &m_buckets_storage[i] };
            }
            return end();
        }
    }
    auto end() -> Iterator {
        return Iterator{ nullptr };
    }

    auto begin() const -> ConstIterator {
        if constexpr ( IsOrdered )
            return ConstIterator{ m_collection_data.m_head };
        else {
            /* find the first used bucket */
            for ( auto const i : Range{ 0u, m_data_capacity } ) {
                if ( Details::is_used_bucket(m_buckets_storage[i].m_bucket_state) )
                    return ConstIterator{ &m_buckets_storage[i] };
            }
            return end();
        }
    }
    auto end() const -> ConstIterator {
        return ConstIterator{ nullptr };
    }

    /**
     * @brief reverse for-each support
     */
    auto rbegin() -> ReverseIterator {
        static_assert(IsOrdered, "Reverse iterator only available with OrderedSet/OrderedMap");
        return ReverseIterator{ m_collection_data.m_head };
    }
    auto rend() -> ReverseIterator {
        return ReverseIterator{ nullptr };
    }

    auto rbegin() const -> ConstReverseIterator {
        static_assert(IsOrdered, "Reverse iterator only available with OrderedSet/OrderedMap");
        return ConstReverseIterator{ m_collection_data.m_head };
    }
    auto rend() const -> ConstReverseIterator {
        return ConstReverseIterator{ nullptr };
    }

    auto reverse_iter() -> ReverseIteratorSupport::Wrapper<Set<T, TTraits, IsOrdered>> {
        return ReverseIteratorSupport::in_reverse(*this);
    }
    auto reverse_iter() const -> ReverseIteratorSupport::Wrapper<Set<T, TTraits, IsOrdered> const> {
        return ReverseIteratorSupport::in_reverse(*this);
    }

    /**
     * @brief Returns whether this vector contains the given value
     */
    [[nodiscard]] auto contains(T const& value) const -> bool {
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
    [[nodiscard]] auto count() const -> usize {
        return m_values_count;
    }
    [[nodiscard]] auto capacity() const -> usize {
        return m_data_capacity;
    }
    [[nodiscard]] auto is_empty() const -> bool {
        return m_values_count == 0;
    }

private:
    explicit constexpr Set() noexcept = default;

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

    auto rehash(usize new_capacity) {
        MUST(try_rehash(new_capacity));
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
        m_buckets_storage = TRY(Memory::Raw::clean_alloc_object<Bucket>(size_in_bytes(new_capacity)));
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
        Memory::Raw::free_sized(old_buckets, size_in_bytes(old_capacity));
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

            auto       target_hash    = new_hash;
            auto const to_move_hash   = i;
            auto       target_bucket  = &m_buckets_storage[target_hash % m_data_capacity];
            auto       bucket_to_move = &m_buckets_storage[i];

            /* Try to move the bucket to move into its correct spot.
             * During the procedure, we might re-hash or actually change the bucket to move
             */
            while ( !Details::is_free_bucket(bucket_to_move->m_bucket_state) ) {
                /* if we're targeting ourselves, there's nothing to do */
                if ( to_move_hash == target_hash % m_data_capacity ) {
                    bucket_to_move->m_bucket_state = Details::SetBucketState::Rehashed;
                    break;
                }

                if ( Details::is_free_bucket(target_bucket->m_bucket_state) ) {
                    /* we can just overwrite the target bucket and bail out */
                    new (target_bucket->slot()) T(Cxx::move(*bucket_to_move->slot()));
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
                    target_hash   = Hashing::rehash_key(target_hash);
                    target_bucket = &m_buckets_storage[target_hash % m_data_capacity];
                } else {
                    VERIFY(target_bucket->m_bucket_state != Details::SetBucketState::End);

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

        for ( auto const i : Range{ 0u, m_data_capacity } ) {
            if ( m_buckets_storage[i].m_bucket_state == Details::SetBucketState::Rehashed )
                m_buckets_storage[i].m_bucket_state = Details::SetBucketState::Used;
        }

        m_deleted_count = 0;
    }

    [[nodiscard]] auto lookup_with_hash(u32 hash, auto predicate) const -> Bucket* {
        if ( is_empty() )
            return nullptr;

        /* iterate each bucket */
        for ( ;; ) {
            auto& bucket = m_buckets_storage[hash % m_data_capacity];

            /* give to the predicate if it is used */
            if ( Details::is_used_bucket(bucket.m_bucket_state) && predicate(*bucket.slot()) )
                return &bucket;

            /* stop on end bucket */
            if ( bucket.m_bucket_state != Details::SetBucketState::Used && bucket.m_bucket_state != Details::SetBucketState::Deleted )
                return nullptr;

            /* double the hash and try again */
            hash = Hashing::rehash_key(hash);
        }
    }

    auto try_lookup_for_writing(T const& value) -> ErrorOr<Bucket*> {
        if ( should_grow() )
            TRY(try_rehash(capacity() * 2));

        /* pre-calculate the hash value */
        auto hash = TTraits::hash(value);

        /* iterate each bucket to find the first usable */
        Bucket* first_empty_bucket = nullptr;
        for ( ;; ) {
            auto& bucket = m_buckets_storage[hash % m_data_capacity];

            /* use the already used bucket if the value matches */
            if ( Details::is_used_bucket(bucket.m_bucket_state) && TTraits::equals(*bucket.slot(), value) )
                return &bucket;

            /* use a new and empty bucket */
            if ( !Details::is_used_bucket(bucket.m_bucket_state) ) {
                if ( first_empty_bucket == nullptr )
                    first_empty_bucket = &bucket;

                if ( bucket.m_bucket_state != Details::SetBucketState::Deleted )
                    return first_empty_bucket;
            }

            /* double the hash and try again */
            hash = Hashing::rehash_key(hash);
        }
    }
    [[nodiscard]] auto lookup_for_writing(T const& value) -> Bucket& {
        return *MUST(try_lookup_for_writing(value));
    }

    [[nodiscard]] auto used_bucket_count() const -> usize {
        return m_values_count + m_deleted_count;
    }

    [[nodiscard]] auto should_grow() const -> bool {
        return ((used_bucket_count() + 1) * 100) >= (m_data_capacity * LOAD_FACTOR_PERCENT);
    }

    [[nodiscard]] static constexpr auto size_in_bytes(usize capacity) -> usize {
        if constexpr ( IsOrdered )
            return sizeof(Bucket) * capacity;
        else
            return sizeof(Bucket) * (capacity + 1);
    }

    auto remove(Bucket& bucket) -> bool {
        VERIFY(Details::is_used_bucket(bucket.m_bucket_state));

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

} /* namespace Collection */

using Collection::OrderedSet;
using Collection::Set;

} /* namespace TC */
