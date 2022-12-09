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

#include <CCLang/Lang/IntTypes/i16.hh>

#include <CCLang/Core/ErrorOr.hh>
#include <CCLang/Lang/Cxx.hh>
#include <CCLang/Lang/IntTypes/usize.hh>
#include <CCLang/Lang/Must.hh>
#include <CCLang/Lang/Range.hh>
#include <CCLang/Lang/Try.hh>

auto i16::max(i16 const& lhs, i16 const& rhs) -> i16 {
    if ( lhs > rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto i16::min(i16 const& lhs, i16 const& rhs) -> i16 {
    if ( lhs < rhs ) {
        return lhs;
    } else {
        return rhs;
    }
}

auto i16::range(i16 const& begin, i16 const& end) -> Range<i16> {
    return Range<i16>(begin, end);
}

auto i16::range_inclusive(i16 const& begin, i16 const& last) -> RangeInclusive<i16> {
    return RangeInclusive<i16>(begin, last);
}

i16::i16(__INT16_TYPE__ value)
    : m_value(value) {
}

i16::i16(i16&& rhs)
    : m_value(Cxx::exchange(rhs.m_value, 0)) {
}

auto i16::operator=(__INT16_TYPE__ value) -> i16& {
    i16 integer = value;
    swap(integer);
    return *this;
}

auto i16::operator=(i16 const& rhs) -> i16& {
    auto integer = rhs;
    swap(integer);
    return *this;
}

auto i16::operator=(i16&& rhs) -> i16& {
    auto integer = Cxx::move(rhs);
    swap(integer);
    return *this;
}

auto i16::swap(i16& rhs) -> void {
    Cxx::swap(m_value, rhs.m_value);
}

auto i16::is_signed() const -> bool {
    return true;
}

auto i16::bit_count() const -> usize {
    return sizeof(__INT16_TYPE__) * 8;
}

auto i16::unwrap() const -> __INT16_TYPE__ {
    return m_value;
}

auto i16::equal(i16 const& rhs) const -> bool {
    return m_value == rhs.m_value;
}

auto i16::operator==(i16 const& rhs) const -> bool {
    return equal(rhs);
}

auto i16::not_equal(i16 const& rhs) const -> bool {
    return m_value != rhs.m_value;
}

auto i16::operator!=(i16 const& rhs) const -> bool {
    return not_equal(rhs);
}

auto i16::greater(i16 const& rhs) const -> bool {
    return m_value > rhs.m_value;
}

auto i16::operator>(i16 const& rhs) const -> bool {
    return greater(rhs);
}

auto i16::greater_equal(i16 const& rhs) const -> bool {
    return m_value >= rhs.m_value;
}

auto i16::operator>=(i16 const& rhs) const -> bool {
    return greater_equal(rhs);
}

auto i16::less(i16 const& rhs) const -> bool {
    return m_value < rhs.m_value;
}

auto i16::operator<(i16 const& rhs) const -> bool {
    return less(rhs);
}

auto i16::less_equal(i16 const& rhs) const -> bool {
    return m_value <= rhs.m_value;
}

auto i16::operator<=(i16 const& rhs) const -> bool {
    return less_equal(rhs);
}

auto i16::bit_one_complement() const -> i16 {
    return ~m_value;
}

auto i16::operator~() const -> i16 {
    return bit_one_complement();
}

auto i16::bit_and(i16 const& rhs) const -> i16 {
    return m_value & rhs.m_value;
}

auto i16::operator&(i16 const& rhs) const -> i16 {
    return bit_and(rhs);
}

auto i16::bit_and_assign(i16 const& rhs) -> void {
    m_value &= rhs.m_value;
}

auto i16::operator&=(i16 const& rhs) -> i16& {
    bit_and_assign(rhs);
    return *this;
}

auto i16::bit_or(i16 const& rhs) const -> i16 {
    return m_value | rhs.m_value;
}

auto i16::operator|(i16 const& rhs) const -> i16 {
    return bit_or(rhs);
}

auto i16::bit_or_assign(i16 const& rhs) -> void {
    m_value |= rhs.m_value;
}

auto i16::operator|=(i16 const& rhs) -> i16& {
    bit_or_assign(rhs);
    return *this;
}

auto i16::bit_xor(i16 const& rhs) const -> i16 {
    return m_value ^ rhs.m_value;
}

auto i16::operator^(i16 const& rhs) const -> i16 {
    return bit_xor(rhs);
}

auto i16::bit_xor_assign(i16 const& rhs) -> void {
    m_value ^= rhs.m_value;
}

auto i16::operator^=(i16 const& rhs) -> i16& {
    bit_xor_assign(rhs);
    return *this;
}

auto i16::try_left_shift(i16 const& rhs) const -> ErrorOr<i16> {
    if ( rhs >= bit_count().as<i16>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return i16(static_cast<CCIntegerType>(m_value << rhs.m_value));
    }
}

auto i16::left_shift(i16 const& rhs) const -> i16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_left_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value << rhs.m_value);
    }
}

