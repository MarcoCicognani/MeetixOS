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
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Functional/Try.hh>
#include <LibTC/Hashing.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Math.hh>
#include <LibTC/Memory/Raw.hh>
#include <LibTC/Trait/Conditional.hh>
#include <LibTC/Trait/Hashable.hh>
#include <LibTC/Trait/TypeIntrinsics.hh>

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

constexpr bool is_used_bucket(SetBucketState state) {
    return (static_cast<u8>(state) & 0xf0) == 0x10;
}

constexpr bool is_free_bucket(SetBucketState state) {
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

    SetIterator& operator=(SetIterator const&) = default;

    /**
     * @brief Increment operators
     */
    SetIterator& operator++() {
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
    SetIterator operator++(int) {
        SetIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    T& operator*() {
        return *operator->();
    }
    T const& operator*() const {
        return *operator->();
    }

    /**
     * @brief Pointer access operators
     */
    T* operator->() {
        VERIFY_NOT_NULL(m_current_bucket);
        return m_current_bucket->slot();
    }
    T const* operator->() const {
        VERIFY_NOT_NULL(m_current_bucket);
        return m_current_bucket->slot();
    }

    /**
     * @brief Comparison operators
     */
    bool operator==(SetIterator const& rhs) const {
        return m_current_bucket == rhs.m_current_bucket;
    }
    bool operator!=(SetIterator const& rhs) const {
        return m_current_bucket != rhs.m_current_bucket;
    }
    bool operator<=>(SetIterator const& rhs) const = default;

private:
    BucketType* m_current_bucket{ nullptr };
};

template<typename Collection, typename T, typename BucketType>
class OrderedSetIterator {
public:
    /**
     * @brief Constructors
     */
    explicit OrderedSetIterator(BucketType* bucket)
        : m_current_bucket{ bucket } {
    }
    OrderedSetIterator(OrderedSetIterator const&) = default;

    OrderedSetIterator& operator=(OrderedSetIterator const&) = default;

    /**
     * @brief Increment operators
     */
    OrderedSetIterator& operator++() {
        if ( m_current_bucket == nullptr )
            return *this;

        m_current_bucket = m_current_bucket->m_next;
        return *this;
    }
    OrderedSetIterator operator++(int) {
        OrderedSetIterator it{ *this };

        operator++();
        return it;
    }

    /**
     * @brief Decrement operators
     */
    /**
     * @brief Increment operators
     */
    OrderedSetIterator& operator--() {
        if ( m_current_bucket == nullptr )
            return *this;

        m_current_bucket = m_current_bucket->m_previous;
        return *this;
    }
    OrderedSetIterator operator--(int) {
        OrderedSetIterator it{ *this };

        operator--();
        return it;
    }

    /**
     * @brief ValueReference access operators
     */
    T& operator*() {
        return *operator->();
    }
    T const& operator*() const {
        return *operator->();
    }

    /**
     * @brief Pointer access operators
     */
    T* operator->() {
        VERIFY_NOT_NULL(m_current_bucket);
        return m_current_bucket->slot();
    }
    T const* operator->() const {
        VERIFY_NOT_NULL(m_current_bucket);
        return m_current_bucket->slot();
    }

    /**
     * @brief Comparison operators
     */
    bool operator==(OrderedSetIterator const& rhs) const {
        return m_current_bucket == rhs.m_current_bucket;
    }
    bool operator!=(OrderedSetIterator const& rhs) const {
        return m_current_bucket != rhs.m_current_bucket;
    }
    bool operator<=>(OrderedSetIterator const& rhs) const = default;

private:
    BucketType* m_current_bucket{ nullptr };
};

template<typename T>
struct SetBucket {
    T* slot() {
        return bit_cast<T*>(&m_storage);
    }
    T const* slot() const {
        return bit_cast<T const*>(&m_storage);
    }

    SetBucketState m_bucket_state;
    alignas(T) u8 m_storage[sizeof(T)];
};

template<typename T>
struct OrderedSetBucket {
    T* slot() {
        return bit_cast<T*>(&m_storage);
    }
    T const* slot() const {
        return bit_cast<T const*>(&m_storage);
    }

    OrderedSetBucket* m_previous;
    OrderedSetBucket* m_next;
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

template<Hashable T, typename TIntrinsics, bool IsOrdered>
class BaseSet {
private:
    using BucketType         = Conditional<IsOrdered, Details::OrderedSetBucket<T>, Details::SetBucket<T>>;
    using CollectionDataType = Conditional<IsOrdered, Details::OrderedCollectionData<BucketType>, Details::CollectionData>;

public:
    static constexpr usize LOAD_FACTOR_PERCENT = 60;

    using Iterator
        = Conditional<IsOrdered, Details::OrderedSetIterator<BaseSet, T, BucketType>, Details::SetIterator<BaseSet, T, BucketType>>;
    using ConstIterator = Conditional<IsOrdered,
                                      Details::OrderedSetIterator<BaseSet const, T const, BucketType const>,
                                      Details::SetIterator<BaseSet const, T const, BucketType const>>;

public:
    /**
     * @brief Constructors
     */
    BaseSet() = default;
    explicit BaseSet(usize capacity) {
        rehash(capacity);
    }
    BaseSet(BaseSet const& rhs) {
        rehash(rhs.capacity());
        for ( auto& value : rhs )
            set(value);
    }
    BaseSet(BaseSet&& rhs) noexcept
        : m_buckets_storage{ exchange(rhs.m_buckets_storage, nullptr) }
        , m_collection_data{ exchange(rhs.m_collection_data, CollectionDataType{}) }
        , m_data_capacity{ exchange(rhs.m_data_capacity, 0) }
        , m_values_count{ exchange(rhs.m_values_count, 0) }
        , m_deleted_count{ exchange(rhs.m_deleted_count, 0) } {
    }
    BaseSet(std::initializer_list<T> initializer_list) {
        ensure_capacity(initializer_list.size());
        for ( auto const& value : initializer_list )
            insert(Cxx::move(value));
    }

    ~BaseSet() {
        clear(KeepStorageCapacity::No);
    }

    BaseSet& operator=(BaseSet const& rhs) {
        if ( this == &rhs )
            return *this;

        BaseSet set{ rhs };
        swap(set);
        return *this;
    }
    BaseSet& operator=(BaseSet&& rhs) noexcept {
        BaseSet set{ move(rhs) };
        swap(set);
        return *this;
    }

    /**
     * @brief Destroys all the stored values keeping the capacity of this set if requested
     */
    void clear(KeepStorageCapacity keep_storage_capacity = KeepStorageCapacity::Yes) {
        if ( m_buckets_storage == nullptr )
            return;

        /* call the destructors only for non-trivial types */
        if constexpr ( !TIntrinsics::is_trivial() ) {
            for ( usize i : Range{ 0u, m_data_capacity } ) {
                if ( Details::is_used_bucket(m_buckets_storage[i].m_bucket_state) )
                    m_buckets_storage[i].slot()->~T();
            }
        }

        /* free the memory if requested */
        if ( keep_storage_capacity == KeepStorageCapacity::Yes ) {
            __builtin_memset(m_buckets_storage, 0, size_in_bytes(capacity()));

            if constexpr ( IsOrdered )
                m_collection_data = Details::OrderedCollectionData<BucketType>{ nullptr, nullptr };
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
    void swap(BaseSet& rhs) noexcept {
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
    template<typename U = T>
    InsertResult insert(U&& value, OnExistingEntry existing_entry_behaviour = OnExistingEntry::Replace) {
        return MUST(try_insert(forward<U>(value), existing_entry_behaviour));
    }
    template<typename U = T>
    ErrorOr<InsertResult> try_insert(U&& value, OnExistingEntry existing_entry_behavior = OnExistingEntry::Replace) {
        /* find a bucket which is suitable for writing the value */
        auto* bucket = TRY(try_lookup_for_writing(value));
        if ( Details::is_used_bucket(bucket->m_bucket_state) ) {
            /* keep the existing value and return */
            if ( existing_entry_behavior == OnExistingEntry::Keep )
                return InsertResult::KeptExisting;

            /* replace the value with the given one */
            (*bucket->slot()) = forward<U>(value);
            return InsertResult::ReplacedExisting;
        }

        /* overwrite the already existing value and decrease the deleted count */
        new (bucket->slot()) T{ forward<U>(value) };
        if ( bucket->m_bucket_state == Details::SetBucketState::Deleted )
            --m_deleted_count;

        /* update his state */
        bucket->m_bucket_state = Details::SetBucketState::Used;

        /* update linked list of ordered values if is ordered */
        if constexpr ( IsOrdered ) {
            if ( m_collection_data.m_head == nullptr ) [[unlikely]] {
                m_collection_data.m_head = bucket;
            } else {
                bucket->m_previous               = m_collection_data.m_tail;
                m_collection_data.m_tail->m_next = bucket;
            }
            m_collection_data.m_tail = bucket;
        }

        ++m_values_count;
        return InsertResult::InsertedNew;
    }

    /**
     * @brief Removes the given value from this set and returns whether it was existing
     */
    bool remove(T const& value) {
        auto bucket = lookup_with_hash(TIntrinsics::hash(value),
                                       [&value](auto const& current) -> bool { return TIntrinsics::equals(value, current); });
        if ( bucket != nullptr )
            return remove(*bucket);
        else
            return false;
    }

    /**
     * @brief Removes the element referenced by the given iterator
     */
    bool remove(Iterator iterator) {
        if ( iterator != end() )
            return remove(*iterator.m_bucket);
        else
            return false;
    }

    /**
     * @brief Removes all the elements for which the given call_back returns true
     */
    template<typename TPredicate>
    usize remove_all_matching(TPredicate predicate) {
        /* iterate all the used buckets and give them to the given call_back */
        usize removed_count = 0;
        for ( usize i : Range{ 0u, m_data_capacity } ) {
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
    void ensure_capacity(usize capacity) {
        VERIFY_GREATER_EQUAL(capacity, count());
        rehash(capacity * 2);
    }
    ErrorOr<void> try_ensure_capacity(usize capacity) {
        VERIFY_GREATER_EQUAL(capacity, count());
        return try_rehash(capacity * 2);
    }

    /**
     * @brief for-each support
     */
    Iterator begin() {
        if constexpr ( IsOrdered )
            return Iterator{ m_collection_data.m_head };
        else {
            /* find the first used bucket */
            for ( usize i : Range{ 0u, m_data_capacity } ) {
                if ( Details::is_used_bucket(m_buckets_storage[i].m_bucket_state) )
                    return Iterator{ &m_buckets_storage[i] };
            }
            return end();
        }
    }
    Iterator end() {
        return Iterator{ nullptr };
    }

    ConstIterator begin() const {
        if constexpr ( IsOrdered )
            return ConstIterator{ m_collection_data.m_head };
        else {
            /* find the first used bucket */
            for ( usize i : Range{ 0u, m_data_capacity } ) {
                if ( Details::is_used_bucket(m_buckets_storage[i].m_bucket_state) )
                    return ConstIterator{ &m_buckets_storage[i] };
            }
            return end();
        }
    }
    ConstIterator end() const {
        return ConstIterator{ nullptr };
    }

    /**
     * @brief Returns whether this vector contains the given value
     */
    [[nodiscard]] bool contains(T const& value) const {
        return find(value).is_present();
    }

    /**
     * @brief Returns a reference to the element into this Vector if exists
     */
    Option<T&> find(T const& value) {
        return find(TIntrinsics::hash(value), [&value](auto const& current) -> bool { return TIntrinsics::equals(value, current); });
    }
    Option<T const&> find(T const& value) const {
        return find(TIntrinsics::hash(value), [&value](auto const& current) -> bool { return TIntrinsics::equals(value, current); });
    }

    /**
     * @brief Returns a reference to the element for which the given call_back return true
     */
    template<typename TPredicate>
    Option<T&> find(usize hash, TPredicate predicate) {
        BucketType* bucket_type = lookup_with_hash(hash, move(predicate));
        if ( bucket_type != nullptr )
            return *bucket_type->slot();
        else
            return {};
    }
    template<typename TPredicate>
    Option<T const&> find(usize hash, TPredicate predicate) const {
        BucketType* bucket_type = lookup_with_hash(hash, move(predicate));
        if ( bucket_type != nullptr )
            return *bucket_type->slot();
        else
            return {};
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] usize count() const {
        return m_values_count;
    }
    [[nodiscard]] usize capacity() const {
        return m_data_capacity;
    }
    [[nodiscard]] bool is_empty() const {
        return m_values_count == 0;
    }

private:
    void insert_during_rehash(T&& value) {
        auto& bucket = lookup_for_writing(value);
        new (bucket.slot()) T{ move(value) };
        bucket.m_bucket_state = Details::SetBucketState::Used;

        /* link to the linked list if the set is ordered */
        if constexpr ( IsOrdered ) {
            if ( !m_collection_data.m_head ) [[unlikely]] {
                m_collection_data.m_head = &bucket;
            } else {
                bucket.m_previous                = m_collection_data.m_tail;
                m_collection_data.m_tail->m_next = &bucket;
            }
            m_collection_data.m_tail = &bucket;
        }
    }

    void rehash(usize new_capacity) {
        MUST(try_rehash(new_capacity));
    }
    ErrorOr<void> try_rehash(usize new_capacity) {
        if ( new_capacity == m_data_capacity && new_capacity >= 4 ) {
            rehash_in_place();
            return {};
        }

        /* roundup the given new capacity */
        new_capacity = Math::max(new_capacity, 4u);
        new_capacity = new_capacity * sizeof(BucketType) / sizeof(BucketType);

        /* keep old references */
        auto* old_buckets  = m_buckets_storage;
        auto  old_capacity = m_data_capacity;
        auto  old_iter     = begin();

        /* allocate the new memory */
        m_buckets_storage = TRY(Memory::Raw::clean_alloc_object<BucketType>(size_in_bytes(new_capacity)));
        m_data_capacity   = new_capacity;
        m_deleted_count   = 0;

        /* update the collection */
        if constexpr ( IsOrdered )
            m_collection_data = Details::OrderedCollectionData<BucketType>{ nullptr, nullptr };
        else
            m_buckets_storage[m_data_capacity].m_bucket_state = Details::SetBucketState::End;

        /* return if this set was emtpy */
        if ( old_buckets == nullptr )
            return {};

        /* move the values to the new memory */
        for ( auto it = move(old_iter); it != end(); ++it ) {
            insert_during_rehash(move(*it));
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
        for ( usize i : Range{ 0u, m_data_capacity } ) {
            auto& bucket = m_buckets_storage[i];

            if ( bucket.m_bucket_state == Details::SetBucketState::Rehashed || bucket.m_bucket_state == Details::SetBucketState::End
                 || bucket.m_bucket_state == Details::SetBucketState::Free )
                continue;
            if ( bucket.m_bucket_state == Details::SetBucketState::Deleted ) {
                bucket.m_bucket_state = Details::SetBucketState::Free;
                continue;
            }

            auto const new_hash = TIntrinsics::hash(*bucket.slot());
            if ( new_hash % m_data_capacity == i ) {
                bucket.m_bucket_state = Details::SetBucketState::Rehashed;
                continue;
            }

            auto        target_hash    = new_hash;
            auto const  to_move_hash   = i;
            BucketType* target_bucket  = &m_buckets_storage[target_hash % m_data_capacity];
            BucketType* bucket_to_move = &m_buckets_storage[i];

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
                    new (target_bucket->slot()) T(move(*bucket_to_move->slot()));
                    target_bucket->m_bucket_state  = Details::SetBucketState::Rehashed;
                    bucket_to_move->m_bucket_state = Details::SetBucketState::Free;

                    if constexpr ( IsOrdered ) {
                        Cxx::swap(bucket_to_move->m_previous, target_bucket->m_previous);
                        Cxx::swap(bucket_to_move->m_next, target_bucket->m_next);

                        if ( target_bucket->m_previous )
                            target_bucket->m_previous->m_next = target_bucket;
                        else
                            m_collection_data.m_head = target_bucket;
                        if ( target_bucket->m_next )
                            target_bucket->m_next->m_previous = target_bucket;
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
                        Cxx::swap(bucket_to_move->m_previous, target_bucket->m_previous);
                        Cxx::swap(bucket_to_move->m_next, target_bucket->m_next);

                        if ( target_bucket->m_previous )
                            target_bucket->m_previous->m_next = target_bucket;
                        else
                            m_collection_data.m_head = target_bucket;
                        if ( target_bucket->m_next )
                            target_bucket->m_next->m_previous = target_bucket;
                        else
                            m_collection_data.m_tail = target_bucket;
                    }

                    target_hash   = TIntrinsics::hash(*bucket_to_move->slot());
                    target_bucket = &m_buckets_storage[target_hash % m_data_capacity];

                    /* the data is already in the correct location: Adjust the pointers */
                    if ( target_hash % m_data_capacity == to_move_hash ) {
                        bucket_to_move->m_bucket_state = Details::SetBucketState::Rehashed;
                        if constexpr ( IsOrdered ) {
                            /* update state for the bucket to move as it's not actually moved anymore */
                            if ( bucket_to_move->m_previous )
                                bucket_to_move->m_previous->m_next = bucket_to_move;
                            else
                                m_collection_data.m_head = bucket_to_move;
                            if ( bucket_to_move->m_next )
                                bucket_to_move->m_next->m_previous = bucket_to_move;
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

        for ( usize i : Range{ 0u, m_data_capacity } ) {
            if ( m_buckets_storage[i].m_bucket_state == Details::SetBucketState::Rehashed )
                m_buckets_storage[i].m_bucket_state = Details::SetBucketState::Used;
        }

        m_deleted_count = 0;
    }

    template<typename TPredicate>
    [[nodiscard]] BucketType* lookup_with_hash(u32 hash, TPredicate predicate) const {
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

    ErrorOr<BucketType*> try_lookup_for_writing(T const& value) {
        if ( should_grow() )
            TRY(try_rehash(capacity() * 2));

        /* pre-calculate the hash value */
        auto hash = TIntrinsics::hash(value);

        /* iterate each bucket to find the first usable */
        BucketType* first_empty_bucket = nullptr;
        for ( ;; ) {
            auto& bucket = m_buckets_storage[hash % m_data_capacity];

            /* use the already used bucket if the value matches */
            if ( Details::is_used_bucket(bucket.m_bucket_state) && TIntrinsics::equals(*bucket.slot(), value) )
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
    [[nodiscard]] BucketType& lookup_for_writing(T const& value) {
        return *MUST(try_lookup_for_writing(value));
    }

    [[nodiscard]] usize used_bucket_count() const {
        return m_values_count + m_deleted_count;
    }

    [[nodiscard]] bool should_grow() const {
        return ((used_bucket_count() + 1) * 100) >= (m_data_capacity * LOAD_FACTOR_PERCENT);
    }

    [[nodiscard]] static constexpr usize size_in_bytes(usize capacity) {
        if constexpr ( IsOrdered )
            return sizeof(BucketType) * capacity;
        else
            return sizeof(BucketType) * (capacity + 1);
    }

    bool remove(BucketType& bucket) {
        VERIFY(Details::is_used_bucket(bucket.m_bucket_state));

        delete_bucket(bucket);
        --m_values_count;
        ++m_deleted_count;
        rehash_in_place_if_needed();
        return true;
    }

    void delete_bucket(BucketType& bucket) {
        bucket.slot()->~T();
        bucket.m_bucket_state = Details::SetBucketState::Deleted;

        if constexpr ( IsOrdered ) {
            if ( bucket.m_previous != nullptr )
                bucket.m_previous->m_next = bucket.m_next;
            else
                m_collection_data.m_head = bucket.m_next;

            if ( bucket.m_next != nullptr )
                bucket.m_next->m_previous = bucket.m_previous;
            else
                m_collection_data.m_tail = bucket.m_previous;
        }
    }

    BucketType*        m_buckets_storage{ nullptr };
    CollectionDataType m_collection_data;
    usize              m_data_capacity{ 0 };
    usize              m_values_count{ 0 };
    usize              m_deleted_count{ 0 };
};

template<Hashable T>
using Set = BaseSet<T, TypeIntrinsics<T>, false>;

template<Hashable T>
using OrderedSet = BaseSet<T, TypeIntrinsics<T>, true>;

} /* namespace Collection */

using Collection::InsertResult;
using Collection::KeepStorageCapacity;
using Collection::OnExistingEntry;
using Collection::OrderedSet;
using Collection::Set;

} /* namespace TC */
