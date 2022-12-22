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

#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes.hh>

template<typename T>
class RAIIInert {
public:
    explicit(false) RAIIInert(T&& object) {
        new (m_data_storage) T(Cxx::move(object));
    }
    ~RAIIInert() = default;

    auto as_ref() -> T& {
        return *Cxx::bit_cast<T*>(&m_data_storage);
    }
    auto as_ref() const -> T const& {
        return *Cxx::bit_cast<T*>(&m_data_storage);
    }

    auto as_ptr() -> T* {
        return Cxx::bit_cast<T*>(&m_data_storage);
    }
    auto as_ptr() const -> T const* {
        return Cxx::bit_cast<T*>(&m_data_storage);
    }

    auto operator*() -> T& {
        return as_ref();
    }
    auto operator*() const -> T const& {
        return as_ref();
    }

    auto operator->() -> T* {
        return as_ptr();
    }
    auto operator->() const -> T const* {
        return as_ptr();
    }

private:
    alignas(T) u8 m_data_storage[sizeof(T)];
};