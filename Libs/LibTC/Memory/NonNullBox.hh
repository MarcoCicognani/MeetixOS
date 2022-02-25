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
#include <LibTC/Functional/ErrorOr.hh>

namespace TC {
namespace Memory {

template<typename T>
class NonNullBox {
    TC_DENY_COPY(NonNullBox);

public:
    enum AdoptTag { Adopt };

public:
    /**
     * @brief Constructors
     */
    NonNullBox() = delete;
    NonNullBox(AdoptTag, T& ref)
        : m_boxed_ptr{ &ref } {
    }
    NonNullBox(AdoptTag, T const& ref)
        : m_boxed_ptr{ const_cast<T*>(&ref) } {
    }
    NonNullBox(NonNullBox&& rhs) noexcept
        : m_boxed_ptr{ exchange(rhs.m_boxed_ptr, nullptr) } {
    }
    ~NonNullBox() {
        delete m_boxed_ptr;
        m_boxed_ptr = nullptr;
    }

    NonNullBox& operator=(NonNullBox&& rhs) noexcept {
        NonNullBox non_null_box{ move(rhs) };
        swap(non_null_box);
        return *this;
    }

    /**
     * @brief Swaps this box with another
     */
    void swap(NonNullBox& rhs) noexcept {
        Cxx::swap(m_boxed_ptr, rhs.m_boxed_ptr);
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
        VERIFY_NOT_NULL(m_boxed_ptr);
        return m_boxed_ptr;
    }
    [[nodiscard]] A_RETURN_NONNULL T const* as_ptr() const {
        VERIFY_NOT_NULL(m_boxed_ptr);
        return m_boxed_ptr;
    }

    [[nodiscard]] T& as_ref() {
        return *as_ptr();
    }
    [[nodiscard]] T const& as_ref() const {
        return *as_ptr();
    }

private:
    T* m_boxed_ptr{ nullptr };
};

template<typename T, typename... Args>
inline NonNullBox<T> make_box(Args&&... args) {
    auto ptr = new (nothrow) T{ forward<Args>(args)... };
    VERIFY_NOT_NULL(ptr);
    return NonNullBox<T>{ NonNullBox<T>::Adopt, *ptr };
}

template<typename T, typename... Args>
inline Functional::ErrorOr<NonNullBox<T>> try_make_box(Args&&... args) {
    auto ptr = new (nothrow) T{ forward<Args>(args)... };
    if ( ptr != nullptr )
        return NonNullBox<T>{ NonNullBox<T>::Adopt, *ptr };
    else
        return ENOMEM;
}

} /* namespace Memory */

using Memory::make_box;
using Memory::NonNullBox;
using Memory::try_make_box;

} /* namespace TC */
