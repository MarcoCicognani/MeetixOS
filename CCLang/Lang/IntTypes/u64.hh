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

class u64 final {
public:
    using NativeInt = __UINT64_TYPE__;

public:
    static auto max() -> u64 {
        return __UINT64_MAX__;
    }
    static auto min() -> u64 {
        return 0;
    }

    static auto max(u64 const& lhs, u64 const& rhs) -> u64;
    static auto min(u64 const& lhs, u64 const& rhs) -> u64;

    static auto ceil_div(u64 const& lhs, u64 const& rhs) -> u64;

    static auto range(u64 const& begin, u64 const& end) -> Range<u64>;
    static auto range_inclusive(u64 const& begin, u64 const& last) -> RangeInclusive<u64>;

    explicit(false) u64() = default;
    explicit(false) u64(__UINT64_TYPE__ value);

    auto operator=(__UINT64_TYPE__ value) -> u64&;

    auto swap(u64& rhs) -> void;

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

    auto unwrap() const -> __UINT64_TYPE__;

    auto equal(u64 const& rhs) const -> bool;
    auto operator==(u64 const& rhs) const -> bool;

    auto not_equal(u64 const& rhs) const -> bool;
    auto operator!=(u64 const& rhs) const -> bool;

    auto greater(u64 const& rhs) const -> bool;
    auto operator>(u64 const& rhs) const -> bool;

    auto greater_equal(u64 const& rhs) const -> bool;
    auto operator>=(u64 const& rhs) const -> bool;

    auto less(u64 const& rhs) const -> bool;
    auto operator<(u64 const& rhs) const -> bool;

    auto less_equal(u64 const& rhs) const -> bool;
    auto operator<=(u64 const& rhs) const -> bool;

    auto bit_one_complement() const -> u64;
    auto operator~() const -> u64;

    auto bit_and(u64 const& rhs) const -> u64;
    auto operator&(u64 const& rhs) const -> u64;

    auto bit_and_assign(u64 const& rhs) -> void;
    auto operator&=(u64 const& rhs) -> u64&;

    auto bit_or(u64 const& rhs) const -> u64;
    auto operator|(u64 const& rhs) const -> u64;

    auto bit_or_assign(u64 const& rhs) -> void;
    auto operator|=(u64 const& rhs) -> u64&;

    auto bit_xor(u64 const& rhs) const -> u64;
    auto operator^(u64 const& rhs) const -> u64;

    auto bit_xor_assign(u64 const& rhs) -> void;
    auto operator^=(u64 const& rhs) -> u64&;

    auto try_left_shift(u64 const& rhs) const -> ErrorOr<u64>;
    auto left_shift(u64 const& rhs) const -> u64;
    auto operator<<(u64 const& rhs) const -> u64;

    auto try_left_shift_assign(u64 const& rhs) -> ErrorOr<void>;
    auto left_shift_assign(u64 const& rhs) -> void;
    auto operator<<=(u64 const& rhs) -> u64&;

    auto try_right_shift(u64 const& rhs) const -> ErrorOr<u64>;
    auto right_shift(u64 const& rhs) const -> u64;
    auto operator>>(u64 const& rhs) const -> u64;

    auto try_right_shift_assign(u64 const& rhs) -> ErrorOr<void>;
    auto right_shift_assign(u64 const& rhs) -> void;
    auto operator>>=(u64 const& rhs) -> u64&;

    auto operator++() -> u64&;
    auto operator++(int) -> u64;

    auto operator--() -> u64&;
    auto operator--(int) -> u64;

    auto try_add(u64 const& rhs) const -> ErrorOr<u64>;
    auto add(u64 const& rhs) const -> u64;
    auto operator+(u64 const& rhs) const -> u64;

    auto try_add_assign(u64 const& rhs) -> ErrorOr<void>;
    auto add_assign(u64 const& rhs) -> void;
    auto operator+=(u64 const& rhs) -> u64&;

    auto try_sub(u64 const& rhs) const -> ErrorOr<u64>;
    auto sub(u64 const& rhs) const -> u64;
    auto operator-(u64 const& rhs) const -> u64;

    auto try_sub_assign(u64 const& rhs) -> ErrorOr<void>;
    auto sub_assign(u64 const& rhs) -> void;
    auto operator-=(u64 const& rhs) -> u64&;

    auto try_mul(u64 const& rhs) const -> ErrorOr<u64>;
    auto mul(u64 const& rhs) const -> u64;
    auto operator*(u64 const& rhs) const -> u64;

    auto try_mul_assign(u64 const& rhs) -> ErrorOr<void>;
    auto mul_assign(u64 const& rhs) -> void;
    auto operator*=(u64 const& rhs) -> u64&;

    auto try_div(u64 const& rhs) const -> ErrorOr<u64>;
    auto div(u64 const& rhs) const -> u64;
    auto operator/(u64 const& rhs) const -> u64;

    auto try_div_assign(u64 const& rhs) -> ErrorOr<void>;
    auto div_assign(u64 const& rhs) -> void;
    auto operator/=(u64 const& rhs) -> u64&;

    auto operator%(u64 const& rhs) const -> u64;
    auto operator%=(u64 const& rhs) -> u64&;

    auto atomic_load(MemOrder mem_order = MemOrder::Total) volatile -> u64;
    auto atomic_store(u64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_add(u64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;
    auto atomic_sub(u64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_fetch_add(u64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u64;
    auto atomic_fetch_sub(u64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u64;

    auto atomic_add_fetch(u64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u64;
    auto atomic_sub_fetch(u64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> u64;

    auto hash_code() const -> usize;

private:
    __UINT64_TYPE__ m_value = 0;
};
static_assert(sizeof(u64) == sizeof(__UINT64_TYPE__));

auto operator<<(__UINT64_TYPE__ lhs, u64 rhs) -> u64;
auto operator>>(__UINT64_TYPE__ lhs, u64 rhs) -> u64;
template<typename T>
auto operator+(T* ptr, u64 offset) -> T* {
    return ptr + offset.unwrap();
}
template<typename T>
auto operator-(T* ptr, u64 offset) -> T* {
    return ptr - offset.unwrap();
}

namespace Cxx {

auto swap(u64& lhs, u64& rhs) -> void;

} /* namespace Cxx */
