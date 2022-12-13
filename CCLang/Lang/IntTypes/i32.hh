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

class i32 final {
public:
    using NativeInt = __INT32_TYPE__;

public:
    static auto max() -> i32 {
        return __INT32_MAX__;
    }
    static auto min() -> i32 {
        return -__INT32_MAX__ - 1;
    }

    static auto max(i32 const& lhs, i32 const& rhs) -> i32;
    static auto min(i32 const& lhs, i32 const& rhs) -> i32;

    static auto ceil_div(i32 const& lhs, i32 const& rhs) -> i32;

    static auto range(i32 const& begin, i32 const& end) -> Range<i32>;
    static auto range_inclusive(i32 const& begin, i32 const& last) -> RangeInclusive<i32>;

    explicit(false) i32() = default;
    explicit(false) i32(__INT32_TYPE__ value);

    auto operator=(__INT32_TYPE__ value) -> i32&;

    auto swap(i32& rhs) -> void;

    auto is_signed() const -> bool;

    auto bit_count() const -> usize;

    template<typename TIntWrapper>
    auto as() const -> TIntWrapper {
        return TIntWrapper(static_cast<typename TIntWrapper::NativeInt>(m_value));
    }

    auto unwrap() const -> __INT32_TYPE__;

    auto equal(i32 const& rhs) const -> bool;
    auto operator==(i32 const& rhs) const -> bool;

    auto not_equal(i32 const& rhs) const -> bool;
    auto operator!=(i32 const& rhs) const -> bool;

    auto greater(i32 const& rhs) const -> bool;
    auto operator>(i32 const& rhs) const -> bool;

    auto greater_equal(i32 const& rhs) const -> bool;
    auto operator>=(i32 const& rhs) const -> bool;

    auto less(i32 const& rhs) const -> bool;
    auto operator<(i32 const& rhs) const -> bool;

    auto less_equal(i32 const& rhs) const -> bool;
    auto operator<=(i32 const& rhs) const -> bool;

    auto bit_one_complement() const -> i32;
    auto operator~() const -> i32;

    auto bit_and(i32 const& rhs) const -> i32;
    auto operator&(i32 const& rhs) const -> i32;

    auto bit_and_assign(i32 const& rhs) -> void;
    auto operator&=(i32 const& rhs) -> i32&;

    auto bit_or(i32 const& rhs) const -> i32;
    auto operator|(i32 const& rhs) const -> i32;

    auto bit_or_assign(i32 const& rhs) -> void;
    auto operator|=(i32 const& rhs) -> i32&;

    auto bit_xor(i32 const& rhs) const -> i32;
    auto operator^(i32 const& rhs) const -> i32;

    auto bit_xor_assign(i32 const& rhs) -> void;
    auto operator^=(i32 const& rhs) -> i32&;

    auto try_left_shift(i32 const& rhs) const -> ErrorOr<i32>;
    auto left_shift(i32 const& rhs) const -> i32;
    auto operator<<(i32 const& rhs) const -> i32;

    auto try_left_shift_assign(i32 const& rhs) -> ErrorOr<void>;
    auto left_shift_assign(i32 const& rhs) -> void;
    auto operator<<=(i32 const& rhs) -> i32&;

    auto try_right_shift(i32 const& rhs) const -> ErrorOr<i32>;
    auto right_shift(i32 const& rhs) const -> i32;
    auto operator>>(i32 const& rhs) const -> i32;

    auto try_right_shift_assign(i32 const& rhs) -> ErrorOr<void>;
    auto right_shift_assign(i32 const& rhs) -> void;
    auto operator>>=(i32 const& rhs) -> i32&;

    auto operator++() -> i32&;
    auto operator++(int) -> i32;

    auto operator--() -> i32&;
    auto operator--(int) -> i32;

    auto try_add(i32 const& rhs) const -> ErrorOr<i32>;
    auto add(i32 const& rhs) const -> i32;
    auto operator+(i32 const& rhs) const -> i32;

    auto try_add_assign(i32 const& rhs) -> ErrorOr<void>;
    auto add_assign(i32 const& rhs) -> void;
    auto operator+=(i32 const& rhs) -> i32&;

    auto try_sub(i32 const& rhs) const -> ErrorOr<i32>;
    auto sub(i32 const& rhs) const -> i32;
    auto operator-(i32 const& rhs) const -> i32;

    auto try_sub_assign(i32 const& rhs) -> ErrorOr<void>;
    auto sub_assign(i32 const& rhs) -> void;
    auto operator-=(i32 const& rhs) -> i32&;

    auto try_mul(i32 const& rhs) const -> ErrorOr<i32>;
    auto mul(i32 const& rhs) const -> i32;
    auto operator*(i32 const& rhs) const -> i32;

    auto try_mul_assign(i32 const& rhs) -> ErrorOr<void>;
    auto mul_assign(i32 const& rhs) -> void;
    auto operator*=(i32 const& rhs) -> i32&;

    auto try_div(i32 const& rhs) const -> ErrorOr<i32>;
    auto div(i32 const& rhs) const -> i32;
    auto operator/(i32 const& rhs) const -> i32;

    auto try_div_assign(i32 const& rhs) -> ErrorOr<void>;
    auto div_assign(i32 const& rhs) -> void;
    auto operator/=(i32 const& rhs) -> i32&;

    auto operator%(i32 const& rhs) const -> i32;
    auto operator%=(i32 const& rhs) -> i32&;

    auto atomic_load(MemOrder mem_order = MemOrder::Total) volatile -> i32;
    auto atomic_store(i32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_add(i32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;
    auto atomic_sub(i32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_fetch_add(i32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i32;
    auto atomic_fetch_sub(i32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i32;

    auto atomic_add_fetch(i32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i32;
    auto atomic_sub_fetch(i32 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i32;

    auto hash_code() const -> usize;

private:
    __INT32_TYPE__ m_value = 0;
};
static_assert(sizeof(i32) == sizeof(__INT32_TYPE__));

namespace Cxx {

auto swap(i32& lhs, i32& rhs) -> void;

} /* namespace Cxx */
