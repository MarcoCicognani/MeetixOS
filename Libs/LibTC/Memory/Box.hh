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

#define TC_BOX_CONSTRUCTIBLE(ClassName)                                                                                                                        \
    template<typename T>                                                                                                                                       \
    friend class TC::Memory::Box

namespace TC {
namespace Memory {

template<typename T>
class Box {
    TC_DENY_COPY(Box);

public:
    /**
     * @brief Non-error safe Factory functions
     */
    template<typename... TArgs>
    [[nodiscard]] static auto construct_from_args(TArgs... args) -> Box<T> {
        return MUST(try_construct_from_args(Cxx::forward<TArgs>(args)...));
    }
    [[nodiscard]] static auto construct_from_adopt(T& unboxed_ref) -> Box<T> {
        return Box<T>{ &unboxed_ref };
    }

    /**
     * @brief Error safe Factory functions
     */
    template<typename... TArgs>
    [[nodiscard]] static auto try_construct_from_args(TArgs... args) -> ErrorOr<Box<T>> {
        auto unboxed_ptr = new (nothrow) T{ Cxx::forward<TArgs>(args)... };
        if ( unboxed_ptr != nullptr ) [[likely]]
            return Box<T>{ unboxed_ptr };
        else
            return Error{ ENOMEM };
    }

    /**
     * @brief Move constructor and move assignment
     */
    Box(Box<T>&& rhs) noexcept
        : m_boxed_object_ptr{ Cxx::exchange(rhs.m_boxed_object_ptr, nullptr) } {
    }
    auto operator=(Box<T>&& rhs) noexcept -> Box<T>& {
        Box non_null_box{ Cxx::move(rhs) };
        swap(non_null_box);
        return *this;
    }

    ~Box() {
        if ( !is_null() ) [[likely]]
            delete m_boxed_object_ptr;
    }

    /**
     * @brief Swaps this box with another
     */
    void swap(Box<T>& rhs) noexcept {
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

    [[nodiscard]] auto leak_ptr() -> T* {
        auto boxed_object_ptr = m_boxed_object_ptr;
        m_boxed_object_ptr    = nullptr;
        return boxed_object_ptr;
    }
    [[nodiscard]] auto leak_ref() -> T& {
        VERIFY_NOT_NULL(m_boxed_object_ptr);
        return *leak_ptr();
    }

    [[nodiscard]] auto is_null() -> bool {
        return m_boxed_object_ptr == nullptr;
    }

private:
    explicit constexpr Box(T* unboxed_ptr) noexcept
        : m_boxed_object_ptr{ unboxed_ptr } {
    }

private:
    T* m_boxed_object_ptr{ nullptr };
};

} /* namespace Memory */

using Memory::Box;

template<typename T>
struct TypeTraits<Box<T>> : public Details::TypeTraits<Box<T>> {
    static constexpr auto hash(Box<T> const& value) -> usize {
        return Hashing::pointer_calculate_hash(value.as_ptr());
    }

    static constexpr auto is_trivial() -> bool {
        return false;
    }

    static constexpr auto equals(Box<T> const& a, Box<T> const& b) -> bool {
        return a.as_ptr() == b.as_ptr();
    }
};

} /* namespace TC */
