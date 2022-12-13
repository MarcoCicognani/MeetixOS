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

class i16 final {
public:
    using NativeInt = __INT16_TYPE__;

public:
    static auto max() -> i16 {
        return __INT16_MAX__;
    }
    static auto min() -> i16 {
        return -__INT16_MAX__ - 1;
    }

    static auto max(i16 const& lhs, i16 const& rhs) -> i16;
    static auto min(i16 const& lhs, i16 const& rhs) -> i16;

    static auto ceil_div(i16 const& lhs, i16 const& rhs) -> i16;

    static auto range(i16 const& begin, i16 const& end) -> Range<i16>;
    static auto range_inclusive(i16 const& begin, i16 const& last) -> RangeInclusive<i16>;

    explicit(false) i16() = default;
    explicit(false) i16(__INT16_TYPE__ value);

    auto operator=(__INT16_TYPE__ value) -> i16&;

    auto swap(i16& rhs) -> void;

    auto is_signed() const -> bool;

    auto bit_count() const -> usize;

    template<typename TIntWrapper>
    auto as() const -> TIntWrapper {
        return TIntWrapper(static_cast<typename TIntWrapper::NativeInt>(m_value));
    }

    auto unwrap() const -> __INT16_TYPE__;

    auto equal(i16 const& rhs) const -> bool;
    auto operator==(i16 const& rhs) const -> bool;

    auto not_equal(i16 const& rhs) const -> bool;
    auto operator!=(i16 const& rhs) const -> bool;

    auto greater(i16 const& rhs) const -> bool;
    auto operator>(i16 const& rhs) const -> bool;

    auto greater_equal(i16 const& rhs) const -> bool;
    auto operator>=(i16 const& rhs) const -> bool;

    auto less(i16 const& rhs) const -> bool;
    auto operator<(i16 const& rhs) const -> bool;

    auto less_equal(i16 const& rhs) const -> bool;
    auto operator<=(i16 const& rhs) const -> bool;

    auto bit_one_complement() const -> i16;
    auto operator~() const -> i16;

    auto bit_and(i16 const& rhs) const -> i16;
    auto operator&(i16 const& rhs) const -> i16;

    auto bit_and_assign(i16 const& rhs) -> void;
    auto operator&=(i16 const& rhs) -> i16&;

    auto bit_or(i16 const& rhs) const -> i16;
    auto operator|(i16 const& rhs) const -> i16;

    auto bit_or_assign(i16 const& rhs) -> void;
    auto operator|=(i16 const& rhs) -> i16&;

    auto bit_xor(i16 const& rhs) const -> i16;
    auto operator^(i16 const& rhs) const -> i16;

    auto bit_xor_assign(i16 const& rhs) -> void;
    auto operator^=(i16 const& rhs) -> i16&;

    auto try_left_shift(i16 const& rhs) const -> ErrorOr<i16>;
    auto left_shift(i16 const& rhs) const -> i16;
    auto operator<<(i16 const& rhs) const -> i16;

    auto try_left_shift_assign(i16 const& rhs) -> ErrorOr<void>;
    auto left_shift_assign(i16 const& rhs) -> void;
    auto operator<<=(i16 const& rhs) -> i16&;

    auto try_right_shift(i16 const& rhs) const -> ErrorOr<i16>;
    auto right_shift(i16 const& rhs) const -> i16;
    auto operator>>(i16 const& rhs) const -> i16;

    auto try_right_shift_assign(i16 const& rhs) -> ErrorOr<void>;
    auto right_shift_assign(i16 const& rhs) -> void;
    auto operator>>=(i16 const& rhs) -> i16&;

    auto operator++() -> i16&;
    auto operator++(int) -> i16;

    auto operator--() -> i16&;
    auto operator--(int) -> i16;

    auto try_add(i16 const& rhs) const -> ErrorOr<i16>;
    auto add(i16 const& rhs) const -> i16;
    auto operator+(i16 const& rhs) const -> i16;

    auto try_add_assign(i16 const& rhs) -> ErrorOr<void>;
    auto add_assign(i16 const& rhs) -> void;
    auto operator+=(i16 const& rhs) -> i16&;

    auto try_sub(i16 const& rhs) const -> ErrorOr<i16>;
    auto sub(i16 const& rhs) const -> i16;
    auto operator-(i16 const& rhs) const -> i16;

    auto try_sub_assign(i16 const& rhs) -> ErrorOr<void>;
    auto sub_assign(i16 const& rhs) -> void;
    auto operator-=(i16 const& rhs) -> i16&;

    auto try_mul(i16 const& rhs) const -> ErrorOr<i16>;
    auto mul(i16 const& rhs) const -> i16;
    auto operator*(i16 const& rhs) const -> i16;

    auto try_mul_assign(i16 const& rhs) -> ErrorOr<void>;
    auto mul_assign(i16 const& rhs) -> void;
    auto operator*=(i16 const& rhs) -> i16&;

    auto try_div(i16 const& rhs) const -> ErrorOr<i16>;
    auto div(i16 const& rhs) const -> i16;
    auto operator/(i16 const& rhs) const -> i16;

    auto try_div_assign(i16 const& rhs) -> ErrorOr<void>;
    auto div_assign(i16 const& rhs) -> void;
    auto operator/=(i16 const& rhs) -> i16&;

    auto operator%(i16 const& rhs) const -> i16;
    auto operator%=(i16 const& rhs) -> i16&;

    auto atomic_load(MemOrder mem_order = MemOrder::Total) volatile -> i16;
    auto atomic_store(i16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_add(i16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;
    auto atomic_sub(i16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_fetch_add(i16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i16;
    auto atomic_fetch_sub(i16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i16;

    auto atomic_add_fetch(i16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i16;
    auto atomic_sub_fetch(i16 rhs, MemOrder mem_order = MemOrder::Total) volatile -> i16;

    auto hash_code() const -> usize;

private:
    __INT16_TYPE__ m_value = 0;
};
static_assert(sizeof(i16) == sizeof(__INT16_TYPE__));

namespace Cxx {

auto swap(i16& lhs, i16& rhs) -> void;

} /* namespace Cxx */
