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

#include <TC/Cxx.hh>
#include <TC/Functional/Option.hh>

namespace TC {
namespace Functional {

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
     * @brief Swaps this result with another
     */
    void swap(Result& rhs) noexcept;

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
     * @brief Swaps this result with another
     */
    void swap(Result& rhs) noexcept;

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
     * @brief Swaps this result with another
     */
    void swap(Result& rhs) noexcept;

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

} /* namespace Functional */

using Functional::Option;

} /* namespace TC */

#include <../LibTC/Functional/Result.hhi>