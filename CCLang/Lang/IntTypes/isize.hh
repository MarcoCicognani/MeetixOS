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
    using NativeInt = __PTRDIFF_TYPE__;

public:
    static auto max() -> isize {
        return __PTRDIFF_MAX__;
    }
    static auto min() -> isize {
        return -__PTRDIFF_MAX__ - 1;
    }

    static auto max(isize const& lhs, isize const& rhs) -> isize;
    static auto min(isize const& lhs, isize const& rhs) -> isize;

    static auto ceil_div(isize const& lhs, isize const& rhs) -> isize;

    static auto range(isize const& begin, isize const& end) -> Range<isize>;
    static auto range_inclusive(isize const& begin, isize const& last) -> RangeInclusive<isize>;

    explicit(false) isize() = default;
    explicit(false) isize(__PTRDIFF_TYPE__ value);

    auto operator=(__PTRDIFF_TYPE__ value) -> isize&;

    auto swap(isize& rhs) -> void;

    auto is_signed() const -> bool;

    auto bit_count() const -> usize;

    template<typename TIntWrapper>
    auto as() const -> TIntWrapper {
        return TIntWrapper(static_cast<typename TIntWrapper::NativeInt>(m_value));
    }

    auto unwrap() const -> __PTRDIFF_TYPE__;

    auto equal(isize const& rhs) const -> bool;
    auto operator==(isize const& rhs) const -> bool;

    auto not_equal(isize const& rhs) const -> bool;
    auto operator!=(isize const& rhs) const -> bool;

    auto greater(isize const& rhs) const -> bool;
    auto operator>(isize const& rhs) const -> bool;

    auto greater_equal(isize const& rhs) const -> bool;
    auto operator>=(isize const& rhs) const -> bool;

    auto less(isize const& rhs) const -> bool;
    auto operator<(isize const& rhs) const -> bool;

    auto less_equal(isize const& rhs) const -> bool;
    auto operator<=(isize const& rhs) const -> bool;

    auto bit_one_complement() const -> isize;
    auto operator~() const -> isize;

    auto bit_and(isize const& rhs) const -> isize;
    auto operator&(isize const& rhs) const -> isize;

    auto bit_and_assign(isize const& rhs) -> void;
    auto operator&=(isize const& rhs) -> isize&;

    auto bit_or(isize const& rhs) const -> isize;
    auto operator|(isize const& rhs) const -> isize;

    auto bit_or_assign(isize const& rhs) -> void;
    auto operator|=(isize const& rhs) -> isize&;

    auto bit_xor(isize const& rhs) const -> isize;
    auto operator^(isize const& rhs) const -> isize;

    auto bit_xor_assign(isize const& rhs) -> void;
    auto operator^=(isize const& rhs) -> isize&;

    auto try_left_shift(isize const& rhs) const -> ErrorOr<isize>;
    auto left_shift(isize const& rhs) const -> isize;
    auto operator<<(isize const& rhs) const -> isize;

    auto try_left_shift_assign(isize const& rhs) -> ErrorOr<void>;
    auto left_shift_assign(isize const& rhs) -> void;
    auto operator<<=(isize const& rhs) -> isize&;

    auto try_right_shift(isize const& rhs) const -> ErrorOr<isize>;
    auto right_shift(isize const& rhs) const -> isize;
    auto operator>>(isize const& rhs) const -> isize;

    auto try_right_shift_assign(isize const& rhs) -> ErrorOr<void>;
    auto right_shift_assign(isize const& rhs) -> void;
    auto operator>>=(isize const& rhs) -> isize&;

    auto operator++() -> isize&;
    auto operator++(int) -> isize;

    auto operator--() -> isize&;
    auto operator--(int) -> isize;

    auto try_add(isize const& rhs) const -> ErrorOr<isize>;
    auto add(isize const& rhs) const -> isize;
    auto operator+(isize const& rhs) const -> isize;

    auto try_add_assign(isize const& rhs) -> ErrorOr<void>;
    auto add_assign(isize const& rhs) -> void;
    auto operator+=(isize const& rhs) -> isize&;

    auto try_sub(isize const& rhs) const -> ErrorOr<isize>;
    auto sub(isize const& rhs) const -> isize;
    auto operator-(isize const& rhs) const -> isize;

    auto try_sub_assign(isize const& rhs) -> ErrorOr<void>;
    auto sub_assign(isize const& rhs) -> void;
    auto operator-=(isize const& rhs) -> isize&;

    auto try_mul(isize const& rhs) const -> ErrorOr<isize>;
    auto mul(isize const& rhs) const -> isize;
    auto operator*(isize const& rhs) const -> isize;

    auto try_mul_assign(isize const& rhs) -> ErrorOr<void>;
    auto mul_assign(isize const& rhs) -> void;
    auto operator*=(isize const& rhs) -> isize&;

    auto try_div(isize const& rhs) const -> ErrorOr<isize>;
    auto div(isize const& rhs) const -> isize;
    auto operator/(isize const& rhs) const -> isize;

    auto try_div_assign(isize const& rhs) -> ErrorOr<void>;
    auto div_assign(isize const& rhs) -> void;
    auto operator/=(isize const& rhs) -> isize&;

    auto operator%(isize const& rhs) const -> isize;
    auto operator%=(isize const& rhs) -> isize&;

    auto atomic_load(MemOrder mem_order = MemOrder::Total) volatile -> isize;
    auto atomic_store(isize rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_add(isize rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;
    auto atomic_sub(isize rhs, MemOrder mem_order = MemOrder::Total) volatile -> void;

    auto atomic_fetch_add(isize rhs, MemOrder mem_order = MemOrder::Total) volatile -> isize;
    auto atomic_fetch_sub(isize rhs, MemOrder mem_order = MemOrder::Total) volatile -> isize;

    auto atomic_add_fetch(isize rhs, MemOrder mem_order = MemOrder::Total) volatile -> isize;
    auto atomic_sub_fetch(isize rhs, MemOrder mem_order = MemOrder::Total) volatile -> isize;

    auto hash_code() const -> usize;

private:
    __PTRDIFF_TYPE__ m_value = 0;
};
static_assert(sizeof(isize) == sizeof(__PTRDIFF_TYPE__));

namespace Cxx {

auto swap(isize& lhs, isize& rhs) -> void;

} /* namespace Cxx */