auto i16::operator<<(i16 const& rhs) const -> i16 {
    return left_shift(rhs);
}

auto i16::try_left_shift_assign(i16 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<i16>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value <<= rhs.m_value;
        return {};
    }
}

auto i16::left_shift_assign(i16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_left_shift_assign(rhs));
    } else {
        m_value <<= rhs.m_value;
    }
}

auto i16::operator<<=(i16 const& rhs) -> i16& {
    left_shift_assign(rhs);
    return *this;
}

auto i16::try_right_shift(i16 const& rhs) const -> ErrorOr<i16> {
    if ( rhs >= bit_count().as<i16>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        return i16(static_cast<CCIntegerType>(m_value >> rhs.m_value));
    }
}

auto i16::right_shift(i16 const& rhs) const -> i16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_right_shift(rhs));
    } else {
        return static_cast<CCIntegerType>(m_value >> rhs.m_value);
    }
}

auto i16::operator>>(i16 const& rhs) const -> i16 {
    return right_shift(rhs);
}

auto i16::try_right_shift_assign(i16 const& rhs) -> ErrorOr<void> {
    if ( rhs >= bit_count().as<i16>() ) {
        return Error::from_code(ErrorCode::ShiftOverflow);
    } else {
        m_value >>= rhs.m_value;
        return {};
    }
}

auto i16::right_shift_assign(i16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_right_shift_assign(rhs));
    } else {
        m_value >>= rhs.m_value;
    }
}

auto i16::operator>>=(i16 const& rhs) -> i16& {
    right_shift_assign(rhs);
    return *this;
}

auto i16::operator++() -> i16& {
    add_assign(1);
    return *this;
}

auto i16::operator++(int) -> i16 {
    auto __prev = *this;
    add_assign(1);
    return __prev;
}

auto i16::operator--() -> i16& {
    sub_assign(1);
    return *this;
}

auto i16::operator--(int) -> i16 {
    auto __prev = *this;
    sub_assign(1);
    return __prev;
}

auto i16::try_add(i16 const& rhs) const -> ErrorOr<i16> {
    i16::CCIntegerType __value;
    if ( __builtin_add_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i16(__value);
    }
}

auto i16::add(i16 const& rhs) const -> i16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_add(rhs));
    } else {
        return m_value + rhs.m_value;
    }
}

auto i16::operator+(i16 const& rhs) const -> i16 {
    return add(rhs);
}

auto i16::try_add_assign(i16 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_add(rhs)).m_value;
    return {};
}

auto i16::add_assign(i16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_add_assign(rhs));
    } else {
        m_value += rhs.m_value;
    }
}

auto i16::operator+=(i16 const& rhs) -> i16& {
    add_assign(rhs);
    return *this;
}

auto i16::try_sub(i16 const& rhs) const -> ErrorOr<i16> {
    i16::CCIntegerType __value;
    if ( __builtin_sub_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i16(__value);
    }
}

auto i16::sub(i16 const& rhs) const -> i16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_sub(rhs));
    } else {
        return m_value - rhs.m_value;
    }
}

auto i16::operator-(i16 const& rhs) const -> i16 {
    return sub(rhs);
}

auto i16::try_sub_assign(i16 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_sub(rhs)).m_value;
    return {};
}

