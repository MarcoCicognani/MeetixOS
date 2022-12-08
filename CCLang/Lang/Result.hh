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

#include <CCLang/Forward.hh>

#include <CCLang/Core/Assertions.hh>
#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/Meta.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/Option.hh>

template<Concrete T, Concrete E>
class [[nodiscard("You cannot ignore a possible Error variant Result<T, E>")]] Result<T, E> final {
public:
    /**
     * @brief Constructors
     */
    Result() = delete;
    explicit(false) Result(T const& value)
        : m_value_option(value) {
    }
    explicit(false) Result(T&& value)
        : m_value_option(Cxx::move(value)) {
    }
    explicit(false) Result(E const& error)
        : m_error_option(error) {
    }
    explicit(false) Result(E&& error)
        : m_error_option(Cxx::move(error)) {
    }
    explicit(false) Result(Result<T, E> const& rhs)
        : m_value_option(rhs.m_value_option)
        , m_error_option(rhs.m_error_option) {
    }
    explicit(false) Result(Result<T, E>&& rhs)
        : m_value_option(Cxx::move(rhs.m_value_option))
        , m_error_option(Cxx::move(rhs.m_error_option)) {
    }

    ~Result() = default;

    auto operator=(T const& value) -> Result<T, E>& {
        Result<T, E> result = value;
        swap(result);
        return *this;
    }
    auto operator=(T&& value) -> Result<T, E>& {
        Result<T, E> result = Cxx::move(value);
        swap(result);
        return *this;
    }
    auto operator=(E const& error) -> Result<T, E>& {
        Result<T, E> result = error;
        swap(result);
        return *this;
    }
    auto operator=(E&& error) -> Result<T, E>& {
        Result<T, E> result = Cxx::move(error);
        swap(result);
        return *this;
    }

    auto operator=(Result<T, E> const& rhs) -> Result<T, E>& {
        auto result = rhs;
        swap(result);
        return *this;
    }
    auto operator=(Result<T, E>&& rhs) -> Result<T, E>& {
        auto result = Cxx::move(rhs);
        swap(result);
        return *this;
    }

    /**
     * @brief Swaps this result with another
     */
    auto swap(Result<T, E>& rhs) {
        Cxx::swap(m_value_option, rhs.m_value_option);
        Cxx::swap(m_error_option, rhs.m_error_option);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<T, U> auto predicate) -> Result<U, E> {
        if ( is_value() ) {
            return predicate(unwrap());
        } else {
            return unwrap_error();
        }
    }

    /**
     * @brief Maps the code of this Option into another type
     */
    template<typename U>
    auto map_error(Mapper<E, U> auto predicate) -> Result<T, U> {
        if ( is_error() ) {
            return predicate(unwrap_error());
        } else {
            return unwrap();
        }
    }

    /**
     * @brief Returns whether this result contains a value or an error
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
        verify_with_msg$(is_value(), "Tried to unwrap on a `E` variant Result<T, E>");
        return m_value_option.unwrap();
    }
    [[nodiscard]]
    auto unwrap_error() -> E {
        verify_with_msg$(is_error(), "Tried to unwrap error on a `T` variant Result<T, E>");
        return m_error_option.unwrap();
    }
    [[nodiscard]]
    auto __propagate_failure() -> E {
        return unwrap_error();
    }
    [[nodiscard]]
    auto __is_bad_variant() const -> bool {
        return is_error();
    }

private:
    Option<T> m_value_option = OptionNone;
    Option<E> m_error_option = OptionNone;
};

template<Concrete E>
class [[nodiscard("You cannot ignore a possible Error variant Result<T, E>")]] Result<void, E> final {
public:
    /**
     * @brief Constructors
     */
    explicit(false) Result() = default;
    explicit(false) Result(E const& error)
        : m_error_option(error) {
    }
    explicit(false) Result(E&& error)
        : m_error_option(Cxx::move(error)) {
    }
    explicit(false) Result(Result<void, E> const& rhs)
        : m_error_option(rhs.m_error_option) {
    }
    explicit(false) Result(Result<void, E>&& rhs)
        : m_error_option(Cxx::move(rhs.m_error_option)) {
    }

    ~Result() = default;

    auto operator=(E const& error) -> Result<void, E>& {
        Result<void, E> result = error;
        swap(result);
        return *this;
    }
    auto operator=(E&& error) -> Result<void, E>& {
        Result<void, E> result = Cxx::move(error);
        swap(result);
        return *this;
    }

    auto operator=(Result<void, E> const& rhs) -> Result<void, E>& {
        auto result = rhs;
        swap(result);
        return *this;
    }
    auto operator=(Result<void, E>&& rhs) -> Result<void, E>& {
        auto result = Cxx::move(rhs);
        swap(result);
        return *this;
    }

