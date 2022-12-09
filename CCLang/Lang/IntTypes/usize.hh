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

class usize final {
public:
    using CCIntegerType = __SIZE_TYPE__;

public:
    static auto max() -> usize {
        return __SIZE_MAX__;
    }
    static auto min() -> usize {
        return 0;
    }

    static auto max(usize const&, usize const&) -> usize;
    static auto min(usize const&, usize const&) -> usize;

    static auto range(usize const&, usize const&) -> Range<usize>;
    static auto range_inclusive(usize const&, usize const&) -> RangeInclusive<usize>;

    explicit(false) usize()             = default;
    explicit(false) usize(usize const&) = default;

    explicit(false) usize(__SIZE_TYPE__);
    explicit(false) usize(usize&&);

    ~usize() = default;

    auto operator=(__SIZE_TYPE__) -> usize&;
    auto operator=(usize const&) -> usize&;
    auto operator=(usize&&) -> usize&;

    auto swap(usize&) -> void;

    auto is_signed() const -> bool;

    auto try_bit_at(usize) const -> ErrorOr<bool>;
    auto bit_at(usize) const -> bool;

    auto try_set_bit(usize, bool) -> ErrorOr<void>;
    auto set_bit(usize, bool) -> void;

    auto count_zeroes() const -> usize;
    auto count_ones() const -> usize;

    auto bit_count() const -> usize;

    template<typename TInteger>
    auto as() const -> TInteger {
        return TInteger{ static_cast<typename TInteger::CCIntegerType>(m_value) };
    }

    auto unwrap() const -> __SIZE_TYPE__;

    auto equal(usize const&) const -> bool;
    auto operator==(usize const&) const -> bool;

    auto not_equal(usize const&) const -> bool;
    auto operator!=(usize const&) const -> bool;

    auto greater(usize const&) const -> bool;
    auto operator>(usize const&) const -> bool;

    auto greater_equal(usize const&) const -> bool;
    auto operator>=(usize const&) const -> bool;

    auto less(usize const&) const -> bool;
    auto operator<(usize const&) const -> bool;

    auto less_equal(usize const&) const -> bool;
    auto operator<=(usize const&) const -> bool;

    auto bit_one_complement() const -> usize;
    auto operator~() const -> usize;

    auto bit_and(usize const&) const -> usize;
    auto operator&(usize const&) const -> usize;

    auto bit_and_assign(usize const&) -> void;
    auto operator&=(usize const&) -> usize&;

    auto bit_or(usize const&) const -> usize;
    auto operator|(usize const&) const -> usize;

    auto bit_or_assign(usize const&) -> void;
    auto operator|=(usize const&) -> usize&;

    auto bit_xor(usize const&) const -> usize;
    auto operator^(usize const&) const -> usize;

    auto bit_xor_assign(usize const&) -> void;
    auto operator^=(usize const&) -> usize&;

    auto try_left_shift(usize const&) const -> ErrorOr<usize>;
    auto left_shift(usize const&) const -> usize;
    auto operator<<(usize const&) const -> usize;

    auto try_left_shift_assign(usize const&) -> ErrorOr<void>;
    auto left_shift_assign(usize const&) -> void;
    auto operator<<=(usize const&) -> usize&;

    auto try_right_shift(usize const&) const -> ErrorOr<usize>;
    auto right_shift(usize const&) const -> usize;
    auto operator>>(usize const&) const -> usize;

    auto try_right_shift_assign(usize const&) -> ErrorOr<void>;
    auto right_shift_assign(usize const&) -> void;
    auto operator>>=(usize const&) -> usize&;

    auto operator++() -> usize&;
    auto operator++(int) -> usize;

    auto operator--() -> usize&;
    auto operator--(int) -> usize;

    auto try_add(usize const&) const -> ErrorOr<usize>;
    auto add(usize const&) const -> usize;
    auto operator+(usize const&) const -> usize;

    auto try_add_assign(usize const&) -> ErrorOr<void>;
    auto add_assign(usize const&) -> void;
    auto operator+=(usize const&) -> usize&;

    auto try_sub(usize const&) const -> ErrorOr<usize>;
    auto sub(usize const&) const -> usize;
    auto operator-(usize const&) const -> usize;

    auto try_sub_assign(usize const&) -> ErrorOr<void>;
    auto sub_assign(usize const&) -> void;
    auto operator-=(usize const&) -> usize&;

    auto try_mul(usize const&) const -> ErrorOr<usize>;
    auto mul(usize const&) const -> usize;
    auto operator*(usize const&) const -> usize;

    auto try_mul_assign(usize const&) -> ErrorOr<void>;
    auto mul_assign(usize const&) -> void;
    auto operator*=(usize const&) -> usize&;

    auto try_div(usize const&) const -> ErrorOr<usize>;
    auto div(usize const&) const -> usize;
    auto operator/(usize const&) const -> usize;

    auto try_div_assign(usize const&) -> ErrorOr<void>;
    auto div_assign(usize const&) -> void;
    auto operator/=(usize const&) -> usize&;

    auto operator%(usize const&) const -> usize;
    auto operator%=(usize const&) -> usize&;

    [[nodiscard]]
    auto atomic_load(MemOrder = MemOrder::Total) volatile -> usize;
    auto atomic_store(usize, MemOrder = MemOrder::Total) volatile -> void;

    auto atomic_add(usize, MemOrder = MemOrder::Total) volatile -> void;
    auto atomic_sub(usize, MemOrder = MemOrder::Total) volatile -> void;

    [[nodiscard]]
    auto atomic_fetch_add(usize, MemOrder = MemOrder::Total) volatile -> usize;
    [[nodiscard]]
    auto atomic_fetch_sub(usize, MemOrder = MemOrder::Total) volatile -> usize;

    [[nodiscard]]
    auto atomic_add_fetch(usize, MemOrder = MemOrder::Total) volatile -> usize;
    [[nodiscard]]
    auto atomic_sub_fetch(usize, MemOrder = MemOrder::Total) volatile -> usize;

    [[nodiscard]]
    auto hash_code() const -> usize;

private:
    __SIZE_TYPE__ m_value = 0;
};
static_assert(sizeof(usize) == sizeof(__SIZE_TYPE__));

auto operator<<(__SIZE_TYPE__ lhs, usize rhs) -> usize;
auto operator>>(__SIZE_TYPE__ lhs, usize rhs) -> usize;

template<typename T>
auto operator+(T* ptr, usize offset) -> T* {
    return ptr + offset.unwrap();
}
template<typename T>
auto operator-(T* ptr, usize offset) -> T* {
    return ptr - offset.unwrap();
}

namespace Cxx {

auto swap(usize& lhs, usize& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */
