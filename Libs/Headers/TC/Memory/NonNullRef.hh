/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <Api/Common.h>
#include <TC/Assertions.hh>
#include <TC/Cxx.hh>
#include <TC/Functional/ErrorOr.hh>

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
    NonNullRef(AdoptTag, T& ref);
    NonNullRef(AdoptTag, T const& ref);
    NonNullRef(NonNullRef const& rhs);
    NonNullRef(NonNullRef&& rhs) noexcept;
    ~NonNullRef();

    NonNullRef& operator=(T& ref);
    NonNullRef& operator=(T const& ref);
    NonNullRef& operator=(NonNullRef const& rhs);
    NonNullRef& operator=(NonNullRef&& rhs) noexcept;

    /**
     * @brief Swaps this ref with another
     */
    void swap(NonNullRef& rhs) noexcept;

    /**
     * @brief Access operators
     */
    A_RETURN_NONNULL T*       operator->();
    A_RETURN_NONNULL T const* operator->() const;

    T&       operator*();
    T const& operator*() const;

    /**
     * @brief Conversion operators
     */
    A_RETURN_NONNULL explicit operator T*();
    A_RETURN_NONNULL explicit operator T const*() const;

    explicit operator T&();
    explicit operator T const&() const;

    /**
     * @brief Getters
     */
    [[nodiscard]] A_RETURN_NONNULL T*       as_ptr();
    [[nodiscard]] A_RETURN_NONNULL T const* as_ptr() const;

    [[nodiscard]] T&       as_ref();
    [[nodiscard]] T const& as_ref() const;

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

template<typename T>
NonNullRef<T>::NonNullRef(NonNullRef::AdoptTag, T& ref)
    : m_shared_ptr{ &ref } {
}

template<typename T>
NonNullRef<T>::NonNullRef(NonNullRef::AdoptTag, T const& ref)
    : m_shared_ptr{ const_cast<T*>(&ref) } {
}

template<typename T>
NonNullRef<T>::NonNullRef(NonNullRef const& rhs)
    : m_shared_ptr{ rhs.m_shared_ptr } {
    if ( m_shared_ptr != nullptr ) [[likely]]
        m_shared_ptr->add_ref();
}

template<typename T>
NonNullRef<T>::NonNullRef(NonNullRef&& rhs) noexcept
    : m_shared_ptr{ exchange(rhs.m_shared_ptr, nullptr) } {
}

template<typename T>
NonNullRef<T>::~NonNullRef() {
    if ( m_shared_ptr != nullptr ) [[likely]] {
        m_shared_ptr->dec_ref();
        m_shared_ptr = nullptr;
    }
}

template<typename T>
NonNullRef<T>& NonNullRef<T>::operator=(T& ref) {
    NonNullRef non_null_ref{ ref };
    swap(non_null_ref);
    return *this;
}

template<typename T>
NonNullRef<T>& NonNullRef<T>::operator=(T const& ref) {
    NonNullRef non_null_ref{ ref };
    swap(non_null_ref);
    return *this;
}

template<typename T>
NonNullRef<T>& NonNullRef<T>::operator=(NonNullRef const& rhs) {
    if ( this == &rhs )
        return *this;

    NonNullRef non_null_ref{ rhs };
    swap(non_null_ref);
    return *this;
}

template<typename T>
NonNullRef<T>& NonNullRef<T>::operator=(NonNullRef&& rhs) noexcept {
    NonNullRef non_null_ref{ move(rhs) };
    swap(non_null_ref);
    return *this;
}

template<typename T>
void NonNullRef<T>::swap(NonNullRef& rhs) noexcept {
    Cxx::swap(m_shared_ptr, rhs.m_shared_ptr);
}

template<typename T>
T* NonNullRef<T>::operator->() {
    return as_ptr();
}

template<typename T>
T const* NonNullRef<T>::operator->() const {
    return as_ptr();
}

template<typename T>
T& NonNullRef<T>::operator*() {
    return as_ref();
}

template<typename T>
T const& NonNullRef<T>::operator*() const {
    return as_ref();
}

template<typename T>
NonNullRef<T>::operator T*() {
    return as_ptr();
}

template<typename T>
NonNullRef<T>::operator T const*() const {
    return as_ptr();
}

template<typename T>
NonNullRef<T>::operator T&() {
    return as_ref();
}

template<typename T>
NonNullRef<T>::operator T const&() const {
    return as_ref();
}

template<typename T>
T* NonNullRef<T>::as_ptr() {
    VERIFY_NOT_NULL(m_shared_ptr);
    return m_shared_ptr;
}

template<typename T>
T const* NonNullRef<T>::as_ptr() const {
    VERIFY_NOT_NULL(m_shared_ptr);
    return m_shared_ptr;
}

template<typename T>
T& NonNullRef<T>::as_ref() {
    VERIFY_NOT_NULL(m_shared_ptr);
    return *m_shared_ptr;
}

template<typename T>
T const& NonNullRef<T>::as_ref() const {
    VERIFY_NOT_NULL(m_shared_ptr);
    return *m_shared_ptr;
}

} /* namespace Memory */

using Memory::make_ref;
using Memory::NonNullRef;
using Memory::try_make_ref;

} /* namespace TC */