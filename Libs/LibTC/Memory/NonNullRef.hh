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

#include <LibTC/Forward.hh>

#include <Api/Common.h>
#include <LibTC/Assertions.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/DenyMove.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Must.hh>
#include <LibTC/IntTypes.hh>

#define TC_NON_NULL_REF_CONSTRUCTIBLE(ClassName)                                                                                           \
    template<typename T>                                                                                                                   \
    friend class TC::Memory::Details::RefCounted;                                                                                          \
    template<typename T>                                                                                                                   \
    friend class TC::Memory::NonNullRef

namespace TC {
namespace Memory {
namespace Details {

enum class FromArgsTag {
    FromArgs
};

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
    explicit RefCounted(FromArgsTag, Args... args)
        : m_shared_object{ T{ Cxx::forward<Args>(args)... } } {
    }

    ~RefCounted() = default;

    auto add_strong_reference() const -> void {
        auto old_strong_count = __atomic_fetch_add(&m_strong_ref_count, 1, __ATOMIC_SEQ_CST);
        VERIFY_GREATER_EQUAL(old_strong_count, 1);
    }
    auto remove_strong_reference() const -> void {
        auto old_strong_count = __atomic_fetch_sub(&m_strong_ref_count, 1, __ATOMIC_SEQ_CST);
        VERIFY_GREATER_EQUAL(old_strong_count, 1);

        collect(this);
    }

    /**
     * @brief Getters
     */
    [[nodiscard]] auto shared_object() -> T& {
        return m_shared_object;
    }
    [[nodiscard]] auto shared_object() const -> T const& {
        return m_shared_object;
    }

    [[nodiscard]] auto strong_ref_count() const -> usize {
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
    TC_DENY_COPY(NonNullRef);

public:
    /**
     * @brief Non-error safe Factory functions
     */
    template<typename... TArgs>
    [[nodiscard]] static auto construct_from_args(TArgs... args) -> NonNullRef<T> {
        return MUST(try_construct_from_args(Cxx::forward<TArgs>(args)...));
    }
    [[nodiscard]] static auto construct_from_adopt(Details::RefCounted<T>& ref_counted_ref) -> NonNullRef<T> {
        return NonNullRef<T>{ &ref_counted_ref };
    }

    /**
     * @brief Error safe Factory functions
     */
    template<typename... TArgs>
    [[nodiscard]] static auto try_construct_from_args(TArgs... args) -> ErrorOr<NonNullRef<T>> {
        auto ref_counted_ptr = new (nothrow) Details::RefCounted<T>{ Details::FromArgsTag::FromArgs, Cxx::forward<TArgs>(args)... };
        if ( ref_counted_ptr != nullptr ) [[likely]]
            return NonNullRef<T>{ ref_counted_ptr };
        else
            return Error{ ENOMEM };
    }

    /**
     * @brief Move constructor and move assignment
     */
    NonNullRef(NonNullRef<T>&& rhs) noexcept
        : m_ref_counted_ptr{ Cxx::exchange(rhs.m_ref_counted_ptr, nullptr) } {
    }
    auto operator=(NonNullRef<T>&& rhs) noexcept -> NonNullRef<T>& {
        NonNullRef non_null_ref{ Cxx::move(rhs) };
        swap(non_null_ref);
        return *this;
    }

    ~NonNullRef() {
        if ( m_ref_counted_ptr != nullptr ) [[likely]]
            m_ref_counted_ptr->remove_strong_reference();
    }

    /**
     * @brief Clones the references only increasing the strong reference count
     */
    auto clone() const -> NonNullRef<T> {
        VERIFY_NOT_NULL(m_ref_counted_ptr);
        m_ref_counted_ptr->add_strong_reference();
        return NonNullRef<T>{ m_ref_counted_ptr };
    }

    /**
     * @brief Swaps this ref with another
     */
    void swap(NonNullRef<T>& rhs) noexcept {
        Cxx::swap(m_ref_counted_ptr, rhs.m_ref_counted_ptr);
    }

    /**
     * @brief Access operators
     */
    A_RETURN_NONNULL auto operator->() -> T* {
        return as_ptr();
    }
    A_RETURN_NONNULL auto operator->() const -> T const* {
        return as_ptr();
    }

    auto operator*() -> T& {
        return as_ref();
    }
    auto operator*() const -> T const& {
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
    [[nodiscard]] A_RETURN_NONNULL auto as_ptr() -> T* {
        VERIFY_NOT_NULL(m_ref_counted_ptr);
        return &m_ref_counted_ptr->shared_object();
    }
    [[nodiscard]] A_RETURN_NONNULL auto as_ptr() const -> T const* {
        VERIFY_NOT_NULL(m_ref_counted_ptr);
        return &m_ref_counted_ptr->shared_object();
    }

    [[nodiscard]] auto as_ref() -> T& {
        return *as_ptr();
    }
    [[nodiscard]] auto as_ref() const -> T const& {
        return *as_ptr();
    }

    [[nodiscard]] auto strong_ref_count() const -> usize {
        VERIFY_NOT_NULL(m_ref_counted_ptr);
        return m_ref_counted_ptr->strong_ref_count();
    }

private:
    explicit constexpr NonNullRef(Details::RefCounted<T>* ref_counted_ptr) noexcept
        : m_ref_counted_ptr{ ref_counted_ptr } {
    }

private:
    Details::RefCounted<T>* m_ref_counted_ptr;
};

} /* namespace Memory */

using Memory::NonNullRef;

template<typename T>
struct TypeTraits<NonNullRef<T>> : public Details::TypeTraits<NonNullRef<T>> {
    static constexpr auto hash(NonNullRef<T> const& value) -> usize {
        return Hashing::pointer_calculate_hash(value.as_ptr());
    }

    static constexpr auto is_trivial() -> bool {
        return false;
    }

    static constexpr auto equals(NonNullRef<T> const& a, NonNullRef<T> const& b) -> bool {
        return a.as_ptr() == b.as_ptr();
    }
};

} /* namespace TC */
