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

namespace TC::Functional {

template<typename T>
Option<T>::Option(T const& value)
    : m_is_present{ true } {
    new (&m_data_storage) T{ value };
}

template<typename T>
Option<T>::Option(T&& value)
    : m_is_present{ true } {
    new (&m_data_storage) T{ move(value) };
}

template<typename T>
Option<T>::Option(Option const& rhs)
    : m_is_present{ rhs.m_is_present } {
    if ( m_is_present )
        new (&m_data_storage) T{ rhs.value() };
}

template<typename T>
Option<T>::Option(Option&& rhs) noexcept
    : m_is_present{ rhs.m_is_present } {
    if ( m_is_present )
        new (&m_data_storage) T{ move(rhs.unwrap()) };
}

template<typename T>
Option<T>::~Option() {
    reset();
}

template<typename T>
Option<T>& Option<T>::operator=(T const& value) {
    Option option{ value };
    swap(option);
    return *this;
}

template<typename T>
Option<T>& Option<T>::operator=(T&& value) {
    Option option{ move(value) };
    swap(option);
    return *this;
}

template<typename T>
Option<T>& Option<T>::operator=(Cxx::nullptr_t) {
    reset();
    return *this;
}

template<typename T>
Option<T>& Option<T>::operator=(Option const& rhs) {
    if ( this == &rhs )
        return *this;

    Option option{ rhs };
    swap(option);
    return *this;
}

template<typename T>
Option<T>& Option<T>::operator=(Option&& rhs) noexcept {
    Option option{ move(rhs) };
    swap(option);
    return *this;
}

template<typename T>
void Option<T>::swap(Option& rhs) noexcept {
    Cxx::swap(m_is_present, rhs.m_is_present);
    Cxx::swap(storage_as_ref(), rhs.storage_as_ref());
}

template<typename T>
T& Option<T>::value() {
    VERIFY(is_present());
    return storage_as_ref();
}

template<typename T>
T const& Option<T>::value() const {
    VERIFY(is_present());
    return storage_as_ref();
}

template<typename T>
T const& Option<T>::value_or(T const& default_value) const {
    if ( is_present() )
        return value();
    else
        return default_value;
}

template<typename T>
T Option<T>::unwrap() {
    T moved_value{ move(value()) };
    reset();
    return moved_value;
}
template<typename T>
T Option<T>::unwrap_or(T const& default_value) {
    if ( is_present() )
        return unwrap();
    else
        return default_value;
}

template<typename T>
void Option<T>::reset() {
    if ( is_present() ) {
        value().~T();
        m_is_present = false;
    }
}

template<typename T>
bool Option<T>::is_present() const {
    return m_is_present;
}

template<typename T>
T& Option<T>::storage_as_ref() {
    return *__builtin_launder(reinterpret_cast<T*>(m_data_storage));
}

template<typename T>
T const& Option<T>::storage_as_ref() const {
    return *__builtin_launder(reinterpret_cast<T const*>(m_data_storage));
}

template<typename T>
Option<T&>::Option(T const& value)
    : m_inner_option{ const_cast<T*>(&value) } {
}

template<typename T>
Option<T&>::Option(T&& value)
    : m_inner_option{ const_cast<T*>(&move(value)) } {
}

template<typename T>
Option<T&>& Option<T&>::operator=(T const& value) {
    m_inner_option = &value;
    return *this;
}

template<typename T>
Option<T&>& Option<T&>::operator=(T&& value) {
    m_inner_option = &move(value);
    return *this;
}

template<typename T>
Option<T&>& Option<T&>::operator=(Cxx::nullptr_t) {
    m_inner_option = nullptr;
    return *this;
}

template<typename T>
Option<T&>& Option<T&>::operator=(Option const& rhs) {
    m_inner_option = rhs.m_inner_option;
    return *this;
}

template<typename T>
Option<T&>& Option<T&>::operator=(Option&& rhs) noexcept {
    m_inner_option = move(rhs.m_inner_option);
    return *this;
}

template<typename T>
void Option<T&>::swap(Option& rhs) noexcept {
    m_inner_option.swap(rhs.m_inner_option);
}

template<typename T>
T& Option<T&>::value() {
    return *m_inner_option.value();
}

template<typename T>
T const& Option<T&>::value() const {
    return *m_inner_option.value();
}

template<typename T>
T const& Option<T&>::value_or(T const& default_value) const {
    return *m_inner_option.value_or(default_value);
}

template<typename T>
T& Option<T&>::unwrap() {
    return *m_inner_option.unwrap();
}

template<typename T>
T& Option<T&>::unwrap_or(T const& default_value) {
    return *m_inner_option.unwrap_or(default_value);
}

template<typename T>
void Option<T&>::reset() {
    m_inner_option.reset();
}

template<typename T>
bool Option<T&>::is_present() const {
    return m_inner_option.is_present();
}

} /* namespace TC::Functional */