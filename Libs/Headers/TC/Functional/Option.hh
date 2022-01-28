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

#include <TC/Assertion.hh>
#include <TC/Std/Move.hh>
#include <TC/Std/New.hh>

namespace TC::Functional {

template<typename T>
class Option {
public:
    /**
     * @brief Constructors
     */
    Option() = default;
    Option(T const& value);
    Option(T&& value);
    Option(Option const& rhs);
    Option(Option&& rhs) noexcept;

    ~Option();

    /**
     * @brief Returns a reference to the value from this option
     */
    T&       value();
    T const& value() const;
    T const& value_or(T const& default_value) const;

    /**
     * @brief Returns the value of this option
     */
    [[nodiscard]] T unwrap();
    [[nodiscard]] T unwrap_or(T const& default_value) const;

    /**
     * @brief Resets the content of this option
     */
    void reset();

    /**
     * @brief Returns whether the value is present
     */
    [[nodiscard]] bool is_present() const;

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
        : m_inner_option{ const_cast<T*>(&std::move(value)) } {
    }
    Option(Option const& rhs)     = default;
    Option(Option&& rhs) noexcept = default;

    ~Option() = default;

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
    [[nodiscard]] T& unwrap_or(T const& default_value) const {
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

template<typename T>
Option<T>::Option(T const& value)
    : m_is_present{ true } {
    new (&m_data_storage) T{ value };
}

template<typename T>
Option<T>::Option(T&& value)
    : m_is_present{ true } {
    new (&m_data_storage) T{ std::move(value) };
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
        new (&m_data_storage) T{ std::move(rhs.unwrap()) };
}

template<typename T>
Option<T>::~Option() {
    reset();
}

template<typename T>
T& Option<T>::value() {
    VERIFY(is_present());

    return *__builtin_launder(reinterpret_cast<T*>(&m_data_storage));
}

template<typename T>
T const& Option<T>::value() const {
    VERIFY(is_present());

    return *__builtin_launder(reinterpret_cast<T const*>(&m_data_storage));
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
    T moved_value{ std::move(value()) };
    reset();
    return moved_value;
}
template<typename T>
T Option<T>::unwrap_or(T const& default_value) const {
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

} /* namespace TC::Functional */