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

#include <LibTC/Lang/Cxx.hh>
#include <LibTC/Lang/Must.hh>
#include <LibTC/Core/Assertions.hh>
#include <LibTC/Core/ErrorOr.hh>
#include <LibTC/Lang/DenyCopy.hh>

#define TCBoxConstructible$(ClassName) \
    template<typename T>              \
    friend class Box

template<typename T>
class Box {
    TCDenyCopy$(Box);

public:
    /**
     * @brief Non-Error safe factory functions
     */
    template<typename... TArgs>
    [[nodiscard]]
    static auto construct_from_emplace(TArgs&&... args) -> Box<T> {
        return must$(try_construct_from_emplace(Cxx::forward<TArgs>(args)...));
    }
    [[nodiscard]]
    static auto construct_from_adopt(T& unboxed_ref) -> Box<T> {
        return Box<T>{ &unboxed_ref };
    }

    /**
     * @brief Error safe Factory functions
     */
    template<typename... TArgs>
    [[nodiscard]]
    static auto try_construct_from_emplace(TArgs&&... args) -> ErrorOr<Box<T>> {
        auto unboxed_ptr = new T{ Cxx::forward<TArgs>(args)... };
        if ( unboxed_ptr != nullptr ) [[likely]]
            return Box<T>{ unboxed_ptr };
        else
            return Error::construct_from_code(ENOMEM);
    }

    /**
     * @brief Move constructor and move assignment
     */
    Box(Box<T>&& rhs)
        : m_boxed_object_ptr{ Cxx::exchange(rhs.m_boxed_object_ptr, nullptr) } {
    }
    auto operator=(Box<T>&& rhs) -> Box<T>& {
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
    void swap(Box<T>& rhs) {
        Cxx::swap(m_boxed_object_ptr, rhs.m_boxed_object_ptr);
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
        VERIFY_NOT_NULL(m_boxed_object_ptr);
        return m_boxed_object_ptr;
    }
    [[nodiscard, gnu::returns_nonnull]]
    auto as_ptr() const -> T const* {
        VERIFY_NOT_NULL(m_boxed_object_ptr);
        return m_boxed_object_ptr;
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
    auto leak_ptr() -> T* {
        auto boxed_object_ptr = m_boxed_object_ptr;
        m_boxed_object_ptr    = nullptr;
        return boxed_object_ptr;
    }
    [[nodiscard]]
    auto leak_ref() -> T& {
        VERIFY_NOT_NULL(m_boxed_object_ptr);
        return *leak_ptr();
    }

    [[nodiscard]]
    auto is_null() -> bool {
        return m_boxed_object_ptr == nullptr;
    }

private:
    explicit constexpr Box(T* unboxed_ptr)
        : m_boxed_object_ptr{ unboxed_ptr } {
    }

private:
    T* m_boxed_object_ptr{ nullptr };
};

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
