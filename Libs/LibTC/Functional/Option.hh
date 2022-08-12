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
#include <LibTC/Concept.hh>
#include <LibTC/Cxx.hh>
#include <LibTC/Forward.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Meta.hh>

namespace TC {
namespace Functional {

enum class OptionNoneTag {
    None
};

template<typename>
class Option;

template<Concrete T>
class Option<T> {
public:
    /**
     * @brief Constructors
     */
    constexpr explicit(false) Option() noexcept = default;
    constexpr explicit(false) Option(OptionNoneTag) noexcept {
    }
    constexpr explicit(false) Option(T const& value) noexcept
        : m_is_present{ true } {
        new (&m_data_storage) T{ value };
    }
    constexpr explicit(false) Option(T&& value) noexcept
        : m_is_present{ true } {
        new (&m_data_storage) T{ Cxx::move(value) };
    }
    constexpr explicit(false) Option(Option<T> const& rhs) noexcept
        : m_is_present{ rhs.m_is_present } {
        if ( m_is_present )
            new (&m_data_storage) T{ rhs.value() };
    }
    constexpr explicit(false) Option(Option<T>&& rhs) noexcept
        : m_is_present{ rhs.m_is_present } {
        if ( m_is_present )
            new (&m_data_storage) T{ Cxx::move(rhs.unwrap()) };
    }

    ~Option() {
        reset();
    }

    constexpr auto operator=(T const& value) -> Option<T>& {
        Option option{ value };
        swap(option);
        return *this;
    }
    constexpr auto operator=(T&& value) -> Option<T>& {
        Option option{ Cxx::move(value) };
        swap(option);
        return *this;
    }
    constexpr auto operator=(OptionNoneTag) -> Option<T>& {
        reset();
        return *this;
    }
    constexpr auto operator=(Option<T> const& rhs) -> Option<T>& {
        if ( this == &rhs )
            return *this;

        Option option{ rhs };
        swap(option);
        return *this;
    }
    constexpr auto operator=(Option<T>&& rhs) noexcept -> Option<T>& {
        Option option{ Cxx::move(rhs) };
        swap(option);
        return *this;
    }

    /**
     * @brief Swaps this Option with another
     */
    constexpr void swap(Option<T>& rhs) noexcept {
        Cxx::swap(m_is_present, rhs.m_is_present);
        Cxx::swap(storage_as_ref(), rhs.storage_as_ref());
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<T, U> auto predicate) -> Option<U> {
        if ( is_present() )
            return predicate(value());
        else
            return {};
    }

    /**
     * @brief Returns a reference to the value from this option
     */
    [[nodiscard]] auto value() -> T& {
        VERIFY(is_present());
        return storage_as_ref();
    }
    [[nodiscard]] auto value() const -> T const& {
        VERIFY(is_present());
        return storage_as_ref();
    }

    [[nodiscard]] auto value_or(T& default_value) -> T& {
        if ( is_present() )
            return value();
        else
            return default_value;
    }
    [[nodiscard]] auto value_or(T const& default_value) const -> T const& {
        if ( is_present() )
            return value();
        else
            return default_value;
    }

    /**
     * @brief Returns the value of this option
     */
    [[nodiscard]] auto unwrap_or(T const& default_value) -> T {
        if ( is_present() )
            return unwrap();
        else
            return default_value;
    }

    /**
     * @brief Resets the content of this option
     */
    auto reset() {
        if ( is_present() ) {
            if constexpr ( !TypeTraits<T>::is_trivial() ) {
                value().~T();
            }
            m_is_present = false;
        }
    }

    /**
     * @brief Returns whether the value is present
     */
    [[nodiscard]] auto is_present() const -> bool {
        return m_is_present;
    }

