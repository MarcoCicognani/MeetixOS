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
    using CCIntegerType = __INT32_TYPE__;

public:
    static auto max() -> i32 {
        return __UINT8_MAX__;
    }
    static auto min() -> i32 {
        return 0;
    }

    static auto max(i32 const&, i32 const&) -> i32;
    static auto min(i32 const&, i32 const&) -> i32;

    static auto range(i32 const&, i32 const&) -> Range<i32>;
    static auto range_inclusive(i32 const&, i32 const&) -> RangeInclusive<i32>;

    explicit(false) i32() = default;
    explicit(false) i32(__INT32_TYPE__);

    auto operator=(__INT32_TYPE__) -> i32&;

    auto swap(i32&) -> void;

    auto is_signed() const -> bool;

    auto bit_count() const -> usize;

    template<typename TInteger>
    auto as() const -> TInteger {
        return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };
    }

    auto unwrap() const -> __INT32_TYPE__;

    auto equal(i32 const&) const -> bool;
    auto operator==(i32 const&) const -> bool;

    auto not_equal(i32 const&) const -> bool;
    auto operator!=(i32 const&) const -> bool;

    auto greater(i32 const&) const -> bool;
    auto operator>(i32 const&) const -> bool;

    auto greater_equal(i32 const&) const -> bool;
    auto operator>=(i32 const&) const -> bool;

    auto less(i32 const&) const -> bool;
    auto operator<(i32 const&) const -> bool;

    auto less_equal(i32 const&) const -> bool;
    auto operator<=(i32 const&) const -> bool;

    auto bit_one_complement() const -> i32;
    auto operator~() const -> i32;

    auto bit_and(i32 const&) const -> i32;
    auto operator&(i32 const&) const -> i32;

    auto bit_and_assign(i32 const&) -> void;
    auto operator&=(i32 const&) -> i32&;

    auto bit_or(i32 const&) const -> i32;
    auto operator|(i32 const&) const -> i32;

    auto bit_or_assign(i32 const&) -> void;
    auto operator|=(i32 const&) -> i32&;

    auto bit_xor(i32 const&) const -> i32;
    auto operator^(i32 const&) const -> i32;

    auto bit_xor_assign(i32 const&) -> void;
    auto operator^=(i32 const&) -> i32&;

    auto try_left_shift(i32 const&) const -> ErrorOr<i32>;
    auto left_shift(i32 const&) const -> i32;
    auto operator<<(i32 const&) const -> i32;

    auto try_left_shift_assign(i32 const&) -> ErrorOr<void>;
    auto left_shift_assign(i32 const&) -> void;
    auto operator<<=(i32 const&) -> i32&;

    auto try_right_shift(i32 const&) const -> ErrorOr<i32>;
    auto right_shift(i32 const&) const -> i32;
    auto operator>>(i32 const&) const -> i32;

    auto try_right_shift_assign(i32 const&) -> ErrorOr<void>;
    auto right_shift_assign(i32 const&) -> void;
    auto operator>>=(i32 const&) -> i32&;

    auto operator++() -> i32&;
    auto operator++(int) -> i32;

    auto operator--() -> i32&;
    auto operator--(int) -> i32;

    auto try_add(i32 const&) const -> ErrorOr<i32>;
    auto add(i32 const&) const -> i32;
    auto operator+(i32 const&) const -> i32;

    auto try_add_assign(i32 const&) -> ErrorOr<void>;
    auto add_assign(i32 const&) -> void;
    auto operator+=(i32 const&) -> i32&;

    auto try_sub(i32 const&) const -> ErrorOr<i32>;
    auto sub(i32 const&) const -> i32;
    auto operator-(i32 const&) const -> i32;

    auto try_sub_assign(i32 const&) -> ErrorOr<void>;
    auto sub_assign(i32 const&) -> void;
    auto operator-=(i32 const&) -> i32&;

    auto try_mul(i32 const&) const -> ErrorOr<i32>;
    auto mul(i32 const&) const -> i32;
    auto operator*(i32 const&) const -> i32;

    auto try_mul_assign(i32 const&) -> ErrorOr<void>;
    auto mul_assign(i32 const&) -> void;
    auto operator*=(i32 const&) -> i32&;

    auto try_div(i32 const&) const -> ErrorOr<i32>;
    auto div(i32 const&) const -> i32;
    auto operator/(i32 const&) const -> i32;

    auto try_div_assign(i32 const&) -> ErrorOr<void>;
    auto div_assign(i32 const&) -> void;
    auto operator/=(i32 const&) -> i32&;

    auto operator%(i32 const&) const -> i32;
    auto operator%=(i32 const&) -> i32&;

    [[nodiscard]]
    auto atomic_load(MemOrder = MemOrder::Total) volatile -> i32;
    auto atomic_store(i32, MemOrder = MemOrder::Total) volatile -> void;

    auto atomic_add(i32, MemOrder = MemOrder::Total) volatile -> void;
    auto atomic_sub(i32, MemOrder = MemOrder::Total) volatile -> void;

    [[nodiscard]]
    auto atomic_fetch_add(i32, MemOrder = MemOrder::Total) volatile -> i32;
    [[nodiscard]]
    auto atomic_fetch_sub(i32, MemOrder = MemOrder::Total) volatile -> i32;

    [[nodiscard]]
    auto atomic_add_fetch(i32, MemOrder = MemOrder::Total) volatile -> i32;
    [[nodiscard]]
    auto atomic_sub_fetch(i32, MemOrder = MemOrder::Total) volatile -> i32;

    [[nodiscard]]
    auto hash_code() const -> usize;

private:
    __INT32_TYPE__ m_value = 0;
};
static_assert(sizeof(i32) == sizeof(__INT32_TYPE__));

namespace Cxx {

auto swap(i32& lhs, i32& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
