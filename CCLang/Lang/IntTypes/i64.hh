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

class i64 final {
public:
    using CCIntegerType = __INT64_TYPE__;

public:
    static auto max() -> i64 {
        return __UINT8_MAX__;
    }
    static auto min() -> i64 {
        return 0;
    }

    static auto max(i64 const&, i64 const&) -> i64;
    static auto min(i64 const&, i64 const&) -> i64;

    static auto range(i64 const&, i64 const&) -> Range<i64>;
    static auto range_inclusive(i64 const&, i64 const&) -> RangeInclusive<i64>;

    explicit(false) i64() = default;
    explicit(false) i64(__INT64_TYPE__);

    auto operator=(__INT64_TYPE__) -> i64&;

    auto swap(i64&) -> void;

    auto is_signed() const -> bool;

    auto bit_count() const -> usize;

    template<typename TInteger>
    auto as() const -> TInteger {
        return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };
    }

    auto unwrap() const -> __INT64_TYPE__;

    auto equal(i64 const&) const -> bool;
    auto operator==(i64 const&) const -> bool;

    auto not_equal(i64 const&) const -> bool;
    auto operator!=(i64 const&) const -> bool;

    auto greater(i64 const&) const -> bool;
    auto operator>(i64 const&) const -> bool;

    auto greater_equal(i64 const&) const -> bool;
    auto operator>=(i64 const&) const -> bool;

    auto less(i64 const&) const -> bool;
    auto operator<(i64 const&) const -> bool;

    auto less_equal(i64 const&) const -> bool;
    auto operator<=(i64 const&) const -> bool;

    auto bit_one_complement() const -> i64;
    auto operator~() const -> i64;

    auto bit_and(i64 const&) const -> i64;
    auto operator&(i64 const&) const -> i64;

    auto bit_and_assign(i64 const&) -> void;
    auto operator&=(i64 const&) -> i64&;

    auto bit_or(i64 const&) const -> i64;
    auto operator|(i64 const&) const -> i64;

    auto bit_or_assign(i64 const&) -> void;
    auto operator|=(i64 const&) -> i64&;

    auto bit_xor(i64 const&) const -> i64;
    auto operator^(i64 const&) const -> i64;

    auto bit_xor_assign(i64 const&) -> void;
    auto operator^=(i64 const&) -> i64&;

    auto try_left_shift(i64 const&) const -> ErrorOr<i64>;
    auto left_shift(i64 const&) const -> i64;
    auto operator<<(i64 const&) const -> i64;

    auto try_left_shift_assign(i64 const&) -> ErrorOr<void>;
    auto left_shift_assign(i64 const&) -> void;
    auto operator<<=(i64 const&) -> i64&;

    auto try_right_shift(i64 const&) const -> ErrorOr<i64>;
    auto right_shift(i64 const&) const -> i64;
    auto operator>>(i64 const&) const -> i64;

    auto try_right_shift_assign(i64 const&) -> ErrorOr<void>;
    auto right_shift_assign(i64 const&) -> void;
    auto operator>>=(i64 const&) -> i64&;

    auto operator++() -> i64&;
    auto operator++(int) -> i64;

    auto operator--() -> i64&;
    auto operator--(int) -> i64;

    auto try_add(i64 const&) const -> ErrorOr<i64>;
    auto add(i64 const&) const -> i64;
    auto operator+(i64 const&) const -> i64;

    auto try_add_assign(i64 const&) -> ErrorOr<void>;
    auto add_assign(i64 const&) -> void;
    auto operator+=(i64 const&) -> i64&;

    auto try_sub(i64 const&) const -> ErrorOr<i64>;
    auto sub(i64 const&) const -> i64;
    auto operator-(i64 const&) const -> i64;

    auto try_sub_assign(i64 const&) -> ErrorOr<void>;
    auto sub_assign(i64 const&) -> void;
    auto operator-=(i64 const&) -> i64&;

    auto try_mul(i64 const&) const -> ErrorOr<i64>;
    auto mul(i64 const&) const -> i64;
    auto operator*(i64 const&) const -> i64;

    auto try_mul_assign(i64 const&) -> ErrorOr<void>;
    auto mul_assign(i64 const&) -> void;
    auto operator*=(i64 const&) -> i64&;

    auto try_div(i64 const&) const -> ErrorOr<i64>;
    auto div(i64 const&) const -> i64;
    auto operator/(i64 const&) const -> i64;

    auto try_div_assign(i64 const&) -> ErrorOr<void>;
    auto div_assign(i64 const&) -> void;
    auto operator/=(i64 const&) -> i64&;

    auto operator%(i64 const&) const -> i64;
    auto operator%=(i64 const&) -> i64&;

    [[nodiscard]]
    auto atomic_load(MemOrder = MemOrder::Total) volatile -> i64;
    auto atomic_store(i64, MemOrder = MemOrder::Total) volatile -> void;

    auto atomic_add(i64, MemOrder = MemOrder::Total) volatile -> void;
    auto atomic_sub(i64, MemOrder = MemOrder::Total) volatile -> void;

    [[nodiscard]]
    auto atomic_fetch_add(i64, MemOrder = MemOrder::Total) volatile -> i64;
    [[nodiscard]]
    auto atomic_fetch_sub(i64, MemOrder = MemOrder::Total) volatile -> i64;

    [[nodiscard]]
    auto atomic_add_fetch(i64, MemOrder = MemOrder::Total) volatile -> i64;
    [[nodiscard]]
    auto atomic_sub_fetch(i64, MemOrder = MemOrder::Total) volatile -> i64;

    [[nodiscard]]
    auto hash_code() const -> usize;

private:
    __INT64_TYPE__ m_value = 0;
};
static_assert(sizeof(i64) == sizeof(__INT64_TYPE__));

namespace Cxx {

auto swap(i64& lhs, i64& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
