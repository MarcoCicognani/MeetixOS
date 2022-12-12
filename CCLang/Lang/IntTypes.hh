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

#include <CCLang/Lang/IntTypes/i8.hh>
#include <CCLang/Lang/IntTypes/i16.hh>
#include <CCLang/Lang/IntTypes/i32.hh>
#include <CCLang/Lang/IntTypes/i64.hh>
#include <CCLang/Lang/IntTypes/isize.hh>

#include <CCLang/Lang/IntTypes/u8.hh>
#include <CCLang/Lang/IntTypes/u16.hh>
#include <CCLang/Lang/IntTypes/u32.hh>
#include <CCLang/Lang/IntTypes/u64.hh>
#include <CCLang/Lang/IntTypes/usize.hh>

#include <CCLang/Core/Concept.hh>
#include <CCLang/Core/TypeTraits.hh>

#ifndef IN_KERNEL
using f32 = float;
using f64 = double;
using f80 = long double;
#endif

template<WrappedIntegral T>
struct TypeTraits<T> final : public Details::TypeTraits<T> {
    static auto hash(T const& value) -> usize {
        return value.hash_code();
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<NativeIntegral T>
struct TypeTraits<T> final : public Details::TypeTraits<T> {
    static auto hash(T const& value) -> usize {
        return usize(static_cast<usize::CCIntegerType>(value)).hash_code();
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};

template<Pointer T>
struct TypeTraits<T> final : public Details::TypeTraits<T> {
    static auto hash(T value) -> usize {
        return usize(static_cast<usize::CCIntegerType>(value)).hash_code();
    }

    static constexpr auto is_trivial() -> bool {
        return true;
    }
};
