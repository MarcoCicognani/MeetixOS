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

#include <Api/Common.h>
#include <LibTC/Assertions.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/DenyMove.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Memory/Tags.hh>
#include <LibTC/Trait/TypeIntrinsics.hh>

namespace TC {
namespace Memory {
namespace Details {

template<typename T>
struct RefCounted {
    TC_DENY_COPY(RefCounted);
    TC_DENY_MOVE(RefCounted);

public:
    /**
     * @brief Constructor
     */
    RefCounted() = delete;
    template<typename... Args>
    RefCounted(FromArgsTag, Args&&... args)
        : m_shared_object{ T{ forward<Args>(args)... } } {
    }

    ~RefCounted() = default;

    void add_strong_reference() const {
        auto old_strong_count = __atomic_fetch_add(&m_strong_ref_count, 1, __ATOMIC_SEQ_CST);
        VERIFY_GREATER_EQUAL(old_strong_count, 1);
    }
    void remove_strong_reference() const {
        auto old_strong_count = __atomic_fetch_sub(&m_strong_ref_count, 1, __ATOMIC_SEQ_CST);
        VERIFY_GREATER_EQUAL(old_strong_count, 1);

        collect(this);
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] T& shared_object() {
        return m_shared_object;
    }
    [[nodiscard]] T const& shared_object() const {
        return m_shared_object;
    }

    [[nodiscard]] usize strong_ref_count() const {
        return __atomic_load_n(&m_strong_ref_count, __ATOMIC_SEQ_CST);
    }

private:
    static void collect(RefCounted<T> const* ref_counted) {
        auto strong_ref_count = __atomic_load_n(&ref_counted->m_strong_ref_count, __ATOMIC_SEQ_CST);
        if ( strong_ref_count == 0 )
            delete ref_counted;
    }

private:
    mutable usize m_strong_ref_count{ 1 };

    T m_shared_object;
};

} /* namespace Details */

template<typename T>
class NonNullRef {
public:
    /**
     * @brief Error safe constructor
     */
    template<typename... Args>
    static ErrorOr<NonNullRef<T>> try_construct_from_args(Args&&... args) {
        auto ref_counted_ptr = new (nothrow) Details::RefCounted<T>{ FromArgs, forward<Args>(args)... };
        if ( ref_counted_ptr != nullptr ) [[likely]]
            return NonNullRef<T>{ Adopt, ref_counted_ptr };
        else
            return Error{ ENOMEM };
    }

    /**
     * @brief Constructors
     */
    NonNullRef() = delete;
    template<typename... Args>
    NonNullRef(FromArgsTag, Args&&... args)
        : m_ref_counted_ptr{ new (nothrow) Details::RefCounted<T>{ FromArgs, forward<Args>(args)... } } {
        VERIFY_NOT_NULL(m_ref_counted_ptr);
    }
    NonNullRef(AdoptTag, Details::RefCounted<T>* ref_counted_ptr)
        : m_ref_counted_ptr{ ref_counted_ptr } {
    }
    NonNullRef(NonNullRef const& rhs)
        : m_ref_counted_ptr{ rhs.m_ref_counted_ptr } {
        if ( m_ref_counted_ptr != nullptr ) [[likely]]
            m_ref_counted_ptr->add_strong_reference();
    }
    NonNullRef(NonNullRef&& rhs) noexcept
        : m_ref_counted_ptr{ exchange(rhs.m_ref_counted_ptr, nullptr) } {
    }

    ~NonNullRef() {
        if ( m_ref_counted_ptr != nullptr ) [[likely]]
            m_ref_counted_ptr->remove_strong_reference();
    }

    NonNullRef& operator=(NonNullRef const& rhs) {
        if ( this == &rhs )
            return *this;

        NonNullRef non_null_ref{ rhs };
        swap(non_null_ref);
        return *this;
    }
    NonNullRef& operator=(NonNullRef&& rhs) noexcept {
        NonNullRef non_null_ref{ move(rhs) };
        swap(non_null_ref);
        return *this;
    }

    /**
     * @brief Swaps this ref with another
     */
    void swap(NonNullRef& rhs) noexcept {
        Cxx::swap(m_ref_counted_ptr, rhs.m_ref_counted_ptr);
    }

    /**
     * @brief Access operators
     */
    A_RETURN_NONNULL T* operator->() {
        return as_ptr();
    }
    A_RETURN_NONNULL T const* operator->() const {
        return as_ptr();
    }

    T& operator*() {
        return as_ref();
    }
    T const& operator*() const {
        return as_ref();
    }

    /**
     * @brief Conversion operators
     */
    A_RETURN_NONNULL explicit operator T*() {
        return as_ptr();
    }
    A_RETURN_NONNULL explicit operator T const*() const {
        return as_ptr();
    }

    explicit operator T&() {
        return as_ref();
    }
    explicit operator T const&() const {
        return as_ref();
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] A_RETURN_NONNULL T* as_ptr() {
        VERIFY_NOT_NULL(m_ref_counted_ptr);
        return &m_ref_counted_ptr->shared_object();
    }
    [[nodiscard]] A_RETURN_NONNULL T const* as_ptr() const {
        VERIFY_NOT_NULL(m_ref_counted_ptr);
        return &m_ref_counted_ptr->shared_object();
    }

    [[nodiscard]] T& as_ref() {
        return *as_ptr();
    }
    [[nodiscard]] T const& as_ref() const {
        return *as_ptr();
    }

    [[nodiscard]] usize strong_ref_count() {
        VERIFY_NOT_NULL(m_ref_counted_ptr);
        return m_ref_counted_ptr->strong_ref_count();
    }

private:
    Details::RefCounted<T>* m_ref_counted_ptr;
};

} /* namespace Memory */

using Memory::NonNullRef;

namespace Trait {

template<typename T>
struct TypeIntrinsics<NonNullRef<T>> : public Details::TypeIntrinsics<NonNullRef<T>> {
    static constexpr usize hash(NonNullRef<T> const& value) {
        return Hashing::pointer_calculate_hash(value.as_ptr());
    }

    static constexpr bool is_trivial() {
        return false;
    }

    static constexpr bool equals(NonNullRef<T> const& a, NonNullRef<T> const& b) {
        return a.as_ptr() == b.as_ptr();
    }
};

} /* namespace Trait */
} /* namespace TC */
