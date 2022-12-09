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

class i8 final {
public:
    using CCIntegerType = __INT8_TYPE__;

public:
    static auto max() -> i8 {
        return __UINT8_MAX__;
    }
    static auto min() -> i8 {
        return 0;
    }

    static auto max(i8 const&, i8 const&) -> i8;
    static auto min(i8 const&, i8 const&) -> i8;

    static auto range(i8 const&, i8 const&) -> Range<i8>;
    static auto range_inclusive(i8 const&, i8 const&) -> RangeInclusive<i8>;

    explicit(false) i8()           = default;
    explicit(false) i8(i8 const&) = default;

    explicit(false) i8(__INT8_TYPE__);
    explicit(false) i8(i8&&);

    ~i8() = default;

    auto operator=(__INT8_TYPE__) -> i8&;
    auto operator=(i8 const&) -> i8&;
    auto operator=(i8&&) -> i8&;

    auto swap(i8&) -> void;

    auto is_signed() const -> bool;

    auto bit_count() const -> usize;

    template<typename TInteger>
    auto as() const -> TInteger {
        return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };
    }

    auto unwrap() const -> __INT8_TYPE__;

    auto equal(i8 const&) const -> bool;
    auto operator==(i8 const&) const -> bool;

    auto not_equal(i8 const&) const -> bool;
    auto operator!=(i8 const&) const -> bool;

    auto greater(i8 const&) const -> bool;
    auto operator>(i8 const&) const -> bool;

    auto greater_equal(i8 const&) const -> bool;
    auto operator>=(i8 const&) const -> bool;

    auto less(i8 const&) const -> bool;
    auto operator<(i8 const&) const -> bool;

    auto less_equal(i8 const&) const -> bool;
    auto operator<=(i8 const&) const -> bool;

    auto bit_one_complement() const -> i8;
    auto operator~() const -> i8;

    auto bit_and(i8 const&) const -> i8;
    auto operator&(i8 const&) const -> i8;

    auto bit_and_assign(i8 const&) -> void;
    auto operator&=(i8 const&) -> i8&;

    auto bit_or(i8 const&) const -> i8;
    auto operator|(i8 const&) const -> i8;

    auto bit_or_assign(i8 const&) -> void;
    auto operator|=(i8 const&) -> i8&;

    auto bit_xor(i8 const&) const -> i8;
    auto operator^(i8 const&) const -> i8;

    auto bit_xor_assign(i8 const&) -> void;
    auto operator^=(i8 const&) -> i8&;

    auto try_left_shift(i8 const&) const -> ErrorOr<i8>;
    auto left_shift(i8 const&) const -> i8;
    auto operator<<(i8 const&) const -> i8;

    auto try_left_shift_assign(i8 const&) -> ErrorOr<void>;
    auto left_shift_assign(i8 const&) -> void;
    auto operator<<=(i8 const&) -> i8&;

    auto try_right_shift(i8 const&) const -> ErrorOr<i8>;
    auto right_shift(i8 const&) const -> i8;
    auto operator>>(i8 const&) const -> i8;

    auto try_right_shift_assign(i8 const&) -> ErrorOr<void>;
    auto right_shift_assign(i8 const&) -> void;
    auto operator>>=(i8 const&) -> i8&;

    auto operator++() -> i8&;
    auto operator++(int) -> i8;

    auto operator--() -> i8&;
    auto operator--(int) -> i8;

    auto try_add(i8 const&) const -> ErrorOr<i8>;
    auto add(i8 const&) const -> i8;
    auto operator+(i8 const&) const -> i8;

    auto try_add_assign(i8 const&) -> ErrorOr<void>;
    auto add_assign(i8 const&) -> void;
    auto operator+=(i8 const&) -> i8&;

    auto try_sub(i8 const&) const -> ErrorOr<i8>;
    auto sub(i8 const&) const -> i8;
    auto operator-(i8 const&) const -> i8;

    auto try_sub_assign(i8 const&) -> ErrorOr<void>;
    auto sub_assign(i8 const&) -> void;
    auto operator-=(i8 const&) -> i8&;

    auto try_mul(i8 const&) const -> ErrorOr<i8>;
    auto mul(i8 const&) const -> i8;
    auto operator*(i8 const&) const -> i8;

    auto try_mul_assign(i8 const&) -> ErrorOr<void>;
    auto mul_assign(i8 const&) -> void;
    auto operator*=(i8 const&) -> i8&;

    auto try_div(i8 const&) const -> ErrorOr<i8>;
    auto div(i8 const&) const -> i8;
    auto operator/(i8 const&) const -> i8;

    auto try_div_assign(i8 const&) -> ErrorOr<void>;
    auto div_assign(i8 const&) -> void;
    auto operator/=(i8 const&) -> i8&;

    auto operator%(i8 const&) const -> i8;
    auto operator%=(i8 const&) -> i8&;

    [[nodiscard]]
    auto atomic_load(MemOrder = MemOrder::Total) volatile -> i8;
    auto atomic_store(i8, MemOrder = MemOrder::Total) volatile -> void;

    auto atomic_add(i8, MemOrder = MemOrder::Total) volatile -> void;
    auto atomic_sub(i8, MemOrder = MemOrder::Total) volatile -> void;

    [[nodiscard]]
    auto atomic_fetch_add(i8, MemOrder = MemOrder::Total) volatile -> i8;
    [[nodiscard]]
    auto atomic_fetch_sub(i8, MemOrder = MemOrder::Total) volatile -> i8;

    [[nodiscard]]
    auto atomic_add_fetch(i8, MemOrder = MemOrder::Total) volatile -> i8;
    [[nodiscard]]
    auto atomic_sub_fetch(i8, MemOrder = MemOrder::Total) volatile -> i8;

    [[nodiscard]]
    auto hash_code() const -> usize;

private:
    __INT8_TYPE__ m_value = 0;
};
static_assert(sizeof(i8) == sizeof(__INT8_TYPE__));

namespace Cxx {

auto swap(i8& lhs, i8& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
