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

#include <CCLang/Lang/IntTypes.hh>

template<typename T>
class UnsafeArrayPtr {
public:
    constexpr explicit(false) UnsafeArrayPtr() = default;
    constexpr explicit(false) UnsafeArrayPtr(decltype(nullptr)) {
    }
    constexpr explicit(false) UnsafeArrayPtr(T* array_ptr)
        : m_array_ptr(array_ptr) {
    }
    constexpr UnsafeArrayPtr(UnsafeArrayPtr<T> const& rhs)
        : m_array_ptr(rhs.m_array_ptr) {
    }
    constexpr UnsafeArrayPtr(UnsafeArrayPtr<T>&& rhs)
        : m_array_ptr(Cxx::exchange(rhs.m_array_ptr, nullptr)) {
    }

    constexpr ~UnsafeArrayPtr() = default;

    auto operator=(T* array_ptr) -> UnsafeArrayPtr<T>& {
        UnsafeArrayPtr<T> unsafe_array_ptr = array_ptr;
        swap(unsafe_array_ptr);
        return *this;
    }
    auto operator=(UnsafeArrayPtr<T> const& rhs) -> UnsafeArrayPtr<T>& {
        auto unsafe_array_ptr = rhs;
        swap(unsafe_array_ptr);
        return *this;
    }
    auto operator=(UnsafeArrayPtr<T>&& rhs) -> UnsafeArrayPtr<T>& {
        auto unsafe_array_ptr = Cxx::move(rhs);
        swap(unsafe_array_ptr);
        return *this;
    }

    constexpr auto swap(UnsafeArrayPtr<T>& rhs) -> void {
        Cxx::swap(m_array_ptr, rhs.m_array_ptr);
    }

    constexpr auto unwrap() const -> T* {
        return m_array_ptr;
    }

    constexpr auto operator[](usize index) -> T& {
        return m_array_ptr[index.unwrap()];
    }
    constexpr auto operator[](usize index) const -> T const& {
        return m_array_ptr[index.unwrap()];
    }

    constexpr auto is_null() -> bool {
        return m_array_ptr == nullptr;
    }

private:
    T* m_array_ptr = nullptr;
};

template<typename T, unsigned int COUNT>
class [[gnu::packed]] UnsafeInlineArray {
public:
    constexpr explicit(false) UnsafeInlineArray() = default;

    UnsafeInlineArray(UnsafeInlineArray<T, COUNT> const& rhs) = delete;
    UnsafeInlineArray(UnsafeInlineArray<T, COUNT>&& rhs)      = delete;

    constexpr ~UnsafeInlineArray() = default;

    auto operator=(UnsafeInlineArray<T, COUNT> const& rhs) -> UnsafeArrayPtr<T>& = delete;
    auto operator=(UnsafeInlineArray<T, COUNT>&& rhs) -> UnsafeArrayPtr<T>&      = delete;

    constexpr auto swap(UnsafeInlineArray<T, COUNT>& rhs) -> void {
        for ( usize i = 0; i < COUNT; ++i )
            Cxx::swap(m_array_ptr[i.unwrap()], rhs.m_array_ptr[i.unwrap()]);
    }

    constexpr auto unwrap() const -> T const* {
        return m_array_ptr;
    }
    constexpr auto unwrap() -> T* {
        return m_array_ptr;
    }

    constexpr auto operator[](usize index) -> T& {
        return m_array_ptr[index.unwrap()];
    }
    constexpr auto operator[](usize index) const -> T const& {
        return m_array_ptr[index.unwrap()];
    }

private:
    T m_array_ptr[COUNT];
};

namespace Cxx {

template<typename T>
constexpr auto swap(UnsafeArrayPtr<T>& lhs, UnsafeArrayPtr<T>& rhs) -> void {
    lhs.swap(rhs);
}

template<typename T, unsigned int COUNT>
constexpr auto swap(UnsafeInlineArray<T, COUNT>& lhs, UnsafeInlineArray<T, COUNT>& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
