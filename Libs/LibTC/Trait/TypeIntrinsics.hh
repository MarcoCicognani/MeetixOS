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

#include <LibTC/Hashing.hh>
#include <LibTC/IntTypes.hh>
#include <LibTC/Trait/IsIntegral.hh>
#include <LibTC/Trait/IsPointer.hh>
#include <LibTC/Trait/IsSame.hh>

namespace TC {
namespace Trait {
namespace Details {

template<typename T>
struct TypeIntrinsics {
    static constexpr bool is_trivial() {
        return false;
    }
    static constexpr bool equals(T const& a, T const& b) {
        return a == b;
    }
};

} /* namespace Details */

template<typename T>
struct TypeIntrinsics : public Details::TypeIntrinsics<T> {
    /* Inherit implementation from details */
};

template<Integral T>
struct TypeIntrinsics<T> : public Details::TypeIntrinsics<T> {
    static constexpr usize hash(T const& value) {
        if constexpr ( sizeof(T) < 8 )
            return Hashing::u32_calculate_hash(value);
        else
            return Hashing::u64_calculate_hash(value);
    }

    static constexpr bool is_trivial() {
        return true;
    }
};

template<Pointer T>
struct TypeIntrinsics<T> : public Details::TypeIntrinsics<T> {
    static constexpr usize hash(T value) {
        return Hashing::pointer_calculate_hash(value);
    }

    static constexpr bool is_trivial() {
        return true;
    }
};

} /* namespace Trait */

using Trait::TypeIntrinsics;

} /* namespace TC */