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

#include <LibTC/Forward.hh>

#include <LibTC/Concept.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/Option.hh>
#include <LibTC/Meta.hh>

namespace TC {
namespace Functional {

template<typename, typename>
class [[nodiscard("Result<T, E> cannot be obtained and ignored")]] Result;

template<Concrete T, Concrete E>
class [[nodiscard("Result<T, E> cannot be obtained and ignored")]] Result<T, E> {
public:
    /**
     * @brief Constructors
     */
    Result() = delete;
    constexpr explicit(false) Result(T const& value)
        : m_value_option{ value } {
    }
    constexpr explicit(false) Result(T&& value)
        : m_value_option{ Cxx::move(value) } {
    }
    constexpr explicit(false) Result(E const& error)
        : m_error_option{ error } {
    }
    constexpr explicit(false) Result(E&& error)
        : m_error_option{ Cxx::move(error) } {
    }
    constexpr explicit(false) Result(Result<T, E> const& rhs)
        : m_value_option{ rhs.m_value_option }
        , m_error_option{ rhs.m_error_option } {
    }
    constexpr explicit(false) Result(Result<T, E>&& rhs)
        : m_value_option{ Cxx::move(rhs.m_value_option) }
        , m_error_option{ Cxx::move(rhs.m_error_option) } {
    }

    ~Result() = default;

    constexpr auto operator=(T const& value) -> Result<T, E>& {
        Result result{ value };
        swap(result);
        return *this;
    }
    constexpr auto operator=(T&& value) -> Result<T, E>& {
        Result result{ Cxx::move(value) };
        swap(result);
        return *this;
    }
    constexpr auto operator=(E const& error) -> Result<T, E>& {
        Result result{ error };
        swap(result);
        return *this;
    }
    constexpr auto operator=(E&& error) -> Result<T, E>& {
        Result result{ Cxx::move(error) };
        swap(result);
        return *this;
    }

    constexpr auto operator=(Result<T, E> const& rhs) -> Result<T, E>& {
        Result result{ rhs };
        swap(result);
        return *this;
    }
    constexpr auto operator=(Result<T, E>&& rhs) -> Result<T, E>& {
        Result result{ Cxx::move(rhs) };
        swap(result);
        return *this;
    }

    /**
     * @brief Swaps this result with another
     */
    constexpr auto swap(Result<T, E>& rhs) {
        m_value_option.swap(rhs.m_value_option);
        m_error_option.swap(rhs.m_error_option);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<T, U> auto predicate) -> Result<U, E> {
        if ( is_value() ) {
            auto result = predicate(value());
            return result;
        } else
            return {};
    }

    /**
     * @brief Maps the errno_code of this Option into another type
     */
    template<typename U>
    auto map_error(Mapper<E, U> auto predicate) -> Result<T, U> {
        if ( is_error() )
            return predicate(error());
        else
            return {};
    }

    /**
     * @brief Returns the reference to the result value
     */
    [[nodiscard]]
    auto value() -> T& {
        return m_value_option.value();
    }
    [[nodiscard]]
    auto value() const -> T const& {
        return m_value_option.value();
    }

    /**
     * @brief Returns the reference to the result errno_code
     */
    [[nodiscard]]
    auto error() -> E& {
        return m_error_option.value();
    }
    [[nodiscard]]
    auto error() const -> E const& {
        return m_error_option.value();
    }

    /**
     * @brief Unwraps the value or the errno_code if exists
     */
    [[nodiscard]]
    auto unwrap_value() -> T {
        return m_value_option.unwrap();
    }
    [[nodiscard]]
    auto unwrap_error() -> E {
        return m_error_option.unwrap();
    }

    /**
     * @brief Returns whether this result contains a value or an errno_code
     */
    [[nodiscard]]
    auto is_value() const -> bool {
        return m_value_option.is_present();
    }
    [[nodiscard]]
    auto is_error() const -> bool {
        return m_error_option.is_present();
    }

