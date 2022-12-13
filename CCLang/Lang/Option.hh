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
#include <CCLang/Lang/Function.hh>
#include <CCLang/Lang/IntTypes.hh>

enum class OptionNoneTag {
    OptionNone
};
using enum OptionNoneTag;

template<Concrete T>
class [[nodiscard("You cannot ignore a possible None Option<T>")]] Option<T> final {
public:
    /**
     * @brief Constructors
     */
    explicit(false) Option() = default;
    explicit(false) Option(OptionNoneTag) {
    }
    explicit(false) Option(T const& value)
        : m_is_present(true) {
        new (&m_data_storage) T(value);
    }
    explicit(false) Option(T&& value)
        : m_is_present(true) {
        new (&m_data_storage) T(Cxx::move(value));
    }
    explicit(false) Option(Option<T> const& rhs)
        : m_is_present(rhs.m_is_present) {
        if ( m_is_present ) {
            new (&m_data_storage) T(rhs.storage_as_ref());
        }
    }
    explicit(false) Option(Option<T>&& rhs)
        : m_is_present(rhs.m_is_present) {
        if ( m_is_present ) {
            new (&m_data_storage) T(Cxx::move(rhs.unwrap()));
        }
    }

    ~Option() {
        reset();
    }

    auto operator=(T const& value) -> Option<T>& {
        Option<T> option = value;
        swap(option);
        return *this;
    }
    auto operator=(T&& value) -> Option<T>& {
        Option<T> option = Cxx::move(value);
        swap(option);
        return *this;
    }
    auto operator=(OptionNoneTag) -> Option<T>& {
        reset();
        return *this;
    }
    auto operator=(Option<T> const& rhs) -> Option<T>& {
        Option<T> option = rhs;
        swap(option);
        return *this;
    }
    auto operator=(Option<T>&& rhs) -> Option<T>& {
        Option<T> option = Cxx::move(rhs);
        swap(option);
        return *this;
    }

    /**
     * @brief Swaps this Option with another
     */
    void swap(Option<T>& rhs) {
        Cxx::swap(m_is_present, rhs.m_is_present);
        Cxx::swap(storage_as_ref(), rhs.storage_as_ref());
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<T, U> auto predicate) -> Option<U> {
        if ( is_present() ) {
            return predicate(unwrap());
        } else {
            return OptionNone;
        }
    }

    /**
     * @brief Returns the value of this option
     */
    auto unwrap_or(T const& default_value) -> T {
        if ( is_present() ) {
            return unwrap();
        } else {
            return default_value;
        }
    }

    /**
     * @brief Resets the content of this option
     */
    auto reset() {
        if ( is_present() ) {
            if ( !TypeTraits<T>::is_trivial() ) {
                storage_as_ref().~T();
            }
            m_is_present = false;
        }
    }

    /**
     * @brief Returns whether the value is present
     */
    auto is_present() const -> bool {
        return m_is_present;
    }

    /**
     * @brief Tryable support
     */
    auto unwrap(char const* msg = "Tried to unwrap a `None` Option<T>") -> T {
        verify_false_with_msg$(is_present(), msg);

        T __value = Cxx::move(*Cxx::bit_cast<T*>(&m_data_storage));
        reset();
        return __value;
    }
    auto __propagate_failure() const -> OptionNoneTag {
        verify_false_with_msg$(is_present(), "Tried to propagate failure from a `Some` Option<T>");
        return OptionNone;
    }
    auto __is_bad_variant() const -> bool {
        return !m_is_present;
    }

private:
    auto storage_as_ref() -> T& {
        return *Cxx::bit_cast<T*>(&m_data_storage);
    }
    auto storage_as_ref() const -> T const& {
        return *Cxx::bit_cast<T const*>(&m_data_storage);
    }

private:
    bool m_is_present                       = false;
    alignas(T) u8 m_data_storage[sizeof(T)] = { 0 }; /* byte-array to avoid <m_data_storage> constructor call */
};

template<LValue T>
class [[nodiscard("You cannot ignore a possible None Option<T>")]] Option<T> final {
public:
    /**
     * @brief Constructors
     */
    explicit(false) Option() = default;
    explicit(false) Option(OptionNoneTag) {
    }
    explicit(false) Option(RemoveReference<T>& value)
        : m_optional_ptr(&value) {
    }
    explicit(false) Option(Option<T> const& rhs)
        : m_optional_ptr(rhs.m_optional_ptr) {
    }
    explicit(false) Option(Option<T>&& rhs)
        : m_optional_ptr(Cxx::exchange(rhs.m_optional_ptr, nullptr)) {
    }

    ~Option() {
        reset();
    }

    auto operator=(RemoveReference<T>& value) -> Option<T>& {
        Option<T> option = value;
        swap(option);
        return *this;
    }
    auto operator=(OptionNoneTag) -> Option<T>& {
        reset();
        return *this;
    }
    auto operator=(Option const& rhs) -> Option<T>& {
        Option<T> option = rhs;
        swap(option);
        return *this;
    }
    auto operator=(Option<T>&& rhs) -> Option<T>& {
        Option<T> option = Cxx::move(rhs);
        swap(option);
        return *this;
    }

    /**
     * @brief Swaps this Option with another
     */
    void swap(Option<T>& rhs) {
        Cxx::swap(m_optional_ptr, rhs.m_optional_ptr);
    }

    /**
     * @brief Maps the value of this Option into another type
     */
    template<typename U>
    auto map(Mapper<T, U> auto predicate) -> Option<U> {
        if ( is_present() ) {
            return predicate(unwrap());
        } else {
            return {};
        }
    }

    /**
     * @brief Returns the value of this option
     */
    auto unwrap_or(T default_value) -> T {
        if ( is_present() ) {
            return unwrap();
        } else {
            return default_value;
        }
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
    auto is_present() const -> bool {
        return m_optional_ptr != nullptr;
    }

    /**
     * @brief Tryable support
     */
    auto unwrap(char const* msg = "Tried to unwrap a `None` Option<T>") -> T {
        verify_with_msg$(is_present(), msg);
        return *Cxx::exchange(m_optional_ptr, nullptr);
    }
    auto __propagate_failure() const -> OptionNoneTag {
        verify_false_with_msg$(is_present(), "Tried to propagate failure from a `Some` Option<T>");
        return OptionNone;
    }
    auto __is_bad_variant() const -> bool {
        return !is_present();
    }

private:
    RemoveReference<T>* m_optional_ptr = nullptr;
};

static_assert(Tryable<Option<i32>>);
static_assert(Tryable<Option<i32&>>);

namespace Cxx {

template<typename T>
auto swap(Option<T>& lhs, Option<T>& rhs) -> void {
    return lhs.swap(rhs);
}

} /* namespace Cxx */
