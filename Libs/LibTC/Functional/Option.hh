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
#include <LibTC/BitCast.hh>
#include <LibTC/Collection/Function.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Trait/AddConstToReference.hh>
#include <LibTC/Trait/IsLValue.hh>
#include <LibTC/Trait/IsRValue.hh>
#include <LibTC/Trait/RemoveReference.hh>
#include <LibTC/Trait/Tryable.hh>

namespace TC {
namespace Functional {

template<typename>
class Option;

template<typename T>
    requires(!IsLValue<T> && !IsRValue<T>) /* Only instance objects are supported, for references there is the following specialization */
class Option<T> {
public:
    /**
     * @brief Constructors
     */
    constexpr Option() = default;
    constexpr Option(T const& value)
        : m_is_present{ true } {
        new (&m_data_storage) T{ value };
    }
    constexpr Option(T&& value)
        : m_is_present{ true } {
        new (&m_data_storage) T{ move(value) };
    }
    constexpr Option(Option const& rhs)
        : m_is_present{ rhs.m_is_present } {
        if ( m_is_present )
            new (&m_data_storage) T{ rhs.value() };
    }
    constexpr Option(Option&& rhs) noexcept
        : m_is_present{ rhs.m_is_present } {
        if ( m_is_present )
            new (&m_data_storage) T{ move(rhs.unwrap()) };
    }
    ~Option() {
        reset();
    }

    constexpr Option& operator=(T const& value) {
        Option option{ value };
        swap(option);
        return *this;
    }
    constexpr Option& operator=(T&& value) {
        Option option{ move(value) };
        swap(option);
        return *this;
    }
    constexpr Option& operator=(nullptr_t) {
        reset();
        return *this;
    }
    constexpr Option& operator=(Option const& rhs) {
        if ( this == &rhs )
            return *this;

        Option option{ rhs };
        swap(option);
        return *this;
    }
    constexpr Option& operator=(Option&& rhs) noexcept {
        Option option{ move(rhs) };
        swap(option);
        return *this;
    }

    /**
     * @brief Swaps this Option with another
     */
    constexpr void swap(Option& rhs) noexcept {
        Cxx::swap(m_is_present, rhs.m_is_present);
        Cxx::swap(storage_as_ref(), rhs.storage_as_ref());
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    Option<U> map(auto predicate) {
        if ( is_present() )
            return predicate(value());
        else
            return {};
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

    T& value_or(T& default_value) {
        if ( is_present() )
            return value();
        else
            return default_value;
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

    /**
     * @brief Tryable support
     */
    [[nodiscard]] T unwrap() {
        T to_return{ move(value()) };
        reset();
        return to_return;
    }
    template<typename U = T>
    [[nodiscard]] Option<U> backward() const {
        VERIFY_FALSE(is_present());
        return {};
    }
    [[nodiscard]] bool operator!() const {
        return !m_is_present;
    }

private:
    T& storage_as_ref() {
        return *__builtin_launder(bit_cast<T*>(&m_data_storage));
    }
    T const& storage_as_ref() const {
        return *__builtin_launder(bit_cast<T const*>(&m_data_storage));
    }

private:
    bool m_is_present{ false };
    alignas(T) u8 m_data_storage[sizeof(T)]{ 0 }; /* byte-array to avoid <m_data_storage> constructor call */
};

template<LValue T>
class Option<T> {
public:
    /**
     * @brief Constructors
     */
    constexpr Option() = default;
    constexpr Option(RemoveReference<T>& value)
        : m_optional_ptr{ &value } {
    }
    constexpr Option(Option const& rhs) = default;
    constexpr Option(Option&& rhs) noexcept
        : m_optional_ptr{ exchange(rhs.m_optional_ptr, nullptr) } {
    }

    ~Option() {
        reset();
    }

    constexpr Option& operator=(RemoveReference<T>& value) {
        Option<T> option{ value };
        swap(option);
        return *this;
    }
    constexpr Option& operator=(nullptr_t) {
        reset();
        return *this;
    }
    constexpr Option& operator=(Option const& rhs) {
        Option<T> option{ rhs };
        swap(option);
        return *this;
    }
    constexpr Option& operator=(Option&& rhs) noexcept {
        Option<T> option{ move(rhs) };
        swap(option);
        return *this;
    }

    /**
     * @brief Swaps this Option with another
     */
    constexpr void swap(Option& rhs) noexcept {
        Cxx::swap(m_optional_ptr, rhs.m_optional_ptr);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    Option<U> map(auto predicate) {
        if ( is_present() )
            return predicate(value());
        else
            return {};
    }

    /**
     * @brief Returns a reference to the value from this option
     */
    T value() {
        VERIFY_NOT_NULL(m_optional_ptr);
        return *m_optional_ptr;
    }
    AddConstToReference<T> value() const {
        VERIFY_NOT_NULL(m_optional_ptr);
        return *m_optional_ptr;
    }

    T value_or(T default_value) {
        if ( is_present() )
            return value();
        else
            return default_value;
    }
    AddConstToReference<T> value_or(AddConstToReference<T> default_value) const {
        if ( is_present() )
            return value();
        else
            return default_value;
    }

    /**
     * @brief Returns the value of this option
     */
    [[nodiscard]] T unwrap_or(T default_value) {
        if ( is_present() )
            return unwrap();
        else
            return default_value;
    }

    /**
     * @brief Resets the content of this option
     */
    void reset() {
        m_optional_ptr = nullptr;
    }

    /**
     * @brief Returns whether the value is present
     */
    [[nodiscard]] bool is_present() const {
        return m_optional_ptr != nullptr;
    }

    /**
     * @brief Tryable support
     */
    [[nodiscard]] T unwrap() {
        return *exchange(m_optional_ptr, nullptr);
    }
    template<typename U = T>
    [[nodiscard]] Option<U> backward() const {
        VERIFY_FALSE(is_present());
        return {};
    }
    [[nodiscard]] bool operator!() const {
        return !is_present();
    }

private:
    RemoveReference<T>* m_optional_ptr{ nullptr };
};

} /* namespace Functional */

using Functional::Option;

} /* namespace TC */

static_assert(TC::Trait::Tryable<TC::Functional::Option<int>>);
static_assert(TC::Trait::Tryable<TC::Functional::Option<int&>>);