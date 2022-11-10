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

#include <LibLS/Forward.hh>

#include <LibLS/Core/Concept.hh>
#include <LibLS/Core/Hashing.hh>
#include <LibLS/Lang/IntTypes.hh>

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
struct TypeTraits final : public Details::TypeTraits<T> {
    /* Inherit implementation from details */
};

template<Integral T>
struct TypeTraits<T> final : public Details::TypeTraits<T> {
    static constexpr auto hash(T const& value) -> usize {
        if constexpr ( sizeof(T) < 8 )
            return Hashing::hash_u32(value);
        else
            return Hashing::hash_u64(value);
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<Pointer T>
struct TypeTraits<T> final : public Details::TypeTraits<T> {
    static constexpr auto hash(T value) -> usize {
        return Hashing::hash_ptr(value);
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};