    /**
     * @brief Tryable support
     */
    [[nodiscard]]
    auto unwrap() -> T {
        return unwrap_value();
    }
    [[nodiscard]]
    auto backward() -> E {
        return unwrap_error();
    }
    [[nodiscard]]
    auto operator!() const -> bool {
        return is_error();
    }

private:
    Option<T> m_value_option{};
    Option<E> m_error_option{};
};

template<Concrete E>
class [[nodiscard("Result<T, E> cannot be obtained and ignored")]] Result<void, E> {
public:
    /**
     * @brief Constructors
     */
    constexpr explicit(false) Result() = default;
    constexpr explicit(false) Result(E const& error)
        : m_error_option{ error } {
    }
    constexpr explicit(false) Result(E&& error)
        : m_error_option{ Cxx::move(error) } {
    }
    constexpr explicit(false) Result(Result<void, E> const& rhs)
        : m_error_option{ rhs.m_error_option } {
    }
    constexpr explicit(false) Result(Result<void, E>&& rhs)
        : m_error_option{ Cxx::move(rhs.m_error_option) } {
    }

    ~Result() = default;

    constexpr auto operator=(E const& error) -> Result<void, E>& {
        Result result{ error };
        swap(result);
        return *this;
    }
    constexpr auto operator=(E&& error) -> Result<void, E>& {
        Result result{ Cxx::move(error) };
        swap(result);
        return *this;
    }

    constexpr auto operator=(Result<void, E> const& rhs) -> Result<void, E>& {
        Result result{ rhs };
        swap(result);
        return *this;
    }
    constexpr auto operator=(Result<void, E>&& rhs) -> Result<void, E>& {
        Result result{ Cxx::move(rhs) };
        swap(result);
        return *this;
    }

    /**
     * @brief Swaps this result with another
     */
    constexpr auto swap(Result<void, E>& rhs) {
        m_error_option.swap(rhs.m_error_option);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<void, U> auto predicate) -> Result<U, E> {
        if ( is_value() ) {
            auto result = predicate(value());
            return result;
        } else
            return {};
    }

    /**
     * @brief Maps the errno_code of this Option into another type
     */
    template<typename U>
    auto map_error(Mapper<E, U> auto predicate) -> Result<void, U> {
        if ( is_error() )
            return predicate(error());
        else
            return {};
    }

    /**
     * @brief Returns the reference to the result value
     */
    constexpr void value() {
        /* Only for TRY/MUST compatibility */
    }
    constexpr void value() const {
        /* Only for TRY/MUST compatibility */
    }

    /**
     * @brief Returns the reference to the result errno_code
     */
    [[nodiscard]]
    auto error() -> E& {
        return m_error_option.value();
    }
    [[nodiscard]]
    auto error() const -> E const& {
        return m_error_option.value();
    }

    /**
     * @brief Unwraps the value or the errno_code if exists
     */
    void unwrap_value() const {
        /* Nothing to return */
    }
    [[nodiscard]]
    auto unwrap_error() -> E {
        return m_error_option.unwrap();
    }

    /**
     * @brief Returns whether this result contains a value or an errno_code
     */
    [[nodiscard]]
    auto is_value() const -> bool {
        return !is_error();
    }
    [[nodiscard]]
    auto is_error() const -> bool {
        return m_error_option.is_present();
    }

