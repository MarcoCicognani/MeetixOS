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

#include <TC/Cxx/Move.hh>
#include <TC/Functional/Option.hh>

namespace TC::Functional {

template<typename T, typename E>
class Result {
public:
    /**
     * @brief Constructors
     */
    Result(T const& value);
    Result(T&& value);
    Result(E const& error);
    Result(E&& error);

    Result()                  = delete;
    Result(Result const&)     = default;
    Result(Result&&) noexcept = default;
    ~Result()                 = default;

    Result& operator=(T const& value);
    Result& operator=(T&& value);
    Result& operator=(E const& error);
    Result& operator=(E&& error);

    Result& operator=(Result const&) = default;
    Result& operator=(Result&&) noexcept = default;

    /**
     * @brief Returns the reference to the result value
     */
    T&       value();
    T const& value() const;

    /**
     * @brief Returns the reference to the result error
     */
    E&       error();
    E const& error() const;

    /**
     * @brief Unwraps the value or the error if exists
     */
    [[nodiscard]] T unwrap_value();
    [[nodiscard]] E unwrap_error();

    /**
     * @brief Returns whether this result contains a value or an error
     */
    [[nodiscard]] bool is_value() const;
    [[nodiscard]] bool is_error() const;

private:
    Option<T> m_value_option{};
    Option<E> m_error_option{};
};

template<typename E>
class Result<void, E> {
public:
    /**
     * @brief Constructors
     */
    Result(E const& error);
    Result(E&& error);

    Result()                  = default;
    Result(Result const&)     = default;
    Result(Result&&) noexcept = default;
    ~Result()                 = default;

    Result& operator=(E const& error);
    Result& operator=(E&& error);

    Result& operator=(Result const&) = default;
    Result& operator=(Result&&) noexcept = default;

    /**
     * @brief Returns the reference to the result value
     */
    void value();
    void value() const;

    /**
     * @brief Returns the reference to the result error
     */
    E&       error();
    E const& error() const;

    /**
     * @brief Unwraps the value if exists
     */
    void unwrap_value() const;

    /**
     * @brief Unwraps the error if exists
     */
    [[nodiscard]] E unwrap_error();

    /**
     * @brief Returns whether this result contains a value
     */
    [[nodiscard]] bool is_value() const;

    /**
     * @brief Returns whether this result contains an error
     */
    [[nodiscard]] bool is_error() const;

private:
    Option<E> m_error_option{};
};

template<typename T, typename E>
class Result<T&, E> {
public:

public:
    /**
     * @brief Constructors
     */
    Result(T const& value);
    Result(T&& value);
    Result(E const& error);
    Result(E&& error);

    Result()                  = delete;
    Result(Result const&)     = default;
    Result(Result&&) noexcept = default;
    ~Result()                 = default;

    Result& operator=(T const& value);
    Result& operator=(T&& value);
    Result& operator=(E const& error);
    Result& operator=(E&& error);

    Result& operator=(Result const&) = default;
    Result& operator=(Result&&) noexcept = default;

    /**
     * @brief Returns the reference to the result value
     */
    T&       value();
    T const& value() const;

    /**
     * @brief Returns the reference to the result error
     */
    E&       error();
    E const& error() const;

    /**
     * @brief Unwraps the value or the error if exists
     */
    [[nodiscard]] T& unwrap_value();
    [[nodiscard]] E  unwrap_error();

    /**
     * @brief Returns whether this result contains a value or an error
     */
    [[nodiscard]] bool is_value() const;
    [[nodiscard]] bool is_error() const;

private:
    Option<T&> m_value_option{};
    Option<E>  m_error_option{};
};

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
    m_value_option.reset();
    m_error_option.reset();

    m_value_option = value;
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(T&& value) {
    m_value_option.reset();
    m_error_option.reset();

    m_value_option = Cxx::move(value);
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(E const& error) {
    m_value_option.reset();
    m_error_option.reset();

    m_error_option = error;
    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(E&& error) {
    m_value_option.reset();
    m_error_option.reset();

    m_error_option = Cxx::move(error);
    return *this;
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
    m_error_option = error;
    return *this;
}

template<typename E>
Result<void, E>& Result<void, E>::operator=(E&& error) {
    m_error_option = Cxx::move(error);
    return *this;
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
    : m_value_option{ Cxx::move(error) } {
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(T const& value) {
    m_value_option.reset();
    m_error_option.reset();

    m_value_option = value;
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(T&& value) {
    m_value_option.reset();
    m_error_option.reset();

    m_value_option = Cxx::move(value);
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(E const& error) {
    m_value_option.reset();
    m_error_option.reset();

    m_error_option = error;
    return *this;
}

template<typename T, typename E>
Result<T&, E>& Result<T&, E>::operator=(E&& error) {
    m_value_option.reset();
    m_error_option.reset();

    m_error_option = Cxx::move(error);
    return *this;
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
