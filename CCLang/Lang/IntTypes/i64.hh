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
    using NativeInt = __INT64_TYPE__;

public:
    static auto max() -> i64 {
        return __INT64_MAX__;
    }
    static auto min() -> i64 {
        return -__INT64_MAX__ - 1;
    }

    static auto max(i64 const& lhs, i64 const& rhs) -> i64;
    static auto min(i64 const& lhs, i64 const& rhs) -> i64;

    static auto ceil_div(i64 const& lhs, i64 const& rhs) -> i64;

    static auto range(i64 const& begin, i64 const& end) -> Range<i64>;
    static auto range_inclusive(i64 const& begin, i64 const& last) -> RangeInclusive<i64>;

    explicit(false) i64() = default;
    explicit(false) i64(__INT64_TYPE__ value);

    auto operator=(__INT64_TYPE__ value) -> i64&;

    auto swap(i64& rhs) -> void;

    auto is_signed() const -> bool;

    auto bit_count() const -> usize;

    template<typename TIntWrapper>
    auto as() const -> TIntWrapper {
        return TIntWrapper(static_cast<typename TIntWrapper::NativeInt>(m_value));
    }

    auto unwrap() const -> __INT64_TYPE__;

    auto equal(i64 const& rhs) const -> bool;
    auto operator==(i64 const& rhs) const -> bool;

    auto not_equal(i64 const& rhs) const -> bool;
    auto operator!=(i64 const& rhs) const -> bool;

    auto greater(i64 const& rhs) const -> bool;
    auto operator>(i64 const& rhs) const -> bool;

    auto greater_equal(i64 const& rhs) const -> bool;
    auto operator>=(i64 const& rhs) const -> bool;

    auto less(i64 const& rhs) const -> bool;
    auto operator<(i64 const& rhs) const -> bool;

    auto less_equal(i64 const& rhs) const -> bool;
    auto operator<=(i64 const& rhs) const -> bool;

    auto bit_one_complement() const -> i64;
    auto operator~() const -> i64;

    auto bit_and(i64 const& rhs) const -> i64;
    auto operator&(i64 const& rhs) const -> i64;

    auto bit_and_assign(i64 const& rhs) -> void;
    auto operator&=(i64 const& rhs) -> i64&;

    auto bit_or(i64 const& rhs) const -> i64;
    auto operator|(i64 const& rhs) const -> i64;

    auto bit_or_assign(i64 const& rhs) -> void;
    auto operator|=(i64 const& rhs) -> i64&;

    auto bit_xor(i64 const& rhs) const -> i64;
    auto operator^(i64 const& rhs) const -> i64;

    auto bit_xor_assign(i64 const& rhs) -> void;
    auto operator^=(i64 const& rhs) -> i64&;

    auto try_left_shift(i64 const& rhs) const -> ErrorOr<i64>;
    auto left_shift(i64 const& rhs) const -> i64;
    auto operator<<(i64 const& rhs) const -> i64;

    auto try_left_shift_assign(i64 const& rhs) -> ErrorOr<void>;
    auto left_shift_assign(i64 const& rhs) -> void;
    auto operator<<=(i64 const& rhs) -> i64&;

    auto try_right_shift(i64 const& rhs) const -> ErrorOr<i64>;
    auto right_shift(i64 const& rhs) const -> i64;
    auto operator>>(i64 const& rhs) const -> i64;

    auto try_right_shift_assign(i64 const& rhs) -> ErrorOr<void>;
    auto right_shift_assign(i64 const& rhs) -> void;
    auto operator>>=(i64 const& rhs) -> i64&;

    auto operator++() -> i64&;
    auto operator++(int) -> i64;

    auto operator--() -> i64&;
    auto operator--(int) -> i64;

    auto try_add(i64 const& rhs) const -> ErrorOr<i64>;
    auto add(i64 const& rhs) const -> i64;
    auto operator+(i64 const& rhs) const -> i64;

    auto try_add_assign(i64 const& rhs) -> ErrorOr<void>;
    auto add_assign(i64 const& rhs) -> void;
    auto operator+=(i64 const& rhs) -> i64&;

    auto try_sub(i64 const& rhs) const -> ErrorOr<i64>;
    auto sub(i64 const& rhs) const -> i64;
    auto operator-(i64 const& rhs) const -> i64;

    auto try_sub_assign(i64 const& rhs) -> ErrorOr<void>;
    auto sub_assign(i64 const& rhs) -> void;
    auto operator-=(i64 const& rhs) -> i64&;

    auto try_mul(i64 const& rhs) const -> ErrorOr<i64>;
    auto mul(i64 const& rhs) const -> i64;
    auto operator*(i64 const& rhs) const -> i64;

    auto try_mul_assign(i64 const& rhs) -> ErrorOr<void>;
    auto mul_assign(i64 const& rhs) -> void;
    auto operator*=(i64 const& rhs) -> i64&;

    auto try_div(i64 const& rhs) const -> ErrorOr<i64>;
    auto div(i64 const& rhs) const -> i64;
    auto operator/(i64 const& rhs) const -> i64;

    auto try_div_assign(i64 const& rhs) -> ErrorOr<void>;
    auto div_assign(i64 const& rhs) -> void;
    auto operator/=(i64 const& rhs) -> i64&;

    auto operator%(i64 const& rhs) const -> i64;
    auto operator%=(i64 const& rhs) -> i64&;

    auto atomic_load(MemOrder mem_order = MemOrder::Total) volatile -> i64;
    auto atomic_store(i64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_add(i64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;
    auto atomic_sub(i64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_fetch_add(i64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i64;
    auto atomic_fetch_sub(i64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i64;

    auto atomic_add_fetch(i64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i64;
    auto atomic_sub_fetch(i64 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i64;

    auto hash_code() const -> usize;

private:
    __INT64_TYPE__ m_value = 0;
};
static_assert(sizeof(i64) == sizeof(__INT64_TYPE__));

namespace Cxx {

auto swap(i64& lhs, i64& rhs) -> void;

} /* namespace Cxx */
