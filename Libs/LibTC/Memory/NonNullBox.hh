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
#include <LibTC/Memory/Tags.hh>
#include <LibTC/Functional/ErrorOr.hh>

namespace TC {
namespace Memory {

template<typename T>
class NonNullBox {
    TC_DENY_COPY(NonNullBox);

public:
    /**
     * @brief Error safe constructors
     */
    template<typename... Args>
    static ErrorOr<NonNullBox<T>> try_construct_from_args(Args&&... args) {
        auto ptr = new (nothrow) T{ forward<Args>(args)... };
        if ( ptr != nullptr )
            return NonNullBox<T>{ Adopt, *ptr };
        else
            return Error{ ENOMEM };
    }

    /**
     * @brief Constructors
     */
    NonNullBox() = delete;
    NonNullBox(AdoptTag, T& ref)
        : m_boxed_object_ptr{ &ref } {
    }
    NonNullBox(AdoptTag, T const& ref)
        : m_boxed_object_ptr{ const_cast<T*>(&ref) } {
    }
    template<typename... Args>
    NonNullBox(FromArgsTag, Args&&... args)
        : m_boxed_object_ptr{ new (nothrow) T{ forward<Args>(args)... } } {
        VERIFY_NOT_NULL(m_boxed_object_ptr);
    }
    NonNullBox(NonNullBox&& rhs) noexcept
        : m_boxed_object_ptr{ exchange(rhs.m_boxed_object_ptr, nullptr) } {
    }
    ~NonNullBox() {
        delete m_boxed_object_ptr;
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
        Cxx::swap(m_boxed_object_ptr, rhs.m_boxed_object_ptr);
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
        VERIFY_NOT_NULL(m_boxed_object_ptr);
        return m_boxed_object_ptr;
    }
    [[nodiscard]] A_RETURN_NONNULL T const* as_ptr() const {
        VERIFY_NOT_NULL(m_boxed_object_ptr);
        return m_boxed_object_ptr;
    }

    [[nodiscard]] T& as_ref() {
        return *as_ptr();
    }
    [[nodiscard]] T const& as_ref() const {
        return *as_ptr();
    }

private:
    T* m_boxed_object_ptr{ nullptr };
};

} /* namespace Memory */

using Memory::NonNullBox;

} /* namespace TC */
