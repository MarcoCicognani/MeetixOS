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

#include <Api/StdInt.h>

namespace std {

template<typename T>
class initializer_list {
public:
    constexpr initializer_list() = default;

    [[nodiscard]] T& operator[](usize index) {
        return m_data[index];
    }
    [[nodiscard]] constexpr usize size() const {
        return m_size;
    }

    [[nodiscard]] constexpr T const* begin() const {
        return m_data;
    }
    [[nodiscard]] constexpr T const* end() const {
        return m_data + m_size;
    }

private:
    T*    m_data{ nullptr };
    usize m_size{ 0 };

    constexpr initializer_list(const T* data, usize size)
        : m_data{ data }
        , m_size{ size } {
    }
};

} /* namespace std */