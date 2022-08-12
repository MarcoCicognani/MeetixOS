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

#include <LibTC/Forward.hh>

#include <LibTC/Concept.hh>
#include <LibTC/Hashing.hh>
#include <LibTC/IntTypes.hh>

namespace TC {
namespace Details {

template<typename T>
struct TypeTraits {
    static constexpr auto is_trivial() -> bool {
        return false;
    }
    static constexpr auto equals(T const& a, T const& b) -> bool {
        return a == b;
    }
};

} /* namespace Details */

template<typename T>
struct TypeTraits : public Details::TypeTraits<T> {
    /* Inherit implementation from details */
};

template<Integral T>
struct TypeTraits<T> : public Details::TypeTraits<T> {
    static constexpr auto hash(T const& value) -> usize {
        if constexpr ( sizeof(T) < 8 )
            return Hashing::u32_calculate_hash(value);
        else
            return Hashing::u64_calculate_hash(value);
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<Pointer T>
struct TypeTraits<T> : public Details::TypeTraits<T> {
    static constexpr auto hash(T value) -> usize {
        return Hashing::pointer_calculate_hash(value);
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

} /* namespace TC */