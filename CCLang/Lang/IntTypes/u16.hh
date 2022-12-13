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

class u16 final {
public:
    using NativeInt = __UINT16_TYPE__;

public:
    static auto max() -> u16 {
        return __UINT16_MAX__;
    }
    static auto min() -> u16 {
        return 0;
    }

    static auto max(u16 const& lhs, u16 const& rhs) -> u16;
    static auto min(u16 const& lhs, u16 const& rhs) -> u16;

    static auto ceil_div(u16 const& lhs, u16 const& rhs) -> u16;

    static auto range(u16 const& begin, u16 const& end) -> Range<u16>;
    static auto range_inclusive(u16 const& begin, u16 const& last) -> RangeInclusive<u16>;

    explicit(false) u16() = default;
    explicit(false) u16(__UINT16_TYPE__ value);

    auto operator=(__UINT16_TYPE__ value) -> u16&;

    auto swap(u16& rhs) -> void;

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

    auto unwrap() const -> __UINT16_TYPE__;

    auto equal(u16 const& rhs) const -> bool;
    auto operator==(u16 const& rhs) const -> bool;

    auto not_equal(u16 const& rhs) const -> bool;
    auto operator!=(u16 const& rhs) const -> bool;

    auto greater(u16 const& rhs) const -> bool;
    auto operator>(u16 const& rhs) const -> bool;

    auto greater_equal(u16 const& rhs) const -> bool;
    auto operator>=(u16 const& rhs) const -> bool;

    auto less(u16 const& rhs) const -> bool;
    auto operator<(u16 const& rhs) const -> bool;

    auto less_equal(u16 const& rhs) const -> bool;
    auto operator<=(u16 const& rhs) const -> bool;

    auto bit_one_complement() const -> u16;
    auto operator~() const -> u16;

    auto bit_and(u16 const& rhs) const -> u16;
    auto operator&(u16 const& rhs) const -> u16;

    auto bit_and_assign(u16 const& rhs) -> void;
    auto operator&=(u16 const& rhs) -> u16&;

    auto bit_or(u16 const& rhs) const -> u16;
    auto operator|(u16 const& rhs) const -> u16;

    auto bit_or_assign(u16 const& rhs) -> void;
    auto operator|=(u16 const& rhs) -> u16&;

    auto bit_xor(u16 const& rhs) const -> u16;
    auto operator^(u16 const& rhs) const -> u16;

    auto bit_xor_assign(u16 const& rhs) -> void;
    auto operator^=(u16 const& rhs) -> u16&;

    auto try_left_shift(u16 const& rhs) const -> ErrorOr<u16>;
    auto left_shift(u16 const& rhs) const -> u16;
    auto operator<<(u16 const& rhs) const -> u16;

    auto try_left_shift_assign(u16 const& rhs) -> ErrorOr<void>;
    auto left_shift_assign(u16 const& rhs) -> void;
    auto operator<<=(u16 const& rhs) -> u16&;

    auto try_right_shift(u16 const& rhs) const -> ErrorOr<u16>;
    auto right_shift(u16 const& rhs) const -> u16;
    auto operator>>(u16 const& rhs) const -> u16;

    auto try_right_shift_assign(u16 const& rhs) -> ErrorOr<void>;
    auto right_shift_assign(u16 const& rhs) -> void;
    auto operator>>=(u16 const& rhs) -> u16&;

    auto operator++() -> u16&;
    auto operator++(int) -> u16;

    auto operator--() -> u16&;
    auto operator--(int) -> u16;

    auto try_add(u16 const& rhs) const -> ErrorOr<u16>;
    auto add(u16 const& rhs) const -> u16;
    auto operator+(u16 const& rhs) const -> u16;

    auto try_add_assign(u16 const& rhs) -> ErrorOr<void>;
    auto add_assign(u16 const& rhs) -> void;
    auto operator+=(u16 const& rhs) -> u16&;

    auto try_sub(u16 const& rhs) const -> ErrorOr<u16>;
    auto sub(u16 const& rhs) const -> u16;
    auto operator-(u16 const& rhs) const -> u16;

    auto try_sub_assign(u16 const& rhs) -> ErrorOr<void>;
    auto sub_assign(u16 const& rhs) -> void;
    auto operator-=(u16 const& rhs) -> u16&;

    auto try_mul(u16 const& rhs) const -> ErrorOr<u16>;
    auto mul(u16 const& rhs) const -> u16;
    auto operator*(u16 const& rhs) const -> u16;

    auto try_mul_assign(u16 const& rhs) -> ErrorOr<void>;
    auto mul_assign(u16 const& rhs) -> void;
    auto operator*=(u16 const& rhs) -> u16&;

    auto try_div(u16 const& rhs) const -> ErrorOr<u16>;
    auto div(u16 const& rhs) const -> u16;
    auto operator/(u16 const& rhs) const -> u16;

    auto try_div_assign(u16 const& rhs) -> ErrorOr<void>;
    auto div_assign(u16 const& rhs) -> void;
    auto operator/=(u16 const& rhs) -> u16&;

    auto operator%(u16 const& rhs) const -> u16;
    auto operator%=(u16 const& rhs) -> u16&;

    auto atomic_load(MemOrder mem_order = MemOrder::Total) volatile -> u16;
    auto atomic_store(u16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_add(u16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;
    auto atomic_sub(u16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_fetch_add(u16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u16;
    auto atomic_fetch_sub(u16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u16;

    auto atomic_add_fetch(u16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u16;
    auto atomic_sub_fetch(u16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u16;

    auto hash_code() const -> usize;

private:
    __UINT16_TYPE__ m_value = 0;
};
static_assert(sizeof(u16) == sizeof(__UINT16_TYPE__));

auto operator<<(__UINT16_TYPE__ lhs, u16 rhs) -> u16;
auto operator>>(__UINT16_TYPE__ lhs, u16 rhs) -> u16;
template<typename T>
auto operator+(T* ptr, u16 offset) -> T* {
    return ptr + offset.unwrap();
}
template<typename T>
auto operator-(T* ptr, u16 offset) -> T* {
    return ptr - offset.unwrap();
}

namespace Cxx {

auto swap(u16& lhs, u16& rhs) -> void;

} /* namespace Cxx */
