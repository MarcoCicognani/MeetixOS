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
#include <LibTC/Functional/ErrorOr.hh>

namespace TC {
namespace Memory {

template<typename T>
class NonNullRef {
public:
    enum AdoptTag { Adopt };

public:
    /**
     * @brief Constructors
     */
    NonNullRef() = delete;
    NonNullRef(AdoptTag, T& ref)
        : m_shared_ptr{ &ref } {
    }
    NonNullRef(AdoptTag, T const& ref)
        : m_shared_ptr{ const_cast<T*>(&ref) } {
    }
    NonNullRef(NonNullRef const& rhs)
        : m_shared_ptr{ rhs.m_shared_ptr } {
        if ( m_shared_ptr != nullptr ) [[likely]]
            m_shared_ptr->add_ref();
    }
    NonNullRef(NonNullRef&& rhs) noexcept
        : m_shared_ptr{ exchange(rhs.m_shared_ptr, nullptr) } {
    }
    ~NonNullRef() {
        if ( m_shared_ptr != nullptr ) [[likely]] {
            m_shared_ptr->dec_ref();
            m_shared_ptr = nullptr;
        }
    }

    NonNullRef& operator=(T& ref) {
        NonNullRef non_null_ref{ ref };
        swap(non_null_ref);
        return *this;
    }
    NonNullRef& operator=(T const& ref) {
        NonNullRef non_null_ref{ ref };
        swap(non_null_ref);
        return *this;
    }
    NonNullRef& operator=(NonNullRef const& rhs) {
        if ( this == &rhs )
            return *this;

        NonNullRef non_null_ref{ rhs };
        swap(non_null_ref);
        return *this;
    }
    NonNullRef& operator=(NonNullRef&& rhs) noexcept {
        NonNullRef non_null_ref{ move(rhs) };
        swap(non_null_ref);
        return *this;
    }

    /**
     * @brief Swaps this ref with another
     */
    void swap(NonNullRef& rhs) noexcept {
        Cxx::swap(m_shared_ptr, rhs.m_shared_ptr);
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
        VERIFY_NOT_NULL(m_shared_ptr);
        return m_shared_ptr;
    }
    [[nodiscard]] A_RETURN_NONNULL T const* as_ptr() const {
        VERIFY_NOT_NULL(m_shared_ptr);
        return m_shared_ptr;
    }

    [[nodiscard]] T& as_ref() {
        return *as_ptr();
    }
    [[nodiscard]] T const& as_ref() const {
        return *as_ptr();
    }

private:
    T* m_shared_ptr{ nullptr };
};

template<typename T, typename... Args>
inline NonNullRef<T> make_ref(Args&&... args) {
    auto ref_ptr = new (nothrow) T{ forward<Args>(args)... };
    VERIFY_NOT_NULL(ref_ptr);
    return NonNullRef<T>{ NonNullRef<T>::Adopt, *ref_ptr };
}

template<typename T, typename... Args>
inline ErrorOr<NonNullRef<T>> try_make_ref(Args&&... args) {
    auto ref_ptr = new (nothrow) T{ forward<Args>(args)... };
    if ( ref_ptr != nullptr )
        return NonNullRef<T>{ NonNullRef<T>::Adopt, *ref_ptr };
    else
        return ENOMEM;
}

} /* namespace Memory */

using Memory::make_ref;
using Memory::NonNullRef;
using Memory::try_make_ref;

} /* namespace TC */