    /**
     * @brief Tryable support
     */
    void unwrap() const {
        /* Nothing to return */
    }
    [[nodiscard]]
    auto backward() -> E {
        return unwrap_error();
    }
    [[nodiscard]]
    auto operator!() const -> bool {
        return is_error();
    }

private:
    Option<E> m_error_option{};
};

template<LValue T, Concrete E>
class [[nodiscard("Result<T, E> cannot be obtained and ignored")]] Result<T, E> {
public:

public:
    /**
     * @brief Constructors
     */
    Result() = delete;
    constexpr explicit(false) Result(RemoveReference<T>& value)
        : m_value_option{ value } {
    }
    constexpr explicit(false) Result(E const& error)
        : m_error_option{ error } {
    }
    constexpr explicit(false) Result(E&& error)
        : m_error_option{ Cxx::move(error) } {
    }
    constexpr explicit(false) Result(Result<T, E> const& rhs)
        : m_value_option{ rhs.m_value_option }
        , m_error_option{ rhs.m_error_option } {
    }
    constexpr explicit(false) Result(Result<T, E>&& rhs)
        : m_value_option{ Cxx::move(rhs.m_value_option) }
        , m_error_option{ Cxx::move(rhs.m_error_option) } {
    }

    ~Result() = default;

    constexpr auto operator=(RemoveReference<T>& value) -> Result<T, E>& {
        Result result{ value };
        swap(result);
        return *this;
    }
    constexpr auto operator=(E const& error) -> Result<T, E>& {
        Result result{ error };
        swap(result);
        return *this;
    }
    constexpr auto operator=(E&& error) -> Result<T, E>& {
        Result result{ Cxx::move(error) };
        swap(result);
        return *this;
    }

    constexpr auto operator=(Result<T, E> const& rhs) -> Result<T, E>& {
        Result result{ rhs };
        swap(result);
        return *this;
    }
    constexpr auto operator=(Result<T, E>&& rhs) -> Result<T, E>& {
        Result result{ Cxx::move(rhs) };
        swap(result);
        return *this;
    }

    /**
     * @brief Swaps this result with another
     */
    constexpr auto swap(Result<T, E>& rhs) {
        m_value_option.swap(rhs.m_value_option);
        m_error_option.swap(rhs.m_error_option);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<T, U> auto predicate) -> Result<U, E> {
        if ( is_value() ) {
            auto result = predicate(value());
            return result;
        } else
            return {};
    }

    /**
     * @brief Maps the errno_code of this Option into another type
     */
    template<typename U>
    auto map_error(Mapper<E, U> auto predicate) -> Result<T, U> {
        if ( is_error() )
            return predicate(error());
        else
            return {};
    }

    /**
     * @brief Returns the reference to the result value
     */
    [[nodiscard]]
    auto value() -> T {
        return m_value_option.value();
    }
    [[nodiscard]]
    auto value() const -> AddConstToReference<T> {
        return m_value_option.value();
    }

    /**
     * @brief Returns the reference to the result errno_code
     */
    [[nodiscard]]
    auto error() -> E& {
        return m_error_option.value();
    }
    [[nodiscard]]
    auto error() const -> E const& {
        return m_error_option.value();
    }

    /**
     * @brief Unwraps the value or the errno_code if exists
     */
    [[nodiscard]]
    auto unwrap_value() -> T {
        return m_value_option.unwrap();
    }
    [[nodiscard]]
    auto unwrap_error() -> E {
        return m_error_option.unwrap();
    }

    /**
     * @brief Returns whether this result contains a value or an errno_code
     */
    [[nodiscard]]
    auto is_value() const -> bool {
        return m_value_option.is_present();
    }
    [[nodiscard]]
    auto is_error() const -> bool {
        return m_error_option.is_present();
    }

    /**
     * @brief Tryable support
     */
    [[nodiscard]]
    auto unwrap() -> T {
        return unwrap_value();
    }
    [[nodiscard]]
    auto backward() -> E {
        return unwrap_error();
    }
    [[nodiscard]]
    auto operator!() const -> bool {
        return is_error();
    }

private:
    Option<T> m_value_option{};
    Option<E> m_error_option{};
};

} /* namespace Functional */

using Functional::Result;

} /* namespace TC */

static_assert(TC::Tryable<TC::Result<int, long>>);
static_assert(TC::Tryable<TC::Result<int&, long>>);
static_assert(TC::Tryable<TC::Result<void, long>>);