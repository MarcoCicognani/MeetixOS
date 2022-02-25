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
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Functional {

template<typename T>
class Option {
public:
    /**
     * @brief Constructors
     */
    Option() = default;
    Option(T const& value)
        : m_is_present{ true } {
        new (&m_data_storage) T{ value };
    }
    Option(T&& value)
        : m_is_present{ true } {
        new (&m_data_storage) T{ move(value) };
    }
    Option(Option const& rhs)
        : m_is_present{ rhs.m_is_present } {
        if ( m_is_present )
            new (&m_data_storage) T{ rhs.value() };
    }
    Option(Option&& rhs) noexcept
        : m_is_present{ rhs.m_is_present } {
        if ( m_is_present )
            new (&m_data_storage) T{ move(rhs.unwrap()) };
    }
    ~Option() {
        reset();
    }

    Option& operator=(T const& value) {
        Option option{ value };
        swap(option);
        return *this;
    }
    Option& operator=(T&& value) {
        Option option{ move(value) };
        swap(option);
        return *this;
    }
    Option& operator=(nullptr_t) {
        reset();
        return *this;
    }
    Option& operator=(Option const& rhs) {
        if ( this == &rhs )
            return *this;

        Option option{ rhs };
        swap(option);
        return *this;
    }
    Option& operator=(Option&& rhs) noexcept {
        Option option{ move(rhs) };
        swap(option);
        return *this;
    }

    /**
     * @brief Swaps this Option with another
     */
    void swap(Option& rhs) noexcept {
        Cxx::swap(m_is_present, rhs.m_is_present);
        Cxx::swap(storage_as_ref(), rhs.storage_as_ref());
    }

    /**
     * @brief Returns a reference to the value from this option
     */
    T& value() {
        VERIFY(is_present());
        return storage_as_ref();
    }
    T const& value() const {
        VERIFY(is_present());
        return storage_as_ref();
    }
    T const& value_or(T const& default_value) const {
        if ( is_present() )
            return value();
        else
            return default_value;
    }

    /**
     * @brief Returns the value of this option
     */
    [[nodiscard]] T unwrap() {
        T moved_value{ move(value()) };
        reset();
        return moved_value;
    }
    [[nodiscard]] T unwrap_or(T const& default_value) {
        if ( is_present() )
            return unwrap();
        else
            return default_value;
    }

    /**
     * @brief Resets the content of this option
     */
    void reset() {
        if ( is_present() ) {
            value().~T();
            m_is_present = false;
        }
    }

    /**
     * @brief Returns whether the value is present
     */
    [[nodiscard]] bool is_present() const {
        return m_is_present;
    }

private:
    T& storage_as_ref() {
        return *__builtin_launder(reinterpret_cast<T*>(m_data_storage));
    }
    T const& storage_as_ref() const {
        return *__builtin_launder(reinterpret_cast<T const*>(m_data_storage));
    }

private:
    bool m_is_present{ false };
    alignas(T) u8 m_data_storage[sizeof(T)]{ 0 }; /* byte-array to avoid <m_buckets_storage> constructor call */
};

template<typename T>
class Option<T&> {
public:
    /**
     * @brief Constructors
     */
    Option() = default;
    Option(T const& value)
        : m_inner_option{ const_cast<T*>(&value) } {
    }
    Option(T&& value)
        : m_inner_option{ const_cast<T*>(&move(value)) } {
    }
    Option(Option const& rhs)     = default;
    Option(Option&& rhs) noexcept = default;
    ~Option()                     = default;

    Option& operator=(T const& value) {
        m_inner_option = &value;
        return *this;
    }
    Option& operator=(T&& value) {
        m_inner_option = &move(value);
        return *this;
    }
    Option& operator=(nullptr_t) {
        m_inner_option = nullptr;
        return *this;
    }
    Option& operator=(Option const& rhs) {
        m_inner_option = rhs.m_inner_option;
        return *this;
    }
    Option& operator=(Option&& rhs) noexcept {
        m_inner_option = move(rhs.m_inner_option);
        return *this;
    }

    /**
     * @brief Swaps this Option with another
     */
    void swap(Option& rhs) noexcept {
        m_inner_option.swap(rhs.m_inner_option);
    }

    /**
     * @brief Returns a reference to the value from this option
     */
    T& value() {
        return *m_inner_option.value();
    }
    T const& value() const {
        return *m_inner_option.value();
    }
    T const& value_or(T const& default_value) const {
        return *m_inner_option.value_or(default_value);
    }

    /**
     * @brief Returns the value of this option
     */
    [[nodiscard]] T& unwrap() {
        return *m_inner_option.unwrap();
    }
    [[nodiscard]] T& unwrap_or(T const& default_value) {
        return *m_inner_option.unwrap_or(default_value);
    }

    /**
     * @brief Resets the content of this option
     */
    void reset() {
        m_inner_option.reset();
    }

    /**
     * @brief Returns whether the value is present
     */
    [[nodiscard]] bool is_present() const {
        return m_inner_option.is_present();
    }

private:
    Option<T*> m_inner_option{};
};

} /* namespace Functional */

using Functional::Option;

} /* namespace TC */