auto i16::sub_assign(i16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_sub_assign(rhs));
    } else {
        m_value -= rhs.m_value;
    }
}

auto i16::operator-=(i16 const& rhs) -> i16& {
    sub_assign(rhs);
    return *this;
}

auto i16::try_mul(i16 const& rhs) const -> ErrorOr<i16> {
    i16::CCIntegerType __value;
    if ( __builtin_mul_overflow(m_value, rhs.m_value, &__value) ) {
        return Error::from_code(ErrorCode::IntOverflow);
    } else {
        return i16(__value);
    }
}

auto i16::mul(i16 const& rhs) const -> i16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul(rhs));
    } else {
        return m_value * rhs.m_value;
    }
}

auto i16::operator*(i16 const& rhs) const -> i16 {
    return mul(rhs);
}

auto i16::try_mul_assign(i16 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_mul(rhs)).m_value;
    return {};
}

auto i16::mul_assign(i16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_mul_assign(rhs));
    } else {
        m_value *= rhs.m_value;
    }
}

auto i16::operator*=(i16 const& rhs) -> i16& {
    mul_assign(rhs);
    return *this;
}

auto i16::try_div(i16 const& rhs) const -> ErrorOr<i16> {
    if ( rhs == 0 ) {
        return Error::from_code(ErrorCode::DivisionByZero);
    } else {
        return i16(static_cast<CCIntegerType>(m_value / rhs.m_value));
    }
}

auto i16::div(i16 const& rhs) const -> i16 {
    if constexpr ( CCLangSafeIntegerOperations ) {
        return must$(try_div(rhs));
    } else {
        return m_value / rhs.m_value;
    }
}

auto i16::operator/(i16 const& rhs) const -> i16 {
    return div(rhs);
}

auto i16::try_div_assign(i16 const& rhs) -> ErrorOr<void> {
    m_value = try$(try_div(rhs)).m_value;
    return {};
}

auto i16::div_assign(i16 const& rhs) -> void {
    if constexpr ( CCLangSafeIntegerOperations ) {
        must$(try_div_assign(rhs));
    } else {
        m_value /= rhs.m_value;
    }
}

auto i16::operator/=(i16 const& rhs) -> i16& {
    div_assign(rhs);
    return *this;
}

auto i16::operator%(i16 const& rhs) const -> i16 {
    return m_value % rhs.m_value;
}

auto i16::operator%=(i16 const& rhs) -> i16& {
    m_value %= rhs.m_value;
    return *this;
}

auto i16::atomic_load(MemOrder mem_order) volatile -> i16 {
    CCIntegerType __value;
    __atomic_load(&m_value, &__value, static_cast<UnderlyingType<MemOrder>>(mem_order));
    return __value;
}

auto i16::atomic_store(i16 rhs, MemOrder mem_order) volatile -> void {
    __atomic_store(&m_value, &rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i16::atomic_add(i16 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_add(rhs, mem_order);
}

auto i16::atomic_sub(i16 rhs, MemOrder mem_order) volatile -> void {
    (void)atomic_fetch_sub(rhs, mem_order);
}

auto i16::atomic_fetch_add(i16 rhs, MemOrder mem_order) volatile -> i16 {
    return __atomic_fetch_add(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i16::atomic_fetch_sub(i16 rhs, MemOrder mem_order) volatile -> i16 {
    return __atomic_fetch_sub(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i16::atomic_add_fetch(i16 rhs, MemOrder mem_order) volatile -> i16 {
    return __atomic_add_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i16::atomic_sub_fetch(i16 rhs, MemOrder mem_order) volatile -> i16 {
    return __atomic_sub_fetch(&m_value, rhs.m_value, static_cast<UnderlyingType<MemOrder>>(mem_order));
}

auto i16::hash_code() const -> usize {
    auto hash_key = as<usize>();
    hash_key += ~(hash_key << 15);
    hash_key ^= (hash_key >> 10);
    hash_key += (hash_key << 3);
    hash_key ^= (hash_key >> 6);
    hash_key += ~(hash_key << 11);
    hash_key ^= (hash_key >> 16);
    return hash_key;
}