    /**
     * @brief Swaps this result with another
     */
    auto swap(Result<void, E>& rhs) {
        Cxx::swap(m_error_option, rhs.m_error_option);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<void, U> auto predicate) -> Result<U, E> {
        if ( is_value() ) {
            return predicate();
        } else {
            return unwrap_error();
        }
    }

    /**
     * @brief Maps the code of this Option into another type
     */
    template<typename U>
    auto map_error(Mapper<E, U> auto predicate) -> Result<void, U> {
        if ( is_error() ) {
            return predicate(unwrap_error());
        } else {
            return {};
        }
    }

    /**
     * @brief Returns whether this result contains a value or an code
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
        verify_with_msg$(is_value(), "Tried to unwrap on a `E` variant Result<T, E>");
    }
    [[nodiscard]]
    auto unwrap_error() -> E {
        verify_with_msg$(is_error(), "Tried to unwrap error on a `T` variant Result<T, E>");
        return m_error_option.unwrap();
    }

    [[nodiscard]]
    auto __propagate_failure() -> E {
        return unwrap_error();
    }
    [[nodiscard]]
    auto __is_bad_variant() const -> bool {
        return is_error();
    }

private:
    Option<E> m_error_option{};
};

template<LValue T, Concrete E>
class [[nodiscard("You cannot ignore a possible Error variant Result<T, E>")]] Result<T, E> final {
public:

public:
    /**
     * @brief Constructors
     */
    Result() = delete;
    explicit(false) Result(RemoveReference<T>& value)
        : m_value_option(value) {
    }
    explicit(false) Result(E const& error)
        : m_error_option(error) {
    }
    explicit(false) Result(E&& error)
        : m_error_option(Cxx::move(error)) {
    }
    explicit(false) Result(Result<T, E> const& rhs)
        : m_value_option(rhs.m_value_option)
        , m_error_option(rhs.m_error_option) {
    }
    explicit(false) Result(Result<T, E>&& rhs)
        : m_value_option(Cxx::move(rhs.m_value_option))
        , m_error_option(Cxx::move(rhs.m_error_option)) {
    }

    ~Result() = default;

    auto operator=(RemoveReference<T>& value) -> Result<T, E>& {
        Result<T, E> result = value;
        swap(result);
        return *this;
    }
    auto operator=(E const& error) -> Result<T, E>& {
        Result<T, E> result = error;
        swap(result);
        return *this;
    }
    auto operator=(E&& error) -> Result<T, E>& {
        Result<T, E> result = Cxx::move(error);
        swap(result);
        return *this;
    }

    auto operator=(Result<T, E> const& rhs) -> Result<T, E>& {
        auto result = rhs;
        swap(result);
        return *this;
    }
    auto operator=(Result<T, E>&& rhs) -> Result<T, E>& {
        auto result = Cxx::move(rhs);
        swap(result);
        return *this;
    }

    /**
     * @brief Swaps this result with another
     */
    auto swap(Result<T, E>& rhs) {
        Cxx::swap(m_value_option, rhs.m_value_option);
        Cxx::swap(m_error_option, rhs.m_error_option);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<T, U> auto predicate) -> Result<U, E> {
        if ( is_value() ) {
            return predicate(unwrap());
        } else {
            return unwrap_error();
        }
    }

    /**
     * @brief Maps the code of this Option into another type
     */
    template<typename U>
    auto map_error(Mapper<E, U> auto predicate) -> Result<T, U> {
        if ( is_error() ) {
            return predicate(unwrap_error());
        } else {
            return unwrap();
        }
    }

    /**
     * @brief Returns whether this result contains a value or an code
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
        verify_with_msg$(is_value(), "Tried to unwrap on a `E` variant Result<T, E>");
        return m_value_option.unwrap();
    }
    [[nodiscard]]
    auto unwrap_error() -> E {
        verify_with_msg$(is_error(), "Tried to unwrap error on a `T` variant Result<T, E>");
        return m_error_option.unwrap();
    }

    [[nodiscard]]
    auto __propagate_failure() -> E {
        return unwrap_error();
    }
    [[nodiscard]]
    auto __is_bad_variant() const -> bool {
        return is_error();
    }

private:
    Option<T> m_value_option = OptionNone;
    Option<E> m_error_option = OptionNone;
};

static_assert(Tryable<Result<i32, u64>>);
static_assert(Tryable<Result<i32&, u64>>);
static_assert(Tryable<Result<void, u64>>);

namespace Cxx {

template<typename T, typename E>
auto swap(Result<T, E>& lhs, Result<T, E>& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
