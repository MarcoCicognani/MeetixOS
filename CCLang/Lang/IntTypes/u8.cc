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

#include <CCLang/Lang/IntTypes/u8.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto u8::max(u8 const& lhs, u8 const& rhs) -> u8 {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto u8::min(u8 const& lhs, u8 const& rhs) -> u8 {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto u8::ceil_div(u8 const& lhs, u8 const& rhs) -> u8 {
    u8 res = lhs / rhs;
    if ( (lhs % rhs) != 0 ) {
        res += 1;
    }
    return res;
}

auto u8::range(u8 const& begin, u8 const& end) -> Range<u8> {
    return Range<u8>(begin, end);
}

auto u8::range_inclusive(u8 const& begin, u8 const& last) -> RangeInclusive<u8> {
    return RangeInclusive<u8>(begin, last);
}

u8::u8(__UINT8_TYPE__ value)
    : m_value(value) {
}

auto u8::operator=(__UINT8_TYPE__ value) -> u8& {
    u8 u = value;
    swap(u);
    return *this;
}

auto u8::swap(u8& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto u8::is_signed() const -> bool {
    return false;
}

auto u8::try_bit_at(usize index) const -> ErrorOr<bool> {
    if ( index > bit_count() ) {
        return Error::from_code(ErrorCode::IndexOutOfRange);
    } else {
        return (m_value & (1 << index.unwrap())) == (1 << index.unwrap());
    }
}

auto u8::bit_at(usize index) const -> bool {
    return must$(try_bit_at(index));
}

auto u8::try_set_bit(usize index, bool bit_value) -> ErrorOr<void> {
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

auto u8::set_bit(usize index, bool bit_value) -> void {
    must$(try_set_bit(index, bit_value));
}

auto u8::count_zeroes() const -> usize {
    return m_value; /* TODO implement */
}

auto u8::count_ones() const -> usize {
    return m_value; /* TODO implement */
}

auto u8::bit_count() const -> usize {
    return sizeof(__UINT8_TYPE__) * 8;
}

auto u8::unwrap() const -> __UINT8_TYPE__ {
    return m_value;
}

auto u8::equal(u8 const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto u8::operator==(u8 const& rhs) const -> bool {
    return equal(rhs);
}

auto u8::not_equal(u8 const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto u8::operator!=(u8 const& rhs) const -> bool {
    return not_equal(rhs);
}

auto u8::greater(u8 const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto u8::operator>(u8 const& rhs) const -> bool {
    return greater(rhs);
}

auto u8::greater_equal(u8 const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto u8::operator>=(u8 const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto u8::less(u8 const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto u8::operator<(u8 const& rhs) const -> bool {
    return less(rhs);
}

auto u8::less_equal(u8 const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto u8::operator<=(u8 const& rhs) const -> bool {
    return less_equal(rhs);
}

auto u8::bit_one_complement() const -> u8 {
    return ~m_value;
}

auto u8::operator~() const -> u8 {
    return bit_one_complement();
}

auto u8::bit_and(u8 const& rhs) const -> u8 {
    return m_value & rhs.m_value;
}

auto u8::operator&(u8 const& rhs) const -> u8 {
    return bit_and(rhs);
}

auto u8::bit_and_assign(u8 const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto u8::operator&=(u8 const& rhs) -> u8& {
    bit_and_assign(rhs);
    return *this;
}

auto u8::bit_or(u8 const& rhs) const -> u8 {
    return m_value | rhs.m_value;
}

auto u8::operator|(u8 const& rhs) const -> u8 {
    return bit_or(rhs);
}

auto u8::bit_or_assign(u8 const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto u8::operator|=(u8 const& rhs) -> u8& {
    bit_or_assign(rhs);
    return *this;
}

auto u8::bit_xor(u8 const& rhs) const -> u8 {
    return m_value ^ rhs.m_value;
}

auto u8::operator^(u8 const& rhs) const -> u8 {
    return bit_xor(rhs);
}

auto u8::bit_xor_assign(u8 const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto u8::operator^=(u8 const& rhs) -> u8& {
    bit_xor_assign(rhs);
    return *this;
}

auto u8::try_left_shift(u8 const& rhs) const -> ErrorOr<u8> {
    if ( rhs >= bit_count().as<u8>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return u8(static_cast<NativeInt>(m_value << rhs.m_value));
    }
}

auto u8::left_shift(u8 const& rhs) const -> u8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<NativeInt>(m_value << rhs.m_value);
    }
}

auto u8::operator<<(u8 const& rhs) const -> u8 {
    return left_shift(rhs);
}

auto u8::try_left_shift_assign(u8 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<u8>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto u8::left_shift_assign(u8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto u8::operator<<=(u8 const& rhs) -> u8& {
    left_shift_assign(rhs);
    return *this;
}

auto u8::try_right_shift(u8 const& rhs) const -> ErrorOr<u8> {
    if ( rhs >= bit_count().as<u8>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return u8(static_cast<NativeInt>(m_value >> rhs.m_value));
    }
}

auto u8::right_shift(u8 const& rhs) const -> u8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<NativeInt>(m_value >> rhs.m_value);
    }
}

auto u8::operator>>(u8 const& rhs) const -> u8 {
    return right_shift(rhs);
}

auto u8::try_right_shift_assign(u8 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<u8>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto u8::right_shift_assign(u8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto u8::operator>>=(u8 const& rhs) -> u8& {
    right_shift_assign(rhs);
    return *this;
}

auto u8::operator++() -> u8& {
    add_assign(1);
    return *this;
}

auto u8::operator++(int) -> u8 {
    u8 __prev = *this;
    add_assign(1);
    return __prev;
}

auto u8::operator--() -> u8& {
    sub_assign(1);
    return *this;
}

auto u8::operator--(int) -> u8 {
    u8 __prev = *this;
    sub_assign(1);
    return __prev;
}

auto u8::try_add(u8 const& rhs) const -> ErrorOr<u8> {
    NativeInt __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u8(__value);
    }
}

auto u8::add(u8 const& rhs) const -> u8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto u8::operator+(u8 const& rhs) const -> u8 {
    return add(rhs);
}

auto u8::try_add_assign(u8 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto u8::add_assign(u8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto u8::operator+=(u8 const& rhs) -> u8& {
    add_assign(rhs);
    return *this;
}

auto u8::try_sub(u8 const& rhs) const -> ErrorOr<u8> {
    NativeInt __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u8(__value);
    }
}

auto u8::sub(u8 const& rhs) const -> u8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto u8::operator-(u8 const& rhs) const -> u8 {
    return sub(rhs);
}

auto u8::try_sub_assign(u8 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto u8::sub_assign(u8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto u8::operator-=(u8 const& rhs) -> u8& {
    sub_assign(rhs);
    return *this;
}

auto u8::try_mul(u8 const& rhs) const -> ErrorOr<u8> {
    NativeInt __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return u8(__value);
    }
}

auto u8::mul(u8 const& rhs) const -> u8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto u8::operator*(u8 const& rhs) const -> u8 {
    return mul(rhs);
}

auto u8::try_mul_assign(u8 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto u8::mul_assign(u8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto u8::operator*=(u8 const& rhs) -> u8& {
    mul_assign(rhs);
    return *this;
}

auto u8::try_div(u8 const& rhs) const -> ErrorOr<u8> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return u8(static_cast<NativeInt>(m_value / rhs.m_value));
    }
}

auto u8::div(u8 const& rhs) const -> u8 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto u8::operator/(u8 const& rhs) const -> u8 {
    return div(rhs);
}

auto u8::try_div_assign(u8 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto u8::div_assign(u8 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto u8::operator/=(u8 const& rhs) -> u8& {
    div_assign(rhs);
    return *this;
}

auto u8::operator%(u8 const& rhs) const -> u8 {
    return m_value % rhs.m_value;
}

auto u8::operator%=(u8 const& rhs) -> u8& {
    m_value %= rhs.m_value;
    return *this;
}

auto u8::atomic_load(MemOrder mem_order) volatile -> u8 {
    NativeInt __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto u8::atomic_store(u8 rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u8::atomic_add(u8 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto u8::atomic_sub(u8 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto u8::atomic_fetch_add(u8 rhs, MemOrder mem_order) volatile -> u8 {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u8::atomic_fetch_sub(u8 rhs, MemOrder mem_order) volatile -> u8 {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u8::atomic_add_fetch(u8 rhs, MemOrder mem_order) volatile -> u8 {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u8::atomic_sub_fetch(u8 rhs, MemOrder mem_order) volatile -> u8 {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto u8::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}

auto operator<<(__UINT8_TYPE__ lhs, u8 rhs) -> u8 {
    return u8(lhs) << rhs;
}

auto operator>>(__UINT8_TYPE__ lhs, u8 rhs) -> u8 {
    return u8(lhs) >> rhs;
}

namespace Cxx {

auto swap(u8& lhs, u8& rhs) -> void {
    lhs.swap(rhs);
}

} /* namespace Cxx */