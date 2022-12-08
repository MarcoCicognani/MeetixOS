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

