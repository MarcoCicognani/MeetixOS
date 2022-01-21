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

#include <TC/IntTypes.hh>

namespace TC::Traits {

template<typename T>
struct Type {
    static constexpr bool is_trivial() {
        return false;
    }
};

template<>
struct Type<u8> {
    static constexpr bool is_trivial() {
        return true;
    }
};

template<>
struct Type<u16> {
    static constexpr bool is_trivial() {
        return true;
    }
};

template<>
struct Type<u32> {
    static constexpr bool is_trivial() {
        return true;
    }
};

template<>
struct Type<u64> {
    static constexpr bool is_trivial() {
        return true;
    }
};

template<>
struct Type<i8> {
    static constexpr bool is_trivial() {
        return true;
    }
};

template<>
struct Type<i16> {
    static constexpr bool is_trivial() {
        return true;
    }
};

template<>
struct Type<i32> {
    static constexpr bool is_trivial() {
        return true;
    }
};

template<>
struct Type<i64> {
    static constexpr bool is_trivial() {
        return true;
    }
};

template<typename T>
struct Type<T*> {
    static constexpr bool is_trivial() {
        return true;
    }
};

} /* namespace TC::Traits */