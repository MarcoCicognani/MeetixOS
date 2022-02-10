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
#include <TC/Cxx/Exchange.hh>
#include <TC/Cxx/Forward.hh>
#include <TC/Cxx/New.hh>
#include <TC/Cxx/Swap.hh>
#include <TC/DenyCopy.hh>
#include <TC/Functional/ErrorOr.hh>
#include <TC/Memory/NonNullRef.hh>

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
    NonNullBox(AdoptTag, T& ref);
    NonNullBox(AdoptTag, T const& ref);
    NonNullBox(NonNullBox&& rhs) noexcept;
    ~NonNullBox();

    NonNullBox& operator=(NonNullBox&& rhs) noexcept;

    /**
     * @brief Swaps this box with another
     */
    void swap(NonNullBox& rhs) noexcept;

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

template<typename T>
NonNullBox<T>::NonNullBox(AdoptTag, T& ref)
    : m_boxed_ptr{ &ref } {
}

template<typename T>
NonNullBox<T>::NonNullBox(AdoptTag, T const& ref)
    : m_boxed_ptr{ const_cast<T*>(&ref) } {
}

template<typename T>
NonNullBox<T>::NonNullBox(NonNullBox&& rhs) noexcept
    : m_boxed_ptr{ exchange(rhs.m_boxed_ptr, nullptr) } {
}

template<typename T>
NonNullBox<T>::~NonNullBox() {
    delete m_boxed_ptr;
    m_boxed_ptr = nullptr;
}

template<typename T>
NonNullBox<T>& NonNullBox<T>::operator=(NonNullBox&& rhs) noexcept {
    NonNullBox non_null_box{ move(rhs) };
    swap(non_null_box);
    return *this;
}

template<typename T>
void NonNullBox<T>::swap(NonNullBox& rhs) noexcept {
    Cxx::swap(m_boxed_ptr, rhs.m_boxed_ptr);
}

template<typename T>
T* NonNullBox<T>::operator->() {
    return as_ptr();
}

template<typename T>
T const* NonNullBox<T>::operator->() const {
    return as_ptr();
}

template<typename T>
T& NonNullBox<T>::operator*() {
    return as_ref();
}

template<typename T>
T const& NonNullBox<T>::operator*() const {
    return as_ref();
}

template<typename T>
NonNullBox<T>::operator T*() {
    return as_ptr();
}

template<typename T>
NonNullBox<T>::operator T const*() const {
    return as_ptr();
}

template<typename T>
NonNullBox<T>::operator T&() {
    return as_ref();
}

template<typename T>
NonNullBox<T>::operator T const&() const {
    return as_ref();
}

template<typename T>
T* NonNullBox<T>::as_ptr() {
    VERIFY_NOT_NULL(m_boxed_ptr);
    return m_boxed_ptr;
}

template<typename T>
T const* NonNullBox<T>::as_ptr() const {
    VERIFY_NOT_NULL(m_boxed_ptr);
    return m_boxed_ptr;
}

template<typename T>
T& NonNullBox<T>::as_ref() {
    VERIFY_NOT_NULL(m_boxed_ptr);
    return *m_boxed_ptr;
}

template<typename T>
T const& NonNullBox<T>::as_ref() const {
    VERIFY_NOT_NULL(m_boxed_ptr);
    return *m_boxed_ptr;
}

} /* namespace Memory */

using Memory::make_box;
using Memory::NonNullBox;
using Memory::try_make_box;

} /* namespace TC */