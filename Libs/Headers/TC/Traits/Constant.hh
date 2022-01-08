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

namespace TC::Traits {

template<typename T, T value>
struct Constant {
    using ValueType = T;
    using Type      = Constant;

    static constexpr T m_value = value;

    constexpr explicit operator ValueType() const {
        return m_value;
    }

    constexpr ValueType operator()() const {
        return m_value;
    }
};

} /* namespace TC::Traits */