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
    Option(T const& value);
    Option(T&& value);
    Option(Option const& rhs);
    Option(Option&& rhs) noexcept;

    ~Option();

    Option& operator=(T const& value);
    Option& operator=(T&& value);
    Option& operator=(Cxx::nullptr_t);
    Option& operator=(Option const& rhs);
    Option& operator=(Option&& rhs) noexcept;

    /**
     * @brief Swaps this Option with another
     */
    void swap(Option& rhs) noexcept;

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
    [[nodiscard]] T unwrap_or(T const& default_value);

    /**
     * @brief Resets the content of this option
     */
    void reset();

    /**
     * @brief Returns whether the value is present
     */
    [[nodiscard]] bool is_present() const;

private:
    T&       storage_as_ref();
    T const& storage_as_ref() const;

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
    Option(T const& value);
    Option(T&& value);
    Option(Option const& rhs)     = default;
    Option(Option&& rhs) noexcept = default;

    ~Option() = default;

    Option& operator=(T const& value);
    Option& operator=(T&& value);
    Option& operator=(Cxx::nullptr_t);
    Option& operator=(Option const& rhs);
    Option& operator=(Option&& rhs) noexcept;

    /**
     * @brief Swaps this Option with another
     */
    void swap(Option& rhs) noexcept;

    /**
     * @brief Returns a reference to the value from this option
     */
    T&       value();
    T const& value() const;
    T const& value_or(T const& default_value) const;

    /**
     * @brief Returns the value of this option
     */
    [[nodiscard]] T& unwrap();
    [[nodiscard]] T& unwrap_or(T const& default_value);

    /**
     * @brief Resets the content of this option
     */
    void reset();

    /**
     * @brief Returns whether the value is present
     */
    [[nodiscard]] bool is_present() const;

private:
    Option<T*> m_inner_option{};
};

} /* namespace Functional */

using Functional::Option;

} /* namespace TC */

#include <LibTC/Functional/Option.hhi>