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

#include <LibTC/Assertions.hh>
#include <LibTC/Cxx.hh>

namespace TC::Memory {

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

template<typename T, typename... Args>
NonNullBox<T> make_box(Args&&... args) {
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

} /* namespace TC::Memory */