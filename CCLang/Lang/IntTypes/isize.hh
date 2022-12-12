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

class isize final {
public:
    using CCIntegerType = __PTRDIFF_TYPE__;

public:
    static auto max() -> isize {
        return __UINT8_MAX__;
    }
    static auto min() -> isize {
        return 0;
    }

    static auto max(isize const&, isize const&) -> isize;
    static auto min(isize const&, isize const&) -> isize;

    static auto range(isize const&, isize const&) -> Range<isize>;
    static auto range_inclusive(isize const&, isize const&) -> RangeInclusive<isize>;

    explicit(false) isize() = default;
    explicit(false) isize(__PTRDIFF_TYPE__);

    auto operator=(__PTRDIFF_TYPE__) -> isize&;

    auto swap(isize&) -> void;

    auto is_signed() const -> bool;

    auto bit_count() const -> usize;

    template<typename TInteger>
    auto as() const -> TInteger {
        return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };
    }

    auto unwrap() const -> __PTRDIFF_TYPE__;

    auto equal(isize const&) const -> bool;
    auto operator==(isize const&) const -> bool;

    auto not_equal(isize const&) const -> bool;
    auto operator!=(isize const&) const -> bool;

    auto greater(isize const&) const -> bool;
    auto operator>(isize const&) const -> bool;

    auto greater_equal(isize const&) const -> bool;
    auto operator>=(isize const&) const -> bool;

    auto less(isize const&) const -> bool;
    auto operator<(isize const&) const -> bool;

    auto less_equal(isize const&) const -> bool;
    auto operator<=(isize const&) const -> bool;

    auto bit_one_complement() const -> isize;
    auto operator~() const -> isize;

    auto bit_and(isize const&) const -> isize;
    auto operator&(isize const&) const -> isize;

    auto bit_and_assign(isize const&) -> void;
    auto operator&=(isize const&) -> isize&;

    auto bit_or(isize const&) const -> isize;
    auto operator|(isize const&) const -> isize;

    auto bit_or_assign(isize const&) -> void;
    auto operator|=(isize const&) -> isize&;

    auto bit_xor(isize const&) const -> isize;
    auto operator^(isize const&) const -> isize;

    auto bit_xor_assign(isize const&) -> void;
    auto operator^=(isize const&) -> isize&;

    auto try_left_shift(isize const&) const -> ErrorOr<isize>;
    auto left_shift(isize const&) const -> isize;
    auto operator<<(isize const&) const -> isize;

    auto try_left_shift_assign(isize const&) -> ErrorOr<void>;
    auto left_shift_assign(isize const&) -> void;
    auto operator<<=(isize const&) -> isize&;

    auto try_right_shift(isize const&) const -> ErrorOr<isize>;
    auto right_shift(isize const&) const -> isize;
    auto operator>>(isize const&) const -> isize;

    auto try_right_shift_assign(isize const&) -> ErrorOr<void>;
    auto right_shift_assign(isize const&) -> void;
    auto operator>>=(isize const&) -> isize&;

    auto operator++() -> isize&;
    auto operator++(int) -> isize;

    auto operator--() -> isize&;
    auto operator--(int) -> isize;

    auto try_add(isize const&) const -> ErrorOr<isize>;
    auto add(isize const&) const -> isize;
    auto operator+(isize const&) const -> isize;

    auto try_add_assign(isize const&) -> ErrorOr<void>;
    auto add_assign(isize const&) -> void;
    auto operator+=(isize const&) -> isize&;

    auto try_sub(isize const&) const -> ErrorOr<isize>;
    auto sub(isize const&) const -> isize;
    auto operator-(isize const&) const -> isize;

    auto try_sub_assign(isize const&) -> ErrorOr<void>;
    auto sub_assign(isize const&) -> void;
    auto operator-=(isize const&) -> isize&;

    auto try_mul(isize const&) const -> ErrorOr<isize>;
    auto mul(isize const&) const -> isize;
    auto operator*(isize const&) const -> isize;

    auto try_mul_assign(isize const&) -> ErrorOr<void>;
    auto mul_assign(isize const&) -> void;
    auto operator*=(isize const&) -> isize&;

    auto try_div(isize const&) const -> ErrorOr<isize>;
    auto div(isize const&) const -> isize;
    auto operator/(isize const&) const -> isize;

    auto try_div_assign(isize const&) -> ErrorOr<void>;
    auto div_assign(isize const&) -> void;
    auto operator/=(isize const&) -> isize&;

    auto operator%(isize const&) const -> isize;
    auto operator%=(isize const&) -> isize&;

    [[nodiscard]]
    auto atomic_load(MemOrder = MemOrder::Total) volatile -> isize;
    auto atomic_store(isize, MemOrder = MemOrder::Total) volatile -> void;

    auto atomic_add(isize, MemOrder = MemOrder::Total) volatile -> void;
    auto atomic_sub(isize, MemOrder = MemOrder::Total) volatile -> void;

    [[nodiscard]]
    auto atomic_fetch_add(isize, MemOrder = MemOrder::Total) volatile -> isize;
    [[nodiscard]]
    auto atomic_fetch_sub(isize, MemOrder = MemOrder::Total) volatile -> isize;

    [[nodiscard]]
    auto atomic_add_fetch(isize, MemOrder = MemOrder::Total) volatile -> isize;
    [[nodiscard]]
    auto atomic_sub_fetch(isize, MemOrder = MemOrder::Total) volatile -> isize;

    [[nodiscard]]
    auto hash_code() const -> usize;

private:
    __PTRDIFF_TYPE__ m_value = 0;
};
static_assert(sizeof(isize) == sizeof(__PTRDIFF_TYPE__));

namespace Cxx {

auto swap(isize& lhs, isize& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
