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

namespace TC::Trait {

template<typename T, T v>
struct Constant {
    using ValueType = T;
    using Type      = Constant;

    static constexpr T value = v;

    constexpr explicit operator ValueType() const {
        return value;
    }

    constexpr ValueType operator()() const {
        return value;
    }
};

} /* namespace TC::Trait */