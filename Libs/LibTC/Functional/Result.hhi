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

namespace TC::Functional {

template<typename T, typename E>
Result<T, E>::Result(T const& value)
    : m_value_option{ value } {
}

template<typename T, typename E>
Result<T, E>::Result(T&& value)
    : m_value_option{ Cxx::move(value) } {
}

template<typename T, typename E>
Result<T, E>::Result(E const& error)
    : m_error_option{ error } {
}

template<typename T, typename E>
Result<T, E>::Result(E&& error)
    : m_error_option{ Cxx::move(error) } {
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(T const& value) {
    Result result{ value };
    swap(result);
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(T&& value) {
    Result result{ move(value) };
    swap(result);
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(E const& error) {
    Result result{ error };
    swap(result);
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(E&& error) {
    Result result{ move(error) };
    swap(result);
    return *this;
}

template<typename T, typename E>
void Result<T, E>::swap(Result& rhs) noexcept {
    m_value_option.swap(rhs.m_value_option);
    m_error_option.swap(rhs.m_error_option);
}

template<typename T, typename E>
T& Result<T, E>::value() {
    return m_value_option.value();
}

template<typename T, typename E>
T const& Result<T, E>::value() const {
    return m_value_option.value();
}

template<typename T, typename E>
E& Result<T, E>::error() {
    return m_error_option.value();
}

template<typename T, typename E>
E const& Result<T, E>::error() const {
    return m_error_option.value();
}

template<typename T, typename E>
T Result<T, E>::unwrap_value() {
    return m_value_option.unwrap();
}

template<typename T, typename E>
E Result<T, E>::unwrap_error() {
    return m_error_option.unwrap();
}

template<typename T, typename E>
bool Result<T, E>::is_value() const {
    return m_value_option.is_present();
}

template<typename T, typename E>
bool Result<T, E>::is_error() const {
    return m_error_option.is_present();
}

template<typename E>
Result<void, E>::Result(E const& error)
    : m_error_option{ error } {
}

template<typename E>
Result<void, E>::Result(E&& error)
    : m_error_option{ Cxx::move(error) } {
}

template<typename E>
Result<void, E>& Result<void, E>::operator=(E const& error) {
    Result result{ error };
    swap(result);
    return *this;
}

template<typename E>
Result<void, E>& Result<void, E>::operator=(E&& error) {
    Result result{ move(error) };
    swap(result);
    return *this;
}

template<typename E>
void Result<void, E>::swap(Result& rhs) noexcept {
    m_error_option.swap(rhs.m_error_option);
}

template<typename E>
void Result<void, E>::value() {
    /* Only for TRY/MUST compatibility */
}

template<typename E>
void Result<void, E>::value() const {
    /* Only for TRY/MUST compatibility */
}

template<typename E>
E& Result<void, E>::error() {
    return m_error_option.value();
}

template<typename E>
E const& Result<void, E>::error() const {
    return m_error_option.value();
}

template<typename E>
void Result<void, E>::unwrap_value() const {
    /* Only for TRY/MUST compatibility */
}

template<typename E>
E Result<void, E>::unwrap_error() {
    return m_error_option.unwrap();
}

template<typename E>
bool Result<void, E>::is_value() const {
    return !is_error();
}

template<typename E>
bool Result<void, E>::is_error() const {
    return m_error_option.is_present();
}

template<typename T, typename E>
Result<T&, E>::Result(T const& value)
    : m_value_option{ value } {
}

template<typename T, typename E>
Result<T&, E>::Result(T&& value)
    : m_value_option{ Cxx::move(value) } {
}

template<typename T, typename E>
Result<T&, E>::Result(E const& error)
    : m_error_option{ error } {
}

template<typename T, typename E>
Result<T&, E>::Result(E&& error)
    : m_error_option{ Cxx::move(error) } {
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(T const& value) {
    Result result{ value };
    swap(result);
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(T&& value) {
    Result result{ move(value) };
    swap(result);
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(E const& error) {
    Result result{ error };
    swap(result);
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(E&& error) {
    Result result{ move(error) };
    swap(result);
    return *this;
}

template<typename T, typename E>
void Result<T&, E>::swap(Result& rhs) noexcept {
    m_value_option.swap(rhs.m_value_option);
    m_error_option.swap(rhs.m_error_option);
}

template<typename T, typename E>
T& Result<T&, E>::value() {
    return m_value_option.value();
}

template<typename T, typename E>
T const& Result<T&, E>::value() const {
    return m_value_option.value();
}

template<typename T, typename E>
E& Result<T&, E>::error() {
    return m_error_option.value();
}

template<typename T, typename E>
E const& Result<T&, E>::error() const {
    return m_error_option.value();
}

template<typename T, typename E>
T& Result<T&, E>::unwrap_value() {
    return m_value_option.unwrap();
}

template<typename T, typename E>
E Result<T&, E>::unwrap_error() {
    return m_error_option.unwrap();
}

template<typename T, typename E>
bool Result<T&, E>::is_value() const {
    return m_value_option.is_present();
}

template<typename T, typename E>
bool Result<T&, E>::is_error() const {
    return m_error_option.is_present();
}

} /* namespace TC::Functional */