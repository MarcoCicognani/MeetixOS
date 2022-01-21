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

#include <TC/Hashing/Integer.hh>
#include <TC/Hashing/Pointer.hh>
#include <TC/IntTypes.hh>
#include <TC/Trait/IsIntegral.hh>
#include <TC/Trait/IsPointer.hh>
#include <TC/Trait/IsSame.hh>

namespace TC::Trait {

template<typename T>
struct TypeIntrinsics {
    static constexpr bool is_trivial() {
        return false;
    }
};

template<typename T>
    requires(IsIntegral<T>)
struct TypeIntrinsics<T> {
    static constexpr usize hash(T value) {
        return Hashing::integer_hash(value);
    }

    static constexpr bool is_trivial() {
        return true;
    }
};

template<typename T>
    requires(IsPointer<T>)
struct TypeIntrinsics<T> {
    static constexpr usize hash(T value) {
        return Hashing::pointer_hash(value);
    }

    static constexpr bool is_trivial() {
        return true;
    }
};

} /* namespace TC::Trait */