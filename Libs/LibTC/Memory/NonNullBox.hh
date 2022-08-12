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

#include <LibTC/Assertions.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/DenyCopy.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Functional/Must.hh>

#define TC_NON_NULL_BOX_CONSTRUCTIBLE(ClassName)                                                                                           \
    template<typename T>                                                                                                                   \
    friend class Memory::NonNullBox

namespace TC {
namespace Memory {

template<typename T>
class NonNullBox {
    TC_DENY_COPY(NonNullBox);

public:
    /**
     * @brief Non-error safe Factory functions
     */
    template<typename... TArgs>
    [[nodiscard]] static auto construct_from_args(TArgs... args) -> NonNullBox<T> {
        return MUST(try_construct_from_args(Cxx::forward<TArgs>(args)...));
    }
    [[nodiscard]] static auto construct_from_adopt(T& unboxed_ref) -> NonNullBox<T> {
        return NonNullBox<T>{ &unboxed_ref };
    }

    /**
     * @brief Error safe Factory functions
     */
    template<typename... TArgs>
    [[nodiscard]] static auto try_construct_from_args(TArgs... args) -> ErrorOr<NonNullBox<T>> {
        auto unboxed_ptr = new (nothrow) T{ forward<TArgs>(args)... };
        if ( unboxed_ptr != nullptr ) [[likely]]
            return NonNullBox<T>{ unboxed_ptr };
        else
            return Error{ ENOMEM };
    }

    /**
     * @brief Move constructor and move assignment
     */
    NonNullBox(NonNullBox<T>&& rhs) noexcept
        : m_boxed_object_ptr{ exchange(rhs.m_boxed_object_ptr, nullptr) } {
    }
    auto operator=(NonNullBox<T>&& rhs) noexcept -> NonNullBox<T>& {
        NonNullBox non_null_box{ move(rhs) };
        swap(non_null_box);
        return *this;
    }

    ~NonNullBox() {
        delete m_boxed_object_ptr;
    }

    /**
     * @brief Swaps this box with another
     */
    void swap(NonNullBox<T>& rhs) noexcept {
        Cxx::swap(m_boxed_object_ptr, rhs.m_boxed_object_ptr);
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
        VERIFY_NOT_NULL(m_boxed_object_ptr);
        return m_boxed_object_ptr;
    }
    [[nodiscard]] A_RETURN_NONNULL auto as_ptr() const -> T const* {
        VERIFY_NOT_NULL(m_boxed_object_ptr);
        return m_boxed_object_ptr;
    }

    [[nodiscard]] auto as_ref() -> T& {
        return *as_ptr();
    }
    [[nodiscard]] auto as_ref() const -> T const& {
        return *as_ptr();
    }

private:
    explicit constexpr NonNullBox(T* unboxed_ptr) noexcept
        : m_boxed_object_ptr{ unboxed_ptr } {
    }

private:
    T* m_boxed_object_ptr{ nullptr };
};

} /* namespace Memory */

using Memory::NonNullBox;

template<typename T>
struct TypeTraits<NonNullBox<T>> : public Details::TypeTraits<NonNullBox<T>> {
    static constexpr auto hash(NonNullBox<T> const& value) -> usize {
        return Hashing::pointer_calculate_hash(value.as_ptr());
    }

    static constexpr auto is_trivial() -> bool {
        return false;
    }

    static constexpr auto equals(NonNullBox<T> const& a, NonNullBox<T> const& b) -> bool {
        return a.as_ptr() == b.as_ptr();
    }
};

} /* namespace TC */
