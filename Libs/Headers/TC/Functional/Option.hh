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

#include <assert.h>
#include <TC/Std/Move.hh>
#include <TC/Std/New.hh>
#include <TC/Tags/None.hh>

namespace TC::Functional {

template<typename T>
class Option {
public:
    /**
     * @brief Constructors
     */
    explicit Option() = default;
    explicit Option(TC::Tags::None);
    explicit Option(T const& value);
    explicit Option(T&& value);
    Option(Option const& rhs);
    Option(Option&& rhs) noexcept;

    ~Option();

    /**
     * @brief Returns a reference to the value from this option
     */
    [[nodiscard]] T&       value();
    [[nodiscard]] T const& value() const;
    [[nodiscard]] T const& value_or(T const& default_value) const;

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
    union {
        T m_data_storage;
    };
};

template<typename T>
Option<T>::Option(TC::Tags::None) {
}

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
        new (&m_data_storage) T{ rhs.m_data_storage };
}

template<typename T>
Option<T>::Option(Option&& rhs) noexcept
    : m_is_present{ rhs.m_is_present } {
    if ( m_is_present ) {
        new (&m_data_storage) T{ std::move(rhs.m_data_storage) };

        rhs.reset();
    }
}

template<typename T>
Option<T>::~Option() {
    reset();
}

template<typename T>
T& Option<T>::value() {
    assert(is_present());

    return m_data_storage;
}

template<typename T>
T const& Option<T>::value() const {
    assert(is_present());

    return m_data_storage;
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
T Option<T>::unwrap_or(const T& default_value) const {
    if ( is_present() )
        return unwrap();
    else
        return default_value;
}

template<typename T>
void Option<T>::reset() {
    if ( is_present() ) {
        m_data_storage.~T();
        m_is_present = false;
    }
}

template<typename T>
bool Option<T>::is_present() const {
    return m_is_present;
}

} /* namespace TC::Functional */