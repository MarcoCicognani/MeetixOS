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

#include <LibTC/Cxx.hh>
#include <LibTC/Functional/Option.hh>

namespace TC {
namespace Functional {

template<typename T, typename E>
class Result {
public:
    /**
     * @brief Constructors
     */
    Result() = delete;
    Result(T const& value)
        : m_value_option{ value } {
    }
    Result(T&& value)
        : m_value_option{ move(value) } {
    }
    Result(E const& error)
        : m_error_option{ error } {
    }
    Result(E&& error)
        : m_error_option{ move(error) } {
    }
    Result(Result const&)     = default;
    Result(Result&&) noexcept = default;
    ~Result()                 = default;

    Result& operator=(T const& value) {
        Result result{ value };
        swap(result);
        return *this;
    }
    Result& operator=(T&& value) {
        Result result{ move(value) };
        swap(result);
        return *this;
    }
    Result& operator=(E const& error) {
        Result result{ error };
        swap(result);
        return *this;
    }
    Result& operator=(E&& error) {
        Result result{ move(error) };
        swap(result);
        return *this;
    }

    Result& operator=(Result const&) = default;
    Result& operator=(Result&&) noexcept = default;

    /**
     * @brief Swaps this result with another
     */
    void swap(Result& rhs) noexcept {
        m_value_option.swap(rhs.m_value_option);
        m_error_option.swap(rhs.m_error_option);
    }

    /**
     * @brief Returns the reference to the result value
     */
    T& value() {
        return m_value_option.value();
    }
    T const& value() const {
        return m_value_option.value();
    }

    /**
     * @brief Returns the reference to the result error
     */
    E& error() {
        return m_error_option.value();
    }
    E const& error() const {
        return m_error_option.value();
    }

    /**
     * @brief Unwraps the value or the error if exists
     */
    [[nodiscard]] T unwrap_value() {
        return m_value_option.unwrap();
    }
    [[nodiscard]] E unwrap_error() {
        return m_error_option.unwrap();
    }

    /**
     * @brief Returns whether this result contains a value or an error
     */
    [[nodiscard]] bool is_value() const {
        return m_value_option.is_present();
    }
    [[nodiscard]] bool is_error() const {
        return m_error_option.is_present();
    }

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
    Result() = default;
    Result(E const& error)
        : m_error_option{ error } {
    }
    Result(E&& error)
        : m_error_option{ move(error) } {
    }
    Result(Result const&)     = default;
    Result(Result&&) noexcept = default;
    ~Result()                 = default;

    Result& operator=(E const& error) {
        Result result{ error };
        swap(result);
        return *this;
    }
    Result& operator=(E&& error) {
        Result result{ move(error) };
        swap(result);
        return *this;
    }

    Result& operator=(Result const&) = default;
    Result& operator=(Result&&) noexcept = default;

    /**
     * @brief Swaps this result with another
     */
    void swap(Result& rhs) noexcept {
        m_error_option.swap(rhs.m_error_option);
    }

    /**
     * @brief Returns the reference to the result value
     */
    void value() {
        /* Only for TRY/MUST compatibility */
    }
    void value() const {
        /* Only for TRY/MUST compatibility */
    }

    /**
     * @brief Returns the reference to the result error
     */
    E& error() {
        return m_error_option.value();
    }
    E const& error() const {
        return m_error_option.value();
    }

    /**
     * @brief Unwraps the value if exists
     */
    void unwrap_value() const {
        /* Only for TRY/MUST compatibility */
    }

    /**
     * @brief Unwraps the error if exists
     */
    [[nodiscard]] E unwrap_error() {
        return m_error_option.unwrap();
    }

    /**
     * @brief Returns whether this result contains a value or an error
     */
    [[nodiscard]] bool is_value() const {
        return !is_error();
    }
    [[nodiscard]] bool is_error() const {
        return m_error_option.is_present();
    }

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
    Result() = delete;
    Result(T const& value)
        : m_value_option{ value } {
    }
    Result(T&& value)
        : m_value_option{ move(value) } {
    }
    Result(E const& error)
        : m_error_option{ error } {
    }
    Result(E&& error)
        : m_error_option{ move(error) } {
    }
    Result(Result const&)     = default;
    Result(Result&&) noexcept = default;
    ~Result()                 = default;

    Result& operator=(T const& value) {
        Result result{ value };
        swap(result);
        return *this;
    }
    Result& operator=(T&& value) {
        Result result{ move(value) };
        swap(result);
        return *this;
    }
    Result& operator=(E const& error) {
        Result result{ error };
        swap(result);
        return *this;
    }
    Result& operator=(E&& error) {
        Result result{ move(error) };
        swap(result);
        return *this;
    }

    Result& operator=(Result const&) = default;
    Result& operator=(Result&&) noexcept = default;

    /**
     * @brief Swaps this result with another
     */
    void swap(Result& rhs) noexcept {
        m_value_option.swap(rhs.m_value_option);
        m_error_option.swap(rhs.m_error_option);
    }

    /**
     * @brief Returns the reference to the result value
     */
    T& value() {
        return m_value_option.value();
    }
    T const& value() const {
        return m_value_option.value();
    }

    /**
     * @brief Returns the reference to the result error
     */
    E& error() {
        return m_error_option.value();
    }
    E const& error() const {
        return m_error_option.value();
    }

    /**
     * @brief Unwraps the value or the error if exists
     */
    [[nodiscard]] T& unwrap_value() {
        return m_value_option.unwrap();
    }
    [[nodiscard]] E unwrap_error() {
        return m_error_option.unwrap();
    }

    /**
     * @brief Returns whether this result contains a value or an error
     */
    [[nodiscard]] bool is_value() const {
        return m_value_option.is_present();
    }
    [[nodiscard]] bool is_error() const {
        return m_error_option.is_present();
    }

private:
    Option<T&> m_value_option{};
    Option<E>  m_error_option{};
};

} /* namespace Functional */

using Functional::Result;

} /* namespace TC */
