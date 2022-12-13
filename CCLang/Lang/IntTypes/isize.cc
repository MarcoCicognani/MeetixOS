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

#include <CCLang/Lang/IntTypes/isize.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto isize::max(isize const& lhs, isize const& rhs) -> isize {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto isize::min(isize const& lhs, isize const& rhs) -> isize {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto isize::ceil_div(isize const& lhs, isize const& rhs) -> isize {
    isize res = lhs / rhs;
    if ( (lhs % rhs) != 0 ) {
        res += 1;
    }
    return res;
}

auto isize::range(isize const& begin, isize const& end) -> Range<isize> {
    return Range<isize>(begin, end);
}

auto isize::range_inclusive(isize const& begin, isize const& last) -> RangeInclusive<isize> {
    return RangeInclusive<isize>(begin, last);
}

isize::isize(__PTRDIFF_TYPE__ value)
    : m_value(value) {
}

auto isize::operator=(__PTRDIFF_TYPE__ value) -> isize& {
    isize integer = value;
    swap(integer);
    return *this;
}

auto isize::swap(isize& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto isize::is_signed() const -> bool {
    return true;
}

auto isize::bit_count() const -> usize {
    return sizeof(__PTRDIFF_TYPE__) * 8;
}

auto isize::unwrap() const -> __PTRDIFF_TYPE__ {
    return m_value;
}

auto isize::equal(isize const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto isize::operator==(isize const& rhs) const -> bool {
    return equal(rhs);
}

auto isize::not_equal(isize const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto isize::operator!=(isize const& rhs) const -> bool {
    return not_equal(rhs);
}

auto isize::greater(isize const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto isize::operator>(isize const& rhs) const -> bool {
    return greater(rhs);
}

auto isize::greater_equal(isize const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto isize::operator>=(isize const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto isize::less(isize const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto isize::operator<(isize const& rhs) const -> bool {
    return less(rhs);
}

auto isize::less_equal(isize const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto isize::operator<=(isize const& rhs) const -> bool {
    return less_equal(rhs);
}

auto isize::bit_one_complement() const -> isize {
    return ~m_value;
}

auto isize::operator~() const -> isize {
    return bit_one_complement();
}

auto isize::bit_and(isize const& rhs) const -> isize {
    return m_value & rhs.m_value;
}

auto isize::operator&(isize const& rhs) const -> isize {
    return bit_and(rhs);
}

auto isize::bit_and_assign(isize const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto isize::operator&=(isize const& rhs) -> isize& {
    bit_and_assign(rhs);
    return *this;
}

auto isize::bit_or(isize const& rhs) const -> isize {
    return m_value | rhs.m_value;
}

auto isize::operator|(isize const& rhs) const -> isize {
    return bit_or(rhs);
}

auto isize::bit_or_assign(isize const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto isize::operator|=(isize const& rhs) -> isize& {
    bit_or_assign(rhs);
    return *this;
}

auto isize::bit_xor(isize const& rhs) const -> isize {
    return m_value ^ rhs.m_value;
}

auto isize::operator^(isize const& rhs) const -> isize {
    return bit_xor(rhs);
}

auto isize::bit_xor_assign(isize const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto isize::operator^=(isize const& rhs) -> isize& {
    bit_xor_assign(rhs);
    return *this;
}

auto isize::try_left_shift(isize const& rhs) const -> ErrorOr<isize> {
    if ( rhs >= bit_count().as<isize>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return isize(static_cast<NativeInt>(m_value << rhs.m_value));
    }
}

auto isize::left_shift(isize const& rhs) const -> isize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<NativeInt>(m_value << rhs.m_value);
    }
}

auto isize::operator<<(isize const& rhs) const -> isize {
    return left_shift(rhs);
}

auto isize::try_left_shift_assign(isize const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<isize>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto isize::left_shift_assign(isize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto isize::operator<<=(isize const& rhs) -> isize& {
    left_shift_assign(rhs);
    return *this;
}

auto isize::try_right_shift(isize const& rhs) const -> ErrorOr<isize> {
    if ( rhs >= bit_count().as<isize>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return isize(static_cast<NativeInt>(m_value >> rhs.m_value));
    }
}

auto isize::right_shift(isize const& rhs) const -> isize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<NativeInt>(m_value >> rhs.m_value);
    }
}

auto isize::operator>>(isize const& rhs) const -> isize {
    return right_shift(rhs);
}

auto isize::try_right_shift_assign(isize const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<isize>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto isize::right_shift_assign(isize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto isize::operator>>=(isize const& rhs) -> isize& {
    right_shift_assign(rhs);
    return *this;
}

auto isize::operator++() -> isize& {
    add_assign(1);
    return *this;
}

auto isize::operator++(int) -> isize {
    auto __prev = *this;
    add_assign(1);
    return __prev;
}

auto isize::operator--() -> isize& {
    sub_assign(1);
    return *this;
}

auto isize::operator--(int) -> isize {
    auto __prev = *this;
    sub_assign(1);
    return __prev;
}

auto isize::try_add(isize const& rhs) const -> ErrorOr<isize> {
    isize::NativeInt __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return isize(__value);
    }
}

auto isize::add(isize const& rhs) const -> isize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto isize::operator+(isize const& rhs) const -> isize {
    return add(rhs);
}

auto isize::try_add_assign(isize const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto isize::add_assign(isize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto isize::operator+=(isize const& rhs) -> isize& {
    add_assign(rhs);
    return *this;
}

auto isize::try_sub(isize const& rhs) const -> ErrorOr<isize> {
    isize::NativeInt __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return isize(__value);
    }
}

auto isize::sub(isize const& rhs) const -> isize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto isize::operator-(isize const& rhs) const -> isize {
    return sub(rhs);
}

auto isize::try_sub_assign(isize const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto isize::sub_assign(isize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto isize::operator-=(isize const& rhs) -> isize& {
    sub_assign(rhs);
    return *this;
}

auto isize::try_mul(isize const& rhs) const -> ErrorOr<isize> {
    isize::NativeInt __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return isize(__value);
    }
}

auto isize::mul(isize const& rhs) const -> isize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto isize::operator*(isize const& rhs) const -> isize {
    return mul(rhs);
}

auto isize::try_mul_assign(isize const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto isize::mul_assign(isize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto isize::operator*=(isize const& rhs) -> isize& {
    mul_assign(rhs);
    return *this;
}

auto isize::try_div(isize const& rhs) const -> ErrorOr<isize> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return isize(static_cast<NativeInt>(m_value / rhs.m_value));
    }
}

auto isize::div(isize const& rhs) const -> isize {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto isize::operator/(isize const& rhs) const -> isize {
    return div(rhs);
}

auto isize::try_div_assign(isize const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto isize::div_assign(isize const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto isize::operator/=(isize const& rhs) -> isize& {
    div_assign(rhs);
    return *this;
}

auto isize::operator%(isize const& rhs) const -> isize {
    return m_value % rhs.m_value;
}

auto isize::operator%=(isize const& rhs) -> isize& {
    m_value %= rhs.m_value;
    return *this;
}

auto isize::atomic_load(MemOrder mem_order) volatile -> isize {
    NativeInt __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto isize::atomic_store(isize rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto isize::atomic_add(isize rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto isize::atomic_sub(isize rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto isize::atomic_fetch_add(isize rhs, MemOrder mem_order) volatile -> isize {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto isize::atomic_fetch_sub(isize rhs, MemOrder mem_order) volatile -> isize {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto isize::atomic_add_fetch(isize rhs, MemOrder mem_order) volatile -> isize {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto isize::atomic_sub_fetch(isize rhs, MemOrder mem_order) volatile -> isize {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto isize::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}

namespace Cxx {

auto swap(isize& lhs, isize& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */