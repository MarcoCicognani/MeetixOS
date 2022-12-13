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

#include <CCLang/Lang/IntTypes/u64.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto u64::max(u64 const& lhs, u64 const& rhs) -> u64 {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto u64::min(u64 const& lhs, u64 const& rhs) -> u64 {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto u64::ceil_div(u64 const& lhs, u64 const& rhs) -> u64 {
    u64 res = lhs / rhs;
    if ( (lhs % rhs) != 0 ) {
        res += 1;
    }
    return res;
}

auto u64::range(u64 const& begin, u64 const& end) -> Range<u64> {
    return Range<u64>(begin, end);
}

auto u64::range_inclusive(u64 const& begin, u64 const& last) -> RangeInclusive<u64> {
    return RangeInclusive<u64>(begin, last);
}

u64::u64(__UINT64_TYPE__ value)
    : m_value(value) {
}

auto u64::operator=(__UINT64_TYPE__ value) -> u64& {
    u64 u = value;
    swap(u);
    return *this;
}

auto u64::swap(u64& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto u64::is_signed() const -> bool {
    return false;
}

auto u64::try_bit_at(usize index) const -> ErrorOr<bool> {
    if ( index > bit_count() ) {
        return Error::from_code(ErrorCode::IndexOutOfRange);
    } else {
        return (m_value & (1 << index.unwrap())) == (1 << index.unwrap());
    }
}

auto u64::bit_at(usize index) const -> bool {
    return must$(try_bit_at(index));
}

auto u64::try_set_bit(usize index, bool bit_value) -> ErrorOr<void> {
    if ( index > bit_count() ) {
        return Error::from_code(ErrorCode::IndexOutOfRange);
    }

    if ( bit_value ) {
        m_value |= 1 << index.unwrap();
    } else {
        m_value &= ~(1 << index.unwrap());
    }
    return {};
}

auto u64::set_bit(usize index, bool bit_value) -> void {
    must$(try_set_bit(index, bit_value));
}

auto u64::count_zeroes() const -> usize {
    return m_value; /* TODO implement */
}

auto u64::count_ones() const -> usize {
    return m_value; /* TODO implement */
}

auto u64::bit_count() const -> usize {
    return sizeof(__UINT64_TYPE__) * 8;
}

auto u64::unwrap() const -> __UINT64_TYPE__ {
    return m_value;
}

auto u64::equal(u64 const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto u64::operator==(u64 const& rhs) const -> bool {
    return equal(rhs);
}

auto u64::not_equal(u64 const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto u64::operator!=(u64 const& rhs) const -> bool {
    return not_equal(rhs);
}

auto u64::greater(u64 const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto u64::operator>(u64 const& rhs) const -> bool {
    return greater(rhs);
}

auto u64::greater_equal(u64 const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto u64::operator>=(u64 const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto u64::less(u64 const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto u64::operator<(u64 const& rhs) const -> bool {
    return less(rhs);
}

auto u64::less_equal(u64 const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto u64::operator<=(u64 const& rhs) const -> bool {
    return less_equal(rhs);
}

auto u64::bit_one_complement() const -> u64 {
    return ~m_value;
}

auto u64::operator~() const -> u64 {
    return bit_one_complement();
}

auto u64::bit_and(u64 const& rhs) const -> u64 {
    return m_value & rhs.m_value;
}

auto u64::operator&(u64 const& rhs) const -> u64 {
    return bit_and(rhs);
}

auto u64::bit_and_assign(u64 const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto u64::operator&=(u64 const& rhs) -> u64& {
    bit_and_assign(rhs);
    return *this;
}

auto u64::bit_or(u64 const& rhs) const -> u64 {
    return m_value | rhs.m_value;
}

auto u64::operator|(u64 const& rhs) const -> u64 {
    return bit_or(rhs);
}

auto u64::bit_or_assign(u64 const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto u64::operator|=(u64 const& rhs) -> u64& {
    bit_or_assign(rhs);
    return *this;
}

auto u64::bit_xor(u64 const& rhs) const -> u64 {
    return m_value ^ rhs.m_value;
}

auto u64::operator^(u64 const& rhs) const -> u64 {
    return bit_xor(rhs);
}

auto u64::bit_xor_assign(u64 const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto u64::operator^=(u64 const& rhs) -> u64& {
    bit_xor_assign(rhs);
    return *this;
}

auto u64::try_left_shift(u64 const& rhs) const -> ErrorOr<u64> {
    if ( rhs >= bit_count().as<u64>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return u64(static_cast<NativeInt>(m_value << rhs.m_value));
    }
}

auto u64::left_shift(u64 const& rhs) const -> u64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<NativeInt>(m_value << rhs.m_value);
    }
}

auto u64::operator<<(u64 const& rhs) const -> u64 {
    return left_shift(rhs);
}

auto u64::try_left_shift_assign(u64 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<u64>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto u64::left_shift_assign(u64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto u64::operator<<=(u64 const& rhs) -> u64& {
    left_shift_assign(rhs);
    return *this;
}

auto u64::try_right_shift(u64 const& rhs) const -> ErrorOr<u64> {
    if ( rhs >= bit_count().as<u64>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return u64(static_cast<NativeInt>(m_value >> rhs.m_value));
    }
}

auto u64::right_shift(u64 const& rhs) const -> u64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<NativeInt>(m_value >> rhs.m_value);
    }
}

auto u64::operator>>(u64 const& rhs) const -> u64 {
    return right_shift(rhs);
}

auto u64::try_right_shift_assign(u64 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<u64>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto u64::right_shift_assign(u64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto u64::operator>>=(u64 const& rhs) -> u64& {
    right_shift_assign(rhs);
    return *this;
}

auto u64::operator++() -> u64& {
    add_assign(1);
    return *this;
}

auto u64::operator++(int) -> u64 {
    u64 __prev = *this;
    add_assign(1);
    return __prev;
}

auto u64::operator--() -> u64& {
    sub_assign(1);
    return *this;
}

auto u64::operator--(int) -> u64 {
    u64 __prev = *this;
    sub_assign(1);
    return __prev;
}

auto u64::try_add(u64 const& rhs) const -> ErrorOr<u64> {
    NativeInt __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u64(__value);
    }
}

auto u64::add(u64 const& rhs) const -> u64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto u64::operator+(u64 const& rhs) const -> u64 {
    return add(rhs);
}

auto u64::try_add_assign(u64 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto u64::add_assign(u64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto u64::operator+=(u64 const& rhs) -> u64& {
    add_assign(rhs);
    return *this;
}

auto u64::try_sub(u64 const& rhs) const -> ErrorOr<u64> {
    NativeInt __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u64(__value);
    }
}

auto u64::sub(u64 const& rhs) const -> u64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto u64::operator-(u64 const& rhs) const -> u64 {
    return sub(rhs);
}

auto u64::try_sub_assign(u64 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto u64::sub_assign(u64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto u64::operator-=(u64 const& rhs) -> u64& {
    sub_assign(rhs);
    return *this;
}

auto u64::try_mul(u64 const& rhs) const -> ErrorOr<u64> {
    NativeInt __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u64(__value);
    }
}

auto u64::mul(u64 const& rhs) const -> u64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto u64::operator*(u64 const& rhs) const -> u64 {
    return mul(rhs);
}

auto u64::try_mul_assign(u64 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto u64::mul_assign(u64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto u64::operator*=(u64 const& rhs) -> u64& {
    mul_assign(rhs);
    return *this;
}

auto u64::try_div(u64 const& rhs) const -> ErrorOr<u64> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return u64(static_cast<NativeInt>(m_value / rhs.m_value));
    }
}

auto u64::div(u64 const& rhs) const -> u64 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto u64::operator/(u64 const& rhs) const -> u64 {
    return div(rhs);
}

auto u64::try_div_assign(u64 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto u64::div_assign(u64 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto u64::operator/=(u64 const& rhs) -> u64& {
    div_assign(rhs);
    return *this;
}

auto u64::operator%(u64 const& rhs) const -> u64 {
    return m_value % rhs.m_value;
}

auto u64::operator%=(u64 const& rhs) -> u64& {
    m_value %= rhs.m_value;
    return *this;
}

auto u64::atomic_load(MemOrder mem_order) volatile -> u64 {
    NativeInt __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto u64::atomic_store(u64 rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u64::atomic_add(u64 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto u64::atomic_sub(u64 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto u64::atomic_fetch_add(u64 rhs, MemOrder mem_order) volatile -> u64 {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u64::atomic_fetch_sub(u64 rhs, MemOrder mem_order) volatile -> u64 {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u64::atomic_add_fetch(u64 rhs, MemOrder mem_order) volatile -> u64 {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u64::atomic_sub_fetch(u64 rhs, MemOrder mem_order) volatile -> u64 {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u64::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}

auto operator<<(__UINT64_TYPE__ lhs, u64 rhs) -> u64 {
    return u64(lhs) << rhs;
}

auto operator>>(__UINT64_TYPE__ lhs, u64 rhs) -> u64 {
    return u64(lhs) >> rhs;
}

namespace Cxx {

auto swap(u64& lhs, u64& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */