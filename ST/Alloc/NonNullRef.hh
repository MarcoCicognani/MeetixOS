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

#include <ST/Forward.hh>

#include <ST/Core/Assertions.hh>
#include <ST/Core/ErrorOr.hh>
#include <ST/Lang/Cxx.hh>
#include <ST/Lang/DenyCopy.hh>
#include <ST/Lang/DenyMove.hh>
#include <ST/Lang/IntTypes.hh>
#include <ST/Lang/Must.hh>

#define TCNonNullRefConstructible$(ClassName) \
    template<typename T>                      \
    friend class Details::RefCounted;         \
    template<typename T>                      \
    friend class NonNullRef

namespace Details {

enum class FromArgsTag {
    FromArgs
};
using enum FromArgsTag;

template<typename T>
struct RefCounted final {
    TCDenyCopy$(RefCounted);
    TCDenyMove$(RefCounted);

public:
    /**
     * @brief Constructor
     */
    RefCounted() = delete;
    template<typename... Args>
    explicit RefCounted(FromArgsTag, Args&&... args)
        : m_shared_object{ T{ Cxx::forward<Args>(args)... } } {
    }

    ~RefCounted() = default;

    [[gnu::always_inline]]
    auto add_strong_reference() const -> void {
        auto const old_strong_count = __atomic_fetch_add(&m_strong_ref_count, 1, __ATOMIC_SEQ_CST);
        verify_greater_equal$(old_strong_count, 1);
    }
    [[gnu::always_inline]]
    auto remove_strong_reference() const -> void {
        auto const old_strong_count = __atomic_fetch_sub(&m_strong_ref_count, 1, __ATOMIC_SEQ_CST);
        verify_greater_equal$(old_strong_count, 1);

        collect(this);
    }

    /**
     * @brief Getters
     */
    [[nodiscard]]
    auto shared_object() -> T& {
        return m_shared_object;
    }
    [[nodiscard]]
    auto shared_object() const -> T const& {
        return m_shared_object;
    }

    [[nodiscard]]
    auto strong_ref_count() const -> usize {
        return __atomic_load_n(&m_strong_ref_count, __ATOMIC_SEQ_CST);
    }

private:
    static inline auto collect(RefCounted<T> const* ref_counted) -> void {
        auto const strong_ref_count = ref_counted->strong_ref_count();
        if ( strong_ref_count == 0 )
            delete ref_counted;
    }

private:
    mutable usize m_strong_ref_count{ 1 };

    T m_shared_object;
};

} /* namespace Details */

template<typename T>
class NonNullRef final {
    TCDenyCopy$(NonNullRef);

public:
    /**
     * @brief Non-Error safe factory functions
     */
    template<typename... TArgs>
    [[nodiscard]]
    static auto new_from_emplace(TArgs&&... args) -> NonNullRef<T> {
        return must$(try_new_from_emplace(Cxx::forward<TArgs>(args)...));
    }
    [[nodiscard]]
    static auto new_from_adopt(Details::RefCounted<T>& ref_counted_ref) -> NonNullRef<T> {
        return NonNullRef<T>{ &ref_counted_ref };
    }

    /**
     * @brief Error safe Factory functions
     */
    template<typename... TArgs>
    [[nodiscard]]
    static auto try_new_from_emplace(TArgs&&... args) -> ErrorOr<NonNullRef<T>> {
        auto ref_counted_ptr = new (nothrow) Details::RefCounted<T>{ Details::FromArgs, Cxx::forward<TArgs>(args)... };
        if ( ref_counted_ptr != nullptr ) [[likely]]
            return NonNullRef<T>{ ref_counted_ptr };
        else
            return Error::new_from_code(ErrorCode::NoMemory);
    }

    /**
     * @brief Move constructor and move assignment
     */
    NonNullRef(NonNullRef<T>&& rhs)
        : m_ref_counted_ptr{ Cxx::exchange(rhs.m_ref_counted_ptr, nullptr) } {
    }
    auto operator=(NonNullRef<T>&& rhs) -> NonNullRef<T>& {
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
        verify_not_null$(m_ref_counted_ptr);
        m_ref_counted_ptr->add_strong_reference();
        return NonNullRef<T>{ m_ref_counted_ptr };
    }

    /**
     * @brief Swaps this ref with another
     */
    void swap(NonNullRef<T>& rhs) {
        Cxx::swap(m_ref_counted_ptr, rhs.m_ref_counted_ptr);
    }

    /**
     * @brief Access operators
     */
    [[gnu::returns_nonnull]]
    auto operator->() -> T* {
        return as_ptr();
    }
    [[gnu::returns_nonnull]]
    auto operator->() const -> T const* {
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
    [[gnu::returns_nonnull]]
    explicit operator T*() {
        return as_ptr();
    }
    [[gnu::returns_nonnull]]
    explicit operator T const*() const {
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
    [[nodiscard, gnu::returns_nonnull]]
    auto as_ptr() -> T* {
        verify_not_null$(m_ref_counted_ptr);
        return &m_ref_counted_ptr->shared_object();
    }
    [[nodiscard, gnu::returns_nonnull]]
    auto as_ptr() const -> T const* {
        verify_not_null$(m_ref_counted_ptr);
        return &m_ref_counted_ptr->shared_object();
    }

    [[nodiscard]]
    auto as_ref() -> T& {
        return *as_ptr();
    }
    [[nodiscard]]
    auto as_ref() const -> T const& {
        return *as_ptr();
    }

    [[nodiscard]]
    auto strong_ref_count() const -> usize {
        verify_not_null$(m_ref_counted_ptr);
        return m_ref_counted_ptr->strong_ref_count();
    }

private:
    explicit constexpr NonNullRef(Details::RefCounted<T>* ref_counted_ptr)
        : m_ref_counted_ptr{ ref_counted_ptr } {
    }

private:
    Details::RefCounted<T>* m_ref_counted_ptr;
};

template<typename T>
struct TypeTraits<NonNullRef<T>> final : public Details::TypeTraits<NonNullRef<T>> {
    static constexpr auto hash(NonNullRef<T> const& value) -> usize {
        return Hashing::hash_ptr(value.as_ptr());
    }

    static constexpr auto is_trivial() -> bool {
        return false;
    }

    static constexpr auto equals(NonNullRef<T> const& a, NonNullRef<T> const& b) -> bool {
        return a.as_ptr() == b.as_ptr();
    }
};
