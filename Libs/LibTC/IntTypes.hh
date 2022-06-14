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

#include <Api/StdInt.h>

/*
#include <LibTC/Cxx.hh>
#include <LibTC/Functional/ErrorOr.hh>
#include <LibTC/Trait/IsIntegral.hh>

namespace TC {
namespace IntTypes {

template<Integral TNative>
class IntWrapper {
public:
    constexpr IntWrapper() = default;
    constexpr IntWrapper(TNative value)
        : m_native_value{ value } {
    }

    constexpr IntWrapper& operator=(IntWrapper const&)     = default;
    constexpr IntWrapper& operator=(IntWrapper&&) noexcept = default;
    constexpr IntWrapper& operator=(TNative const& rhs) {
        m_native_value = rhs;
        return *this;
    }

    void swap(IntWrapper& rhs) noexcept {
        Cxx::swap(m_native_value, rhs.m_native_value);
    }

    [[nodiscard]] constexpr ErrorOr<IntWrapper> operator+(IntWrapper const& rhs) const {
        TNative result_value;
        if ( __builtin_add_overflow(m_native_value, rhs.m_native_value, &result_value) )
            return EOVERFLOW;
        else
            IntWrapper{ result_value };
    }
    [[nodiscard]] constexpr ErrorOr<IntWrapper> operator+(TNative const& rhs) const {
        TNative result_value;
        if ( __builtin_add_overflow(m_native_value, rhs, &result_value) )
            return EOVERFLOW;
        else
            IntWrapper{ result_value };
    }

    [[nodiscard]] constexpr IntWrapper operator-(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator-(TNative const& rhs) const;

    [[nodiscard]] constexpr IntWrapper operator*(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator*(TNative const& rhs) const;

    [[nodiscard]] constexpr IntWrapper operator/(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator/(TNative const& rhs) const;

    [[nodiscard]] constexpr IntWrapper operator%(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator%(TNative const& rhs) const;

    [[nodiscard]] constexpr IntWrapper operator^(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator^(TNative const& rhs) const;

    [[nodiscard]] constexpr IntWrapper operator&(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator&(TNative const& rhs) const;

    [[nodiscard]] constexpr IntWrapper operator|(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator|(TNative const& rhs) const;

    [[nodiscard]] constexpr IntWrapper operator~() const;
    [[nodiscard]] constexpr IntWrapper operator!() const;

    [[nodiscard]] constexpr IntWrapper operator<<(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator<<(TNative const& rhs) const;

    [[nodiscard]] constexpr IntWrapper operator>>(IntWrapper const& rhs) const;
    [[nodiscard]] constexpr IntWrapper operator>>(TNative const& rhs) const;

    constexpr IntWrapper& operator+=(IntWrapper const& rhs);
    constexpr IntWrapper& operator+=(TNative const& rhs);

    constexpr IntWrapper& operator-=(IntWrapper const& rhs);
    constexpr IntWrapper& operator-=(TNative const& rhs);

    constexpr IntWrapper& operator*=(IntWrapper const& rhs);
    constexpr IntWrapper& operator*=(TNative const& rhs);

    constexpr IntWrapper& operator/=(IntWrapper const& rhs);
    constexpr IntWrapper& operator/=(TNative const& rhs);

    constexpr IntWrapper& operator%=(IntWrapper const& rhs);
    constexpr IntWrapper& operator%=(TNative const& rhs);

    constexpr IntWrapper& operator^=(IntWrapper const& rhs);
    constexpr IntWrapper& operator^=(TNative const& rhs);

    constexpr IntWrapper& operator&=(IntWrapper const& rhs);
    constexpr IntWrapper& operator&=(TNative const& rhs);

    constexpr IntWrapper& operator|=(IntWrapper const& rhs);
    constexpr IntWrapper& operator|=(TNative const& rhs);

    constexpr IntWrapper& operator>>=(IntWrapper const& rhs);
    constexpr IntWrapper& operator>>=(TNative const& rhs);

    constexpr IntWrapper& operator<<=(IntWrapper const& rhs);
    constexpr IntWrapper& operator<<=(TNative const& rhs);

    constexpr IntWrapper& operator++();
    constexpr IntWrapper  operator++(int);

    constexpr IntWrapper& operator--();
    constexpr IntWrapper  operator--(int);

    constexpr bool operator&&(IntWrapper const& rhs) const;
    constexpr bool operator&&(TNative const& rhs) const;

    constexpr bool operator||(IntWrapper const& rhs) const;
    constexpr bool operator||(TNative const& rhs) const;

    [[nodiscard]] bool operator<=>(IntWrapper const&) const = default;
    [[nodiscard]] bool operator<=>(TNative const& rhs) const {
        return m_native_value <=> rhs;
    }

private:
    TNative m_native_value{ 0 };
};

using U8    = IntWrapper<u8>;
using U16   = IntWrapper<u16>;
using U32   = IntWrapper<u32>;
using U64   = IntWrapper<u64>;
using USize = IntWrapper<usize>;

using I8    = IntWrapper<i8>;
using I16   = IntWrapper<i16>;
using I32   = IntWrapper<i32>;
using I64   = IntWrapper<i64>;
using ISize = IntWrapper<isize>;

}

using IntTypes::U16;
using IntTypes::U32;
using IntTypes::U64;
using IntTypes::U8;
using IntTypes::USize;

using IntTypes::I16;
using IntTypes::I32;
using IntTypes::I64;
using IntTypes::I8;
using IntTypes::ISize;

}  namespace TC */