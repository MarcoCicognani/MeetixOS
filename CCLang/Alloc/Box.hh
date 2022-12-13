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

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/DenyCopy.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Try.hh>

template<typename T>
class Box : public DenyCopy {
public:
    /**
     * @brief Non-Error safe factory functions
     */
    template<typename... TArgs>
    static auto from_emplace(TArgs&&... args) -> Box<T> {
        return must$(try_from_emplace(Cxx::forward<TArgs>(args)...));
    }
    static auto new_from_adopt(T& unboxed_ref) -> Box<T> {
        return Box<T>(&unboxed_ref);
    }

    /**
     * @brief Error safe Factory functions
     */
    template<typename... TArgs>
    static auto try_from_emplace(TArgs&&... args) -> ErrorOr<Box<T>> {
        auto unboxed_ptr = new (nothrow) T(Cxx::forward<TArgs>(args)...);
        if ( unboxed_ptr != nullptr ) [[likely]] {
            return Box<T>(unboxed_ptr);
        } else {
            return Error::from_code(ErrorCode::NoMemory);
        }
    }
    static auto try_from_adopt(T* unboxed_ptr) -> ErrorOr<Box<T>> {
        if ( unboxed_ptr != nullptr ) [[likely]] {
            return Box<T>(unboxed_ptr);
        } else {
            return Error::from_code(ErrorCode::NullPointer);
        }
    }

    /**
     * @brief Move constructor and move assignment
     */
    Box(Box<T>&& rhs)
        : m_boxed_ptr(Cxx::exchange(rhs.m_boxed_ptr, nullptr)) {
    }
    auto operator=(Box<T>&& rhs) -> Box<T>& {
        Box<T> box = Cxx::move(rhs);
        swap(box);
        return *this;
    }

    ~Box() {
        if ( !is_null() ) [[likely]] {
            delete m_boxed_ptr;
        }
    }

    /**
     * @brief Swaps this box with another
     */
    void swap(Box<T>& rhs) {
        Cxx::swap(m_boxed_ptr, rhs.m_boxed_ptr);
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
    [[gnu::returns_nonnull]]
    auto as_ptr() -> T* {
        verify_not_null_with_msg$(m_boxed_ptr, "Box<T> - Tried to call as_ptr() from `Null` value");
        return m_boxed_ptr;
    }
    [[gnu::returns_nonnull]]
    auto as_ptr() const -> T const* {
        verify_not_null_with_msg$(m_boxed_ptr, "Box<T> - Tried to call as_ptr() from `Null` value");
        return m_boxed_ptr;
    }

    auto as_ref() -> T& {
        return *as_ptr();
    }
    auto as_ref() const -> T const& {
        return *as_ptr();
    }

    auto leak_ptr() -> T* {
        auto boxed_object_ptr = m_boxed_ptr;
        m_boxed_ptr     = nullptr;
        return boxed_object_ptr;
    }
    auto leak_ref() -> T& {
        verify_not_null_with_msg$(m_boxed_ptr, "Box<T> - Tried to leak_ref() from `Null` value");
        return *leak_ptr();
    }

    auto is_null() -> bool {
        return m_boxed_ptr == nullptr;
    }

    /**
     * @brief Hash support
     */
    auto hash_code() const {
        return usize((usize::NativeInt)m_boxed_ptr).hash_code();
    }

private:
    explicit Box(T* unboxed_ptr)
        : m_boxed_ptr(unboxed_ptr) {
    }

private:
    T* m_boxed_ptr;
};

template<typename T>
class Box<T[]> : public DenyCopy {
public:
    /**
     * @brief Non-Error safe factory functions
     */
    static auto from_len(usize len) -> Box<T[]> {
        return must$(try_from_len(len));
    }

    /**
     * @brief Error safe Factory functions
     */
    static auto try_from_len(usize len) -> ErrorOr<Box<T[]>> {
        auto unboxed_array_ptr = new (nothrow) T[len.unwrap()];
        if ( unboxed_array_ptr != nullptr ) [[likely]] {
            return Box<T[]>(unboxed_array_ptr);
        } else {
            return Error::from_code(ErrorCode::NoMemory);
        }
    }

    /**
     * @brief Move constructor and move assignment
     */
    Box(Box<T[]>&& rhs)
        : m_boxed_array_ptr(Cxx::exchange(rhs.m_boxed_array_ptr, nullptr)) {
    }
    auto operator=(Box<T[]>&& rhs) -> Box<T[]>& {
        Box<T> box = Cxx::move(rhs);
        swap(box);
        return *this;
    }

    ~Box() {
        if ( !is_null() ) [[likely]] {
            delete m_boxed_array_ptr;
        }
    }

    /**
     * @brief Swaps this box with another
     */
    void swap(Box<T[]>& rhs) {
        Cxx::swap(m_boxed_array_ptr, rhs.m_boxed_array_ptr);
    }

    /**
     * @brief Access operators
     */
    auto operator[](usize index) -> T& {
        return as_ptr()[index.unwrap()];
    }
    auto operator[](usize index) const -> T const& {
        return as_ptr()[index.unwrap()];
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

    /**
     * @brief Getters
     */
    [[gnu::returns_nonnull]]
    auto as_ptr() -> T* {
        verify_not_null_with_msg$(m_boxed_array_ptr, "Box<T[]> - Tried to call as_ptr() from `Null` value");
        return m_boxed_array_ptr;
    }
    [[gnu::returns_nonnull]]
    auto as_ptr() const -> T const* {
        verify_not_null_with_msg$(m_boxed_array_ptr, "Box<T[]> - Tried to call as_ptr() from `Null` value");
        return m_boxed_array_ptr;
    }

    auto leak_ptr() -> T* {
        auto boxed_array_ptr = m_boxed_array_ptr;
        m_boxed_array_ptr    = nullptr;
        return boxed_array_ptr;
    }

    auto is_null() -> bool {
        return m_boxed_array_ptr == nullptr;
    }

    /**
     * @brief Hash support
     */
    auto hash_code() const {
        return usize((usize::NativeInt)m_boxed_array_ptr).hash_code();
    }

private:
    explicit Box(T* unboxed_array_ptr)
        : m_boxed_array_ptr(unboxed_array_ptr) {
    }

private:
    T* m_boxed_array_ptr;
};

template<typename T>
struct TypeTraits<Box<T>> : public Details::TypeTraits<Box<T>> {
    static auto hash(Box<T> const& box) -> usize {
        return box.hash_code();
    }

    static auto equals(Box<T> const& a, Box<T> const& b) -> bool {
        return a.as_ptr() == b.as_ptr();
    }

    static constexpr auto is_trivial() -> bool {
        return false;
    }
};

namespace Cxx {

template<typename T>
auto swap(Box<T>& lhs, Box<T>& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
