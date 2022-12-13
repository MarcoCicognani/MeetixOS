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

#include <CCLang/Lang/MemOrder.hh>

class u8 final {
public:
    using NativeInt = __UINT8_TYPE__;

public:
    static auto max() -> u8 {
        return __UINT8_MAX__;
    }
    static auto min() -> u8 {
        return 0;
    }

    static auto max(u8 const& lhs, u8 const& rhs) -> u8;
    static auto min(u8 const& lhs, u8 const& rhs) -> u8;

    static auto ceil_div(u8 const& lhs, u8 const& rhs) -> u8;

    static auto range(u8 const& begin, u8 const& end) -> Range<u8>;
    static auto range_inclusive(u8 const& begin, u8 const& last) -> RangeInclusive<u8>;

    explicit(false) u8() = default;
    explicit(false) u8(__UINT8_TYPE__ value);

    auto operator=(__UINT8_TYPE__ value) -> u8&;

    auto swap(u8& rhs) -> void;

    auto is_signed() const -> bool;

    auto try_bit_at(usize index) const -> ErrorOr<bool>;
    auto bit_at(usize index) const -> bool;

    auto try_set_bit(usize index, bool bit_value) -> ErrorOr<void>;
    auto set_bit(usize index, bool bit_value) -> void;

    auto count_zeroes() const -> usize;
    auto count_ones() const -> usize;

    auto bit_count() const -> usize;

    template<typename TIntWrapper>
    auto as() const -> TIntWrapper {
        return TIntWrapper(static_cast<typename TIntWrapper::NativeInt>(m_value));
    }

    auto unwrap() const -> __UINT8_TYPE__;

    auto equal(u8 const& rhs) const -> bool;
    auto operator==(u8 const& rhs) const -> bool;

    auto not_equal(u8 const& rhs) const -> bool;
    auto operator!=(u8 const& rhs) const -> bool;

    auto greater(u8 const& rhs) const -> bool;
    auto operator>(u8 const& rhs) const -> bool;

    auto greater_equal(u8 const& rhs) const -> bool;
    auto operator>=(u8 const& rhs) const -> bool;

    auto less(u8 const& rhs) const -> bool;
    auto operator<(u8 const& rhs) const -> bool;

    auto less_equal(u8 const& rhs) const -> bool;
    auto operator<=(u8 const& rhs) const -> bool;

    auto bit_one_complement() const -> u8;
    auto operator~() const -> u8;

    auto bit_and(u8 const& rhs) const -> u8;
    auto operator&(u8 const& rhs) const -> u8;

    auto bit_and_assign(u8 const& rhs) -> void;
    auto operator&=(u8 const& rhs) -> u8&;

    auto bit_or(u8 const& rhs) const -> u8;
    auto operator|(u8 const& rhs) const -> u8;

    auto bit_or_assign(u8 const& rhs) -> void;
    auto operator|=(u8 const& rhs) -> u8&;

    auto bit_xor(u8 const& rhs) const -> u8;
    auto operator^(u8 const& rhs) const -> u8;

    auto bit_xor_assign(u8 const& rhs) -> void;
    auto operator^=(u8 const& rhs) -> u8&;

    auto try_left_shift(u8 const& rhs) const -> ErrorOr<u8>;
    auto left_shift(u8 const& rhs) const -> u8;
    auto operator<<(u8 const& rhs) const -> u8;

    auto try_left_shift_assign(u8 const& rhs) -> ErrorOr<void>;
    auto left_shift_assign(u8 const& rhs) -> void;
    auto operator<<=(u8 const& rhs) -> u8&;

    auto try_right_shift(u8 const& rhs) const -> ErrorOr<u8>;
    auto right_shift(u8 const& rhs) const -> u8;
    auto operator>>(u8 const& rhs) const -> u8;

    auto try_right_shift_assign(u8 const& rhs) -> ErrorOr<void>;
    auto right_shift_assign(u8 const& rhs) -> void;
    auto operator>>=(u8 const& rhs) -> u8&;

    auto operator++() -> u8&;
    auto operator++(int) -> u8;

    auto operator--() -> u8&;
    auto operator--(int) -> u8;

    auto try_add(u8 const& rhs) const -> ErrorOr<u8>;
    auto add(u8 const& rhs) const -> u8;
    auto operator+(u8 const& rhs) const -> u8;

    auto try_add_assign(u8 const& rhs) -> ErrorOr<void>;
    auto add_assign(u8 const& rhs) -> void;
    auto operator+=(u8 const& rhs) -> u8&;

    auto try_sub(u8 const& rhs) const -> ErrorOr<u8>;
    auto sub(u8 const& rhs) const -> u8;
    auto operator-(u8 const& rhs) const -> u8;

    auto try_sub_assign(u8 const& rhs) -> ErrorOr<void>;
    auto sub_assign(u8 const& rhs) -> void;
    auto operator-=(u8 const& rhs) -> u8&;

    auto try_mul(u8 const& rhs) const -> ErrorOr<u8>;
    auto mul(u8 const& rhs) const -> u8;
    auto operator*(u8 const& rhs) const -> u8;

    auto try_mul_assign(u8 const& rhs) -> ErrorOr<void>;
    auto mul_assign(u8 const& rhs) -> void;
    auto operator*=(u8 const& rhs) -> u8&;

    auto try_div(u8 const& rhs) const -> ErrorOr<u8>;
    auto div(u8 const& rhs) const -> u8;
    auto operator/(u8 const& rhs) const -> u8;

    auto try_div_assign(u8 const& rhs) -> ErrorOr<void>;
    auto div_assign(u8 const& rhs) -> void;
    auto operator/=(u8 const& rhs) -> u8&;

    auto operator%(u8 const& rhs) const -> u8;
    auto operator%=(u8 const& rhs) -> u8&;

    auto atomic_load(MemOrder mem_order = MemOrder::Total) volatile -> u8;
    auto atomic_store(u8 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_add(u8 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;
    auto atomic_sub(u8 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_fetch_add(u8 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u8;
    auto atomic_fetch_sub(u8 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u8;

    auto atomic_add_fetch(u8 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u8;
    auto atomic_sub_fetch(u8 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u8;

    auto hash_code() const -> usize;

private:
    __UINT8_TYPE__ m_value = 0;
};
static_assert(sizeof(u8) == sizeof(__UINT8_TYPE__));

auto operator<<(__UINT8_TYPE__ lhs, u8 rhs) -> u8;
auto operator>>(__UINT8_TYPE__ lhs, u8 rhs) -> u8;
template<typename T>
auto operator+(T* ptr, u8 offset) -> T* {
    return ptr + offset.unwrap();
}
template<typename T>
auto operator-(T* ptr, u8 offset) -> T* {
    return ptr - offset.unwrap();
}

namespace Cxx {

auto swap(u8& lhs, u8& rhs) -> void;

} /* namespace Cxx */
