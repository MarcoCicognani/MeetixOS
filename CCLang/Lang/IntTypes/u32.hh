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

class u32 final {
public:
    using NativeInt = __UINT32_TYPE__;

public:
    static auto max() -> u32 {
        return __UINT32_MAX__;
    }
    static auto min() -> u32 {
        return 0;
    }

    static auto max(u32 const& lhs, u32 const& rhs) -> u32;
    static auto min(u32 const& lhs, u32 const& rhs) -> u32;

    static auto ceil_div(u32 const& lhs, u32 const& rhs) -> u32;

    static auto range(u32 const& begin, u32 const& end) -> Range<u32>;
    static auto range_inclusive(u32 const& begin, u32 const& last) -> RangeInclusive<u32>;

    explicit(false) u32() = default;
    explicit(false) u32(__UINT32_TYPE__ value);

    auto operator=(__UINT32_TYPE__ value) -> u32&;

    auto swap(u32& rhs) -> void;

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

    auto unwrap() const -> __UINT32_TYPE__;

    auto equal(u32 const& rhs) const -> bool;
    auto operator==(u32 const& rhs) const -> bool;

    auto not_equal(u32 const& rhs) const -> bool;
    auto operator!=(u32 const& rhs) const -> bool;

    auto greater(u32 const& rhs) const -> bool;
    auto operator>(u32 const& rhs) const -> bool;

    auto greater_equal(u32 const& rhs) const -> bool;
    auto operator>=(u32 const& rhs) const -> bool;

    auto less(u32 const& rhs) const -> bool;
    auto operator<(u32 const& rhs) const -> bool;

    auto less_equal(u32 const& rhs) const -> bool;
    auto operator<=(u32 const& rhs) const -> bool;

    auto bit_one_complement() const -> u32;
    auto operator~() const -> u32;

    auto bit_and(u32 const& rhs) const -> u32;
    auto operator&(u32 const& rhs) const -> u32;

    auto bit_and_assign(u32 const& rhs) -> void;
    auto operator&=(u32 const& rhs) -> u32&;

    auto bit_or(u32 const& rhs) const -> u32;
    auto operator|(u32 const& rhs) const -> u32;

    auto bit_or_assign(u32 const& rhs) -> void;
    auto operator|=(u32 const& rhs) -> u32&;

    auto bit_xor(u32 const& rhs) const -> u32;
    auto operator^(u32 const& rhs) const -> u32;

    auto bit_xor_assign(u32 const& rhs) -> void;
    auto operator^=(u32 const& rhs) -> u32&;

    auto try_left_shift(u32 const& rhs) const -> ErrorOr<u32>;
    auto left_shift(u32 const& rhs) const -> u32;
    auto operator<<(u32 const& rhs) const -> u32;

    auto try_left_shift_assign(u32 const& rhs) -> ErrorOr<void>;
    auto left_shift_assign(u32 const& rhs) -> void;
    auto operator<<=(u32 const& rhs) -> u32&;

    auto try_right_shift(u32 const& rhs) const -> ErrorOr<u32>;
    auto right_shift(u32 const& rhs) const -> u32;
    auto operator>>(u32 const& rhs) const -> u32;

    auto try_right_shift_assign(u32 const& rhs) -> ErrorOr<void>;
    auto right_shift_assign(u32 const& rhs) -> void;
    auto operator>>=(u32 const& rhs) -> u32&;

    auto operator++() -> u32&;
    auto operator++(int) -> u32;

    auto operator--() -> u32&;
    auto operator--(int) -> u32;

    auto try_add(u32 const& rhs) const -> ErrorOr<u32>;
    auto add(u32 const& rhs) const -> u32;
    auto operator+(u32 const& rhs) const -> u32;

    auto try_add_assign(u32 const& rhs) -> ErrorOr<void>;
    auto add_assign(u32 const& rhs) -> void;
    auto operator+=(u32 const& rhs) -> u32&;

    auto try_sub(u32 const& rhs) const -> ErrorOr<u32>;
    auto sub(u32 const& rhs) const -> u32;
    auto operator-(u32 const& rhs) const -> u32;

    auto try_sub_assign(u32 const& rhs) -> ErrorOr<void>;
    auto sub_assign(u32 const& rhs) -> void;
    auto operator-=(u32 const& rhs) -> u32&;

    auto try_mul(u32 const& rhs) const -> ErrorOr<u32>;
    auto mul(u32 const& rhs) const -> u32;
    auto operator*(u32 const& rhs) const -> u32;

    auto try_mul_assign(u32 const& rhs) -> ErrorOr<void>;
    auto mul_assign(u32 const& rhs) -> void;
    auto operator*=(u32 const& rhs) -> u32&;

    auto try_div(u32 const& rhs) const -> ErrorOr<u32>;
    auto div(u32 const& rhs) const -> u32;
    auto operator/(u32 const& rhs) const -> u32;

    auto try_div_assign(u32 const& rhs) -> ErrorOr<void>;
    auto div_assign(u32 const& rhs) -> void;
    auto operator/=(u32 const& rhs) -> u32&;

    auto operator%(u32 const& rhs) const -> u32;
    auto operator%=(u32 const& rhs) -> u32&;

    auto atomic_load(MemOrder mem_order = MemOrder::Total) volatile -> u32;
    auto atomic_store(u32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_add(u32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;
    auto atomic_sub(u32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_fetch_add(u32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u32;
    auto atomic_fetch_sub(u32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u32;

    auto atomic_add_fetch(u32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u32;
    auto atomic_sub_fetch(u32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u32;

    auto hash_code() const -> usize;

private:
    __UINT32_TYPE__ m_value = 0;
};
static_assert(sizeof(u32) == sizeof(__UINT32_TYPE__));

auto operator<<(__UINT32_TYPE__ lhs, u32 rhs) -> u32;
auto operator>>(__UINT32_TYPE__ lhs, u32 rhs) -> u32;
template<typename T>
auto operator+(T* ptr, u32 offset) -> T* {
    return ptr + offset.unwrap();
}
template<typename T>
auto operator-(T* ptr, u32 offset) -> T* {
    return ptr - offset.unwrap();
}

namespace Cxx {

auto swap(u32& lhs, u32& rhs) -> void;

} /* namespace Cxx */
