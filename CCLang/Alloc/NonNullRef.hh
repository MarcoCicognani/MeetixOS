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

#include <CCLang/Forward.hh>

#include <CCLang/Alloc/RefCounted.hh>
#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/DenyMove.hh>
#include <CCLang/Lang/IntTypes.hh>
#include <CCLang/Lang/Must.hh>

template<typename T>
class NonNullRef final : public DenyCopy {
public:
    /**
     * @brief Non-Error safe factory functions
     */
    template<typename... TArgs>
    [[nodiscard]]
    static auto new_from_emplace(TArgs&&... args) -> NonNullRef<T> {
        return must$(try_new_from_emplace(Cxx::forward<TArgs>(args)...));
    }

    /**
     * @brief Error safe Factory functions
     */
    template<typename... TArgs>
    static auto try_new_from_emplace(TArgs&&... args) -> ErrorOr<NonNullRef<T>> {
        auto ref_counted_ptr = new (nothrow) T{ Cxx::forward<TArgs>(args)... };
        if ( ref_counted_ptr != nullptr ) [[likely]]
            return NonNullRef<T>{ ref_counted_ptr };
        else
            return Error::new_from_code(ErrorCode::NoMemory);
    }
    static auto try_new_from_adopt(T* ref_counted_ptr) -> ErrorOr<NonNullRef<T>> {
        if ( ref_counted_ptr != nullptr ) [[likely]]
            return NonNullRef<T>{ ref_counted_ptr };
        else
            return Error::new_from_code(ErrorCode::NullPointer);
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
            m_ref_counted_ptr->remove_strong_ref();
    }

    /**
     * @brief Clones the references only increasing the strong reference count
     */
    auto clone() const -> NonNullRef<T> {
        verify_not_null$(m_ref_counted_ptr);
        m_ref_counted_ptr->add_strong_ref();
        return NonNullRef<T>{ m_ref_counted_ptr };
    }

    /**
     * @brief Swaps this ref with another
     */
    constexpr void swap(NonNullRef<T>& rhs) {
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
        return m_ref_counted_ptr;
    }
    [[nodiscard, gnu::returns_nonnull]]
    auto as_ptr() const -> T const* {
        verify_not_null$(m_ref_counted_ptr);
        return m_ref_counted_ptr;
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
    explicit constexpr NonNullRef(T* ref_counted_ptr)
        : m_ref_counted_ptr{ ref_counted_ptr } {
    }

private:
    T* m_ref_counted_ptr;
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

namespace Cxx {

template<typename T>
constexpr auto swap(NonNullRef<T>& lhs, NonNullRef<T>& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