    /**
     * @brief Tryable support
     */
    [[nodiscard]] auto unwrap() -> T {
        T to_return{ Cxx::move(value()) };
        reset();
        return to_return;
    }
    [[nodiscard]] auto backward() const -> OptionNoneTag {
        VERIFY_FALSE(is_present());
        return OptionNoneTag::None;
    }
    [[nodiscard]] auto operator!() const -> bool {
        return !m_is_present;
    }

private:
    [[nodiscard]] auto storage_as_ref() -> T& {
        return *__builtin_launder(bit_cast<T*>(&m_data_storage));
    }
    [[nodiscard]] auto storage_as_ref() const -> T const& {
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
    constexpr explicit(false) Option() noexcept = default;
    constexpr explicit(false) Option(OptionNoneTag) noexcept {
    }
    constexpr explicit(false) Option(RemoveReference<T>& value) noexcept
        : m_optional_ptr{ &value } {
    }
    constexpr explicit(false) Option(Option<T> const& rhs) noexcept
        : m_optional_ptr{ rhs.m_optional_ptr } {
    }
    constexpr explicit(false) Option(Option<T>&& rhs) noexcept
        : m_optional_ptr{ Cxx::exchange(rhs.m_optional_ptr, nullptr) } {
    }

    ~Option() {
        reset();
    }

    constexpr auto operator=(RemoveReference<T>& value) -> Option<T>& {
        Option option{ value };
        swap(option);
        return *this;
    }
    constexpr auto operator=(OptionNoneTag) -> Option<T>& {
        reset();
        return *this;
    }
    constexpr auto operator=(Option const& rhs) -> Option<T>& {
        if ( m_optional_ptr == rhs.m_optional_ptr )
            return *this;

        Option option{ rhs };
        swap(option);
        return *this;
    }
    constexpr auto operator=(Option<T>&& rhs) noexcept -> Option<T>& {
        Option option{ Cxx::move(rhs) };
        swap(option);
        return *this;
    }

    /**
     * @brief Swaps this Option with another
     */
    constexpr void swap(Option<T>& rhs) noexcept {
        Cxx::swap(m_optional_ptr, rhs.m_optional_ptr);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<T, U> auto predicate) -> Option<U> {
        if ( is_present() )
            return predicate(value());
        else
            return {};
    }

    /**
     * @brief Returns a reference to the value from this option
     */
    [[nodiscard]] auto value() -> T {
        VERIFY_NOT_NULL(m_optional_ptr);
        return *m_optional_ptr;
    }
    [[nodiscard]] auto value() const -> AddConstToReference<T> {
        VERIFY_NOT_NULL(m_optional_ptr);
        return *m_optional_ptr;
    }

    [[nodiscard]] auto value_or(T default_value) -> T {
        if ( is_present() )
            return value();
        else
            return default_value;
    }
    [[nodiscard]] auto value_or(AddConstToReference<T> default_value) const -> AddConstToReference<T> {
        if ( is_present() )
            return value();
        else
            return default_value;
    }

    /**
     * @brief Returns the value of this option
     */
    [[nodiscard]] auto unwrap_or(T default_value) -> T {
        if ( is_present() )
            return unwrap();
        else
            return default_value;
    }

    /**
     * @brief Resets the content of this option
     */
    auto reset() {
        m_optional_ptr = nullptr;
    }

    /**
     * @brief Returns whether the value is present
     */
    [[nodiscard]] auto is_present() const -> bool {
        return m_optional_ptr != nullptr;
    }

    /**
     * @brief Tryable support
     */
    [[nodiscard]] auto unwrap() -> T {
        return *Cxx::exchange(m_optional_ptr, nullptr);
    }
    [[nodiscard]] auto backward() const -> OptionNoneTag {
        VERIFY_FALSE(is_present());
        return OptionNoneTag::None;
    }
    [[nodiscard]] auto operator!() const -> bool {
        return !is_present();
    }

private:
    RemoveReference<T>* m_optional_ptr{ nullptr };
};

} /* namespace Functional */

using Functional::Option;
using Functional::OptionNoneTag::None;

} /* namespace TC */

static_assert(TC::Tryable<TC::Option<int>>);
static_assert(TC::Tryable<TC::Option<int&>>);